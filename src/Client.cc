//  $Id: Client.cc,v 1.50 2001/04/13 17:34:56 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <config.h>
#include "globals.hh"
#include "Client.hh"
#include "FadeOut.hh"
#include "Playfield.hh"
#include "LevelInterrupt.hh"
#include "System.hh"
#include "Result.hh"
#include "algo.hh"
#include "Timer.hh"
#include "PingusResource.hh"
#include "Display.hh"
#include "PingusLevelResult.hh"
#include "PingusSound.hh"
#include "PingusError.hh"
#include "OptionMenu.hh"
#include "PLFPLF.hh"
#include "XMLPLF.hh"
#include "DeltaManager.hh"
#include "Cursor.hh"
#include "MouseController.hh"
#include "GamepadController.hh"

using boost::shared_ptr;

bool Client::gui_is_init;
shared_ptr<ButtonPanel>   Client::button_panel;
shared_ptr<PingusCounter> Client::pcounter;
shared_ptr<Playfield>     Client::playfield;
shared_ptr<TimeDisplay>   Client::time_display;
shared_ptr<SmallMap>      Client::small_map;
shared_ptr<HurryUp>       Client::hurry_up;

Client::Client(boost::shared_ptr<Controller> arg_controller, 
	       boost::shared_ptr<Server> s)
  : controller (arg_controller)
{
  //player = 0;
  server = s;
  fast_forward = false;
  pause = false;
  skip_frame = 0;
  do_replay = false;
  is_finished = false;

  Display::add_flip_screen_hook(new Cursor ("cursors/cross", "core", controller));
  Display::add_flip_screen_hook(new Cursor ("cursors/cursor", "core", boost::shared_ptr<Controller>(new MouseController ())));
}

Client::~Client()
{
  std::cout << "Client:~Client" << std::endl;
  deinit_display();
}

void
Client::start()
{
  fast_forward = false;
  pause = false;
  do_replay = false;
  is_finished = false;
  skip_frame = 0;

  play_level(server->get_plf ());

  FadeOut::random();

  if (verbose) std::cout << "Displaying results..." << CL_System::get_time()  << std::flush;
  PingusLevelResult r(server->get_world(), controller);
  r.draw();
  
  if (verbose) std::cout << "finished " << CL_System::get_time()  << std::endl;
}

void 
Client::init_display()
{
  Timer timer;
  
  timer.start();
  if (verbose) std::cout << "Client: Generating UI elements..." << std::flush;

  Display::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor", 
						   PingusResource::get("game")));
  Display::show_cursor();
  
  playfield = shared_ptr<Playfield>(new Playfield(plf, server->get_world(),
						  controller));
    
  button_panel = shared_ptr<ButtonPanel>(new ButtonPanel(plf, controller));
  pcounter     = shared_ptr<PingusCounter>(new PingusCounter());
  small_map    = shared_ptr<SmallMap>(new SmallMap());
  time_display = shared_ptr<TimeDisplay>(new TimeDisplay());
  hurry_up     = shared_ptr<HurryUp>(new HurryUp());
  gui_is_init = true;
   
  button_panel->set_server(server.get ());
  time_display->set_server(server.get ());
  button_panel->set_client(this);
  pcounter->set_client(this);
  small_map->set_client(this);
  hurry_up->set_client(this);

  playfield->set_clip_rect(0, 0, 
			   CL_Display::get_width(),
			   CL_Display::get_height());

  // Connect the button_panel with the playfield
  playfield->set_buttons(button_panel);
  playfield->set_server(server.get ());
  playfield->set_client(this);

  // Adding all GuiObj's to the screen
  obj.push_back(playfield);
  obj.push_back(pcounter);
  obj.push_back(time_display);
  obj.push_back(button_panel);
  obj.push_back(small_map);
  obj.push_back(hurry_up);

  if (verbose) std::cout << "done " << timer.stop() << std::endl;
}

void 
Client::deinit_display()
{
  Display::hide_cursor();
}

void
Client::resize_display()
{
}

