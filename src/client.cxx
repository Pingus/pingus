//  $Id: client.cxx,v 1.13 2002/08/03 11:37:45 grumbel Exp $
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

#include <string>
#include <config.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Display/mousecursor_provider.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/keyboard.h>
#include "globals.hxx"
#include "playfield.hxx"
#include "timer.hxx"
#include "pingus_resource.hxx"
#include "sound.hxx"
#include "option_menu.hxx"
#include "time_display.hxx"
#include "pingus_counter.hxx"
#include "smallmap.hxx"
#include "plf.hxx"
#include "hurry_up.hxx"
#include "path_manager.hxx"
#include "gamepad_controller.hxx"
#include "cursor.hxx"
#include "server.hxx"
#include "button_panel.hxx"
#include "screen_manager.hxx"

// GUI
#include "gui/surface_button.hxx"

Client::Client(Server * s)
  : server       (s),
    skip_frame   (0),
    do_replay    (false),
    is_finished  (false),
    button_panel (0),
    pcounter     (0),
    playfield    (0),
    time_display (0),
    small_map    (0),
    hurry_up     (0)
  //cursor       (new Cursor ("cursors/animcross", "core", controller))
{
  //Display::add_flip_screen_hook(cursor);
  
  gui_manager->add (new GUI::SurfaceButton 
		    (400, 200,
		     ResDescriptor ("editor/button", "core", ResDescriptor::RD_RESOURCE),
		     ResDescriptor ("editor/button_pressed", "core", ResDescriptor::RD_RESOURCE),
		     ResDescriptor ("editor/actions", "core", ResDescriptor::RD_RESOURCE)));


  Timer timer;
  
  timer.start();
  if (verbose) std::cout << "Client: Generating UI elements..." << std::flush;

  Display::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor", 
						   PingusResource::get("game")));

  // These object will get deleted by the gui_manager
  button_panel = new ButtonPanel(server->get_plf (), 2, CL_Display::get_height()/2);
  playfield    = new Playfield(this, server->get_plf (), server->get_world());
  hurry_up     = new HurryUp();
  pcounter     = new PingusCounter();  
  small_map    = new SmallMap();
  time_display = new TimeDisplay();

  button_panel->set_client(this);
  button_panel->set_server(server);
  hurry_up->set_client(this);
  pcounter->set_client(this);
  small_map->set_client(this);
  time_display->set_server(server);

  gui_manager->add (playfield, true);
  gui_manager->add (button_panel, true);
  gui_manager->add (hurry_up, true);
  gui_manager->add (pcounter, true);
  gui_manager->add (small_map, true);
  gui_manager->add (time_display, true);

  gui_manager->add (new ArmageddonButton(server, CL_Display::get_width() - 40,     CL_Display::get_height() - 62), true);
  gui_manager->add (new ForwardButton   (server, CL_Display::get_width() - 40 * 2, CL_Display::get_height() - 62), true);
  gui_manager->add (new PauseButton     (server, CL_Display::get_width() - 40 * 3, CL_Display::get_height() - 62), true);

  /*playfield->set_clip_rect(0, 0, 
			   CL_Display::get_width(),
			   CL_Display::get_height());*/

  // Connect the button_panel with the playfield
  playfield->set_buttons(button_panel);
  playfield->set_server(server);
  playfield->set_client(this);
  
  if (verbose) std::cout << "done " << timer.stop() << std::endl;
}

Client::~Client()
{
}

void
Client::update (const GameDelta& delta)
{
  GUIScreen::update (delta);
}

void 
Client::update (float delta)
{
  // Let the server process a game loop
  //send_next_event();

  /*float wannabe_delta = 0.0333f;
    unsigned int frames = 1;
    bool auto_frame_skip = false;
    float current_delta;*/

  if (server->is_finished())
    {
      std::cout << "Client: update(): Server is finished" << std::endl;
      ScreenManager::instance ()->pop_screen ();
    }

  /*
    CL_System::keep_alive ();
    CL_System::sleep (0);

    if (current_delta > wannabe_delta)
    {
    update (delta.getset ());
    ++frames;
    if (frames % int(frame_skip) == 0)
    {
    if (!auto_frame_skip)
    {
    draw ();
    }
    }
    else
    {
    CL_System::sleep (int(1000 * (wannabe_delta - delta.get ())));
    }
    }*/
}

