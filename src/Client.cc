//  $Id: Client.cc,v 1.41 2001/04/11 11:28:24 grumbel Exp $
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

bool Client::gui_is_init;
boost::shared_ptr<ButtonPanel>   Client::button_panel;
boost::shared_ptr<PingusCounter> Client::pcounter;
boost::shared_ptr<Playfield>     Client::playfield;
boost::shared_ptr<TimeDisplay>   Client::time_display;
boost::shared_ptr<SmallMap>      Client::small_map;
boost::shared_ptr<HurryUp>       Client::hurry_up;

Client::Client(Server* s)
{
  player = 0;
  server = s;
  fast_forward = false;
  pause = false;
  skip_frame = 0;
  do_replay = false;
  is_finished = false;
}

Client::~Client()
{
  std::cout << "Client:~Client" << std::endl;
  deinit_display();
}

void
Client::start(DemoPlayer* player)
{
  assert(player);
  this->player = player;

  std::string demo_level_file = "levels/" + player->get_levelname();

  std::cout << "Demo_level_file: " << demo_level_file << std::endl;

  play_level(demo_level_file);
}

void
Client::start(std::string filename, PingusGameMode m)
{
  fast_forward = false;
  pause = false;
  do_replay = false;
  is_finished = false;
  skip_frame = 0;

  mode = m;

  play_level(filename);

  FadeOut::random();

  if (verbose) std::cout << "Displaying results..." << CL_System::get_time()  << std::flush;
  
  PingusLevelResult r(server->get_world());
  r.draw();
  
  if (verbose) std::cout << "finished " << CL_System::get_time()  << std::endl;
}

void
Client::start(std::string plf_filename, std::string psm_filename)
{
  play_level(plf_filename, psm_filename);
  do_replay = false;
  FadeOut::random();
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
  
  playfield = boost::shared_ptr<Playfield>(new Playfield(plf, server->get_world()));
    
  button_panel = boost::shared_ptr<ButtonPanel>(new ButtonPanel(plf));
  pcounter     = boost::shared_ptr<PingusCounter>(new PingusCounter());
  small_map    = boost::shared_ptr<SmallMap>(new SmallMap());
  time_display = boost::shared_ptr<TimeDisplay>(new TimeDisplay());
  hurry_up     = boost::shared_ptr<HurryUp>(new HurryUp());
  gui_is_init = true;
   
  button_panel->set_server(server);
  time_display->set_server(server);
  button_panel->set_client(this);
  pcounter->set_client(this);
  small_map->set_client(this);
  hurry_up->set_client(this);
  
  /*  if (play_demo)
    server->set_demo(demo_file);
  */
  playfield->set_clip_rect(0, 0, 
			   CL_Display::get_width(),
			   CL_Display::get_height());

  // Connect the button_panel with the playfield
  playfield->set_buttons(button_panel);
  playfield->set_server(server);
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
Client::play_level(std::string plf_filename, std::string psm_filename)
{
  Timer timer;

  timer.start();
  std::cout << "Client::play_level(), Reading PLF..." << std::flush;

  if (plf_filename.substr(plf_filename.size() - 4) == ".xml")
    {
      plf = boost::shared_ptr<PLF>(new XMLPLF(plf_filename));
    }
  else // Assuming we are reading a .plf file
    {
      plf = boost::shared_ptr<PLF>(new PLFPLF(plf_filename));

      std::cout << "done " << timer.stop() << std::endl;

      // FIXME: dirty hack, should replace or merge the psm files
      {
	std::string filename = plf_filename.substr(0, plf_filename.size() - 4);
    
	if (verbose > 1) std::cout << "PSM: " << filename + ".psm" << std::endl;
    
	plf->set_psm_filename(filename + ".psm");
      }
    }
  server->start(plf);

  if (!player)
    server->record_demo();

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

  unsigned int last_update = CL_System::get_time();

  DeltaManager delta_manager;
  float delta;
  // Main Game Loop
  while (!server->is_finished()) 
    {     
      if (!max_cpu_usage)
	{
	  int sleepcount = 0;
	  while (delta_manager.get () < 0.035){
	    CL_System::sleep (10);
	    ++sleepcount;
	  }
	  std::cout << "Sleepcount: " << sleepcount << std::endl;
	}

      delta = delta_manager.getset ();

      CL_System::keep_alive(); 
    
      // Let the server process a game loop
      server->update(delta);

      send_next_event();
      
      // Update every 3/100 seconds
      if (last_update + 30 < CL_System::get_time())
	{
	  last_update = CL_System::get_time();

	  for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
	    obj[i]->updateX();
	}

      // Let the window move its content
      for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
	obj[i]->updateX();

      // Update every 3/100 seconds
      if (last_update + 30 < CL_System::get_time())
	{
	  last_update = CL_System::get_time();

	  for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
	    obj[i]->update(delta);
	}
  
      if (!fast_forward || skip_frame >= 10) 
	{
	  skip_frame = 0;

	  for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
	    {
	      // Update every 3/100 seconds
	      if (last_update + 30 < CL_System::get_time())
		{
		  last_update = CL_System::get_time();

		  for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
		    obj[i]->update(delta);
		}
	      
		obj[i]->draw_clipped();
	    }
      
	  //std::cout << "Flipping display" << std::endl;
	  Display::flip_display();
	}
      else 
	{
	  ++skip_frame;
	}
    }
  unregister_event_handler();
}

void
Client::send_next_event()
{
  if (!player) 
    {
      return;
    }
 else
   {
     while(!player->empty())
       {
	 const PingusEvent& event = player->peek_event();
	 
	 if (event.game_time > GameTime::get_time()) {
	   break;
	 } else if (event.game_time == GameTime::get_time()) {
	   server->send_event(event.str);
	   player->dequeue_event();
	 } else {
	   std::cout << "Client: Demo code currupt..." << std::endl;
	 }
       }
   }
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

  on_button_press_slot = CL_Input::sig_button_press.connect (CL_CreateSlot(this, &Client::on_button_press));
  on_button_release_slot = CL_Input::sig_button_release.connect (CL_CreateSlot(this, &Client::on_button_release));

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
      option_menu.display();
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


/* EOF */