void 
Client::play_level(boost::shared_ptr<PLF> arg_plf)
{
  Timer timer;

  plf = arg_plf;

  timer.start();
  std::cout << "Client::play_level(), Reading PLF..." << std::flush;

  register_event_handler();

  init_display(); 

  if (verbose)
    std::cout << "Client: Entering main_loop. Startup time: " 
	      << CL_System::get_time() << " msec." << std::endl;
  
  // Clear both buffers
  CL_Display::clear_display();
  Display::flip_display();
  CL_Display::clear_display();

#ifdef HAVE_LIBSDL_MIXER
  PingusSound::play_mod(find_file(pingus_datadir, "music/" + plf->get_music().res_name));
#endif

  /** Main game loop */
  DeltaManager delta;
  float wannabe_delta = 0.0333;
  unsigned int frames = 1;
  bool auto_frame_skip = false;
  float current_delta;
  while (!server->is_finished()) 
    {       
      CL_System::keep_alive ();
      // Update 30 times a second
      current_delta = delta.get ();
      if (current_delta > wannabe_delta)
	{
	  update (delta.getset ());
	  ++frames;
	  if (frames % int(frame_skip) == 0)
	    {
	      if (!auto_frame_skip)
		draw ();
	    }
	}
      else
	{
	  CL_System::sleep (1000 * (wannabe_delta - delta.get ()));
	}
    }

  unregister_event_handler();
}

void 
Client::draw ()
{
  for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
    obj[i]->draw_clipped();

  Display::flip_display();	    
}

void 
Client::update (float delta)
{
  // Let the window move its content
  for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
    obj[i]->updateX();
  
  // Update every 3/100 seconds
  // if (last_update + 30 < CL_System::get_time())
	      
  for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
    obj[i]->update(delta);

  // Let the server process a game loop
  server->update(delta);
  send_next_event();
}

void
Client::send_next_event()
{
}

void
Client::set_fast_forward(bool value)
{
  fast_forward = value;
  skip_frame = 0;
  server->set_fast_forward(value);
}

bool
Client::get_fast_forward()
{
  return fast_forward;
}

void
Client::set_pause(bool value)
{
  pause = value;
  server->set_pause(value);
}

bool
Client::get_pause()
{
  return pause;
}

Result
Client::get_result()
{
  return result;
}

bool
Client::replay()
{
  return do_replay;
}

void
Client::do_restart()
{
  do_replay = true;
  server->set_finished();
}

bool
Client::finished()
{
  return is_finished;
}

void
Client::set_finished()
{
  is_finished = true;
  server->set_finished();  
}

void 
Client::register_event_handler()
{
  if (verbose > 1) std::cout << "Client: register_event_handler()" << std::endl;
  //CL_Input::chain_button_press.push_back(this);
  //CL_Input::chain_button_release.push_back(this);

  //on_button_press_slot = CL_Input::sig_button_press.connect (CL_CreateSlot(this, &Client::on_button_press));
  //on_button_release_slot = CL_Input::sig_button_release.connect (CL_CreateSlot(this, &Client::on_button_release));

  slot_left_pressed   = controller->left->signal_pressed.connect (CL_CreateSlot (this, &Client::on_left_pressed));
  slot_left_released   = controller->left->signal_released.connect (CL_CreateSlot (this, &Client::on_left_released));
  slot_middle_pressed = controller->middle->signal_pressed.connect (CL_CreateSlot (this, &Client::on_middle_pressed));
  slot_right_pressed  = controller->right->signal_pressed.connect (CL_CreateSlot (this, &Client::on_right_pressed));
  slot_right_released  = controller->right->signal_released.connect (CL_CreateSlot (this, &Client::on_right_released));
  slot_abort_pressed  = controller->abort->signal_pressed.connect (CL_CreateSlot (this, &Client::on_abort_pressed));
  slot_pause_pressed  = controller->pause->signal_pressed.connect (CL_CreateSlot (this, &Client::on_pause_pressed));
  slot_scroll_left_pressed  = controller->scroll_left->signal_pressed.connect (CL_CreateSlot (this, &Client::on_scroll_left_pressed));
  slot_scroll_right_pressed = controller->scroll_right->signal_pressed.connect (CL_CreateSlot (this, &Client::on_scroll_right_pressed));

  slot_next_action_pressed  = controller->next_action->signal_pressed.connect (CL_CreateSlot (this, &Client::on_next_action_pressed));
  slot_previous_action_pressed = controller->previous_action->signal_pressed.connect (CL_CreateSlot (this, &Client::on_previous_action_pressed));

  enabled = true;
}