#if 0
void
Client::process_events ()
{
  std::list<Input::Event*>& events = input_controller->get_events ();

  for (std::list<Input::Event*>::iterator i = events.begin (); i != events.end (); ++i)
    {
      //std::cout << "Events: " << (*i)->get_type () << std::endl;
    
      switch ((*i)->get_type ())
	{
	case Input::ButtonEventType:
	  process_button_event (static_cast<class Input::ButtonEvent*>(*i));
	  break;

	case Input::PointerEventType:
	  process_pointer_event (static_cast<class Input::PointerEvent*>(*i));
	  break;

	case Input::AxisEventType:
	  process_axis_event (static_cast<class Input::AxisEvent*>(*i));
	  break;
	  
	default:
	  // unhandled event
	  //std::cout << "Client::process_events (): unhandled event: " << (*i)->get_type() << std::endl;
	  break;
	}
    }
}

void
Client::process_button_event (Input::ButtonEvent* event)
{
  std::cout << "Client::process_button_event (): " << event->name << " " << event->state << std::endl;

  switch (event->name)
    {
    case Input::primary:
      {
	if (event->state == Input::pressed)
	  {
	    //GuiObj* obj = get_gui_object (int(event->x), int(event->y));
	    //obj->on_mouse_press ();
	  }
	else
	  {
	    //GuiObj* obj = get_gui_object (int(event->x), int(event->y));
	    //obj->on_mouse_leave ();
	  }
      }
      break;
    case Input::secondary:
      break;
    default:
      break;
    }
}

void
Client::process_pointer_event (Input::PointerEvent* event)
{
  if (event->type == 1)
    std::cout << "Client::process_pointer_event (): " 
	      << event->x << " " << event->y << " " 
	      << event->type << std::endl;  
  
  switch (event->type)
    {
      /*

      case Input::scroll: // FIXME: incorrect enum name
      {
      GuiObj* obj = get_gui_object (int(event->x), int(event->y));
      if (obj)
      {
      if (obj != current_gui_obj)
      {
      current_gui_obj = obj;
      obj->on_mouse_enter();
      }
      else
      {
      if (current_gui_obj) current_gui_obj->on_mouse_leave();
      current_gui_obj = 0;
      }
      }
      else
      {
      if (current_gui_obj) current_gui_obj->on_mouse_leave();
      current_gui_obj = 0;
      }
      }
      break;
      */
      
      //se Input::scroll:
      //reak;	    
      
      //default:
      //std::cout << "Client::process_pointer_event: unhandled event" << std::endl;
      //break;
    }
}

void
Client::process_axis_event (Input::AxisEvent* event)
{
  std::cout << "Client::process_axis_event ()" << std::endl;    
}

void
Client::send_next_event()
{
}
#endif


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

#if 0
void
Client::on_keyboard_button_press(const CL_Key& /*key*/)
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
  std::cout << "Keyboard pressed: " << key.id << std::endl;
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
#endif

void
Client:: on_escape_press ()
{
  server->set_finished();
}

void
Client:: on_pause_press ()
{
  server->set_pause (!server->get_pause ());
}

void 
Client::on_fast_forward_press ()
{
  server->set_fast_forward(!server->get_fast_forward());
}

void 
Client::on_armageddon_press ()
{
  server->send_event("armageddon");
}

void
Client::draw ()
{
  GUIScreen::draw ();
}

void
Client::on_startup ()
{
  std::cout << "Client::on_startup ()" << std::endl;

  /** Hide the system cursor and show the software one */
  Display::show_cursor();
  // FIXME: using this twice will crash with an X Error
  //CL_MouseCursor::hide ();

  do_replay = false;
  is_finished = false;
  skip_frame = 0;

  PingusSound::play_music(path_manager.complete ("music/" + server->get_plf ()->get_music().res_name));

  if (verbose)
    std::cout << "Client: Entering main_loop. Startup time: " 
	      << CL_System::get_time() << " msec." << std::endl;
}

void
Client::on_shutdown ()
{
  std::cout << "Client::on_shutdown ()" << std::endl; 
  Display::hide_cursor();
}

/* EOF */