void
Client::unregister_event_handler()
{
  if (verbose > 1) std::cout << "Client: unregister_event_handler()" << std::endl;
  //CL_Input::chain_button_release.remove(this);
  //CL_Input::chain_button_press.remove(this);

  // Disconnect segfaults
  CL_Input::sig_button_press.disconnect (on_button_press_slot); 
  CL_Input::sig_button_release.disconnect (on_button_release_slot); 

  enabled = false;
}

void
Client::disable_event_handler()
{
  enabled = false;
}

void
Client::enable_event_handler()
{
  enabled = true;
}

void
Client::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled)
    return;
  
  if (device == CL_Input::keyboards[0])
    {
      on_keyboard_button_press(key);
    }
  else if (device == CL_Input::pointers[0])
    {
      on_mouse_button_press(key);
    }
  else
    {
      if (verbose > 1) std::cout << "Unknown device pressed: device=" << device << "; key.id=" << key.id << std::endl;
    }
}

void
Client::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled)
    return;

  if (device == CL_Input::keyboards[0])
    {
      on_keyboard_button_release(key);
    }
  else if (device == CL_Input::pointers[0])
    {
      on_mouse_button_release(key);
    }
  else
    {
      if (verbose > 1) std::cout << "Unknown device released: device=" << device << "; key.id=" << key.id << std::endl;
    }  
}

void
Client::on_keyboard_button_press(const CL_Key& key)
{
  if (CL_Keyboard::get_keycode(CL_KEY_LSHIFT)
      || CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
    {
      playfield->scroll_speed = 50;
    }
  else
    {
      playfield->scroll_speed = 15;
    }
}

void
Client::on_keyboard_button_release(const CL_Key& key)
{
  switch (key.id)
    {
    case CL_KEY_O:
      enabled = false;
      Display::hide_cursor();
      option_menu.display ();
      Display::show_cursor();
      enabled = true;
      break;

      // Playfield scrolling	
    case CL_KEY_LEFT:
      playfield->view[playfield->current_view]->set_x_offset(playfield->view[playfield->current_view]->get_x_offset() + playfield->scroll_speed);
      break;
	  
    case CL_KEY_RIGHT:
      playfield->view[playfield->current_view]->set_x_offset(playfield->view[playfield->current_view]->get_x_offset() - playfield->scroll_speed);
      break;
	  
    case CL_KEY_UP:
      playfield->view[playfield->current_view]->set_y_offset(playfield->view[playfield->current_view]->get_y_offset() + playfield->scroll_speed);
      break;
      
    case CL_KEY_DOWN:
      playfield->view[playfield->current_view]->set_y_offset(playfield->view[playfield->current_view]->get_y_offset() - playfield->scroll_speed);
      break;

      // Playfield zooming
    case CL_KEY_PAGEDOWN:
      playfield->view[playfield->current_view]->set_zoom(playfield->view[playfield->current_view]->get_zoom() / 1.05);
      break;
      
    case CL_KEY_PAGEUP:
      playfield->view[playfield->current_view]->set_zoom(playfield->view[playfield->current_view]->get_zoom() / 0.95);
      break;
      
    case CL_KEY_END:
      playfield->view[playfield->current_view]->set_zoom(1.0);
      break;

      // Misc
    case CL_KEY_P:
      pause = !pause;
      server->set_pause(pause);
      break;

    case CL_KEY_A:
      server->send_event("armageddon");
      break;
	  
    case CL_KEY_R:
      do_restart();
      break;
	  
    case CL_KEY_SPACE:
      set_fast_forward(!get_fast_forward());
      break;

    case CL_KEY_ESCAPE:
      server->set_finished();
      break;

    case CL_KEY_F1:
      button_panel->set_button(0);
      break;
    case CL_KEY_F2:
      button_panel->set_button(1);
      break;
    case CL_KEY_F3:
      button_panel->set_button(2);
      break;      
    case CL_KEY_F4:
      button_panel->set_button(3);
      break;      
    case CL_KEY_F5:
      button_panel->set_button(4);
      break;      
    case CL_KEY_F6:
      button_panel->set_button(5);
      break;      
    case CL_KEY_F7:
      button_panel->set_button(6);
      break;      
    case CL_KEY_F8:
      button_panel->set_button(7);
      break;      
    default:
      if (verbose > 1) std::cout << "Client: Got unknown button: ID=" << key.id << " ASCII=" << char(key.ascii) << std::endl;
    }
}

void
Client::on_mouse_button_press(const CL_Key& key)
{
  button_panel->on_button_press(key);
  small_map->on_button_press(key);

  switch(key.id)
    {
    case CL_MOUSE_LEFTBUTTON:
      playfield->on_button_press(key);
      break;
    case CL_MOUSE_MIDDLEBUTTON:
      break;
    case CL_MOUSE_RIGHTBUTTON:
      playfield->enable_scroll_mode();
      break;
    default:
      if (verbose > 1) std::cout << "Client: Unknown mouse button released: " << key.id << std::endl;
    }
}

void
Client::on_mouse_button_release(const CL_Key& key)
{
  button_panel->on_button_release(key);
  small_map->on_button_release(key);

  switch(key.id)
    {
    case CL_MOUSE_LEFTBUTTON:
      break;
    case CL_MOUSE_MIDDLEBUTTON:
      break;
    case CL_MOUSE_RIGHTBUTTON:
      playfield->disable_scroll_mode();
      break;
    default:
      if (verbose > 1) std::cout << "Client: Unknown mouse button released: " << key.id << std::endl;
    }
  return;
}

void
Client:: on_left_pressed (const CL_Vector& pos)
{
  CL_Key key;
  
  key.id = CL_MOUSE_LEFTBUTTON;
  key.x = pos.x;
  key.y = pos.y;
  
  button_panel->on_button_press(key);
  small_map->on_button_press(key);
  playfield->on_button_press(key);
}

void
Client:: on_left_released (const CL_Vector& pos)
{
  CL_Key key;
  
  key.id = CL_MOUSE_LEFTBUTTON;
  key.x = pos.x;
  key.y = pos.y;
  
  button_panel->on_button_release(key);
  small_map->on_button_release(key);
}


void
Client:: on_middle_pressed (const CL_Vector& pos)
{
  std::cout << "Middle Pressed" << std::endl;
}

void
Client:: on_right_pressed (const CL_Vector& pos)
{
  std::cout << "Right Pressed" << std::endl;
 playfield->enable_scroll_mode();
}

void 
Client::on_right_released (const CL_Vector& pos)
{
  std::cout << "Right released" << std::endl;
  playfield->disable_scroll_mode();
}

void
Client:: on_abort_pressed (const CL_Vector& pos)
{
  server->set_finished();
}

void
Client:: on_pause_pressed (const CL_Vector& pos)
{
  set_pause (!get_pause ());
}

void
Client::on_scroll_left_pressed (const CL_Vector& pos)
{
  std::cout << "Scroll left pressed" << std::endl;
}

void
Client::on_scroll_right_pressed (const CL_Vector& pos)
{
  std::cout << "Scroll right pressed" << std::endl;
}

void 
Client::on_next_action_pressed (const CL_Vector& pos)
{
  std::cout << "Action next pressed" << std::endl;
  button_panel->next_action();
}

void 
Client::on_previous_action_pressed (const CL_Vector& pos)
{
  std::cout << "Action previous pressed" << std::endl;
  button_panel->previous_action();
}

/* EOF */
