//  $Id: client.cxx,v 1.50 2003/10/21 11:01:52 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Display/display.h>
#include "globals.hxx"
#include "playfield.hxx"
#include "timer.hxx"
#include "resource.hxx"
#include "sound/sound.hxx"
#include "time_display.hxx"
#include "pingus_counter.hxx"
#include "smallmap.hxx"
#include "plf.hxx"
#include "hurry_up.hxx"
#include "path_manager.hxx"
#include "gui/cursor.hxx"
#include "true_server.hxx"
#include "button_panel.hxx"
#include "gui/gui_manager.hxx"

namespace Pingus {

Client::Client (TrueServer * s)
  : server       (s),
    skip_frame   (0),
    do_replay    (false),
    is_finished  (false),
    unplayable   (Resource::load_sprite("misc/unplayable2", "core")),
    button_panel (0),
    pcounter     (0),
    playfield    (0),
    time_display (0),
    small_map    (0),
    hurry_up     (0)
{
  Timer timer("Client UI generation");

  // These object will get deleted by the gui_manager
  button_panel = new ButtonPanel(this, 2, CL_Display::get_height()/2);
  playfield    = new Playfield(this);
  hurry_up     = new HurryUp(this);
  pcounter     = new PingusCounter(get_server());
  small_map    = new SmallMap(this);
  time_display = new TimeDisplay(this);

  gui_manager->add (playfield, true);
  gui_manager->add (button_panel, true);
  gui_manager->add (hurry_up, true);
  gui_manager->add (pcounter, true);
  gui_manager->add (small_map, true);
  gui_manager->add (time_display, true);

  gui_manager->add (new ArmageddonButton(server, CL_Display::get_width() - 40,     CL_Display::get_height() - 62), true);
  gui_manager->add (new ForwardButton   (server, CL_Display::get_width() - 40 * 2, CL_Display::get_height() - 62), true);
  gui_manager->add (new PauseButton     (server, CL_Display::get_width() - 40 * 3, CL_Display::get_height() - 62), true);
  // Connect the button_panel with the playfield
  playfield->set_buttons(button_panel);
  playfield->set_server(server);

  timer.stop();
}

Client::~Client()
{
}

void
Client::update (const GameDelta& delta)
{
  GUIScreen::update (delta);
  process_events(delta);
}

void
Client::process_events (const GameDelta& delta)
{
  const Input::EventLst& events = delta.get_events ();

  for (Input::EventLst::const_iterator i = events.begin ();
       i != events.end ();
       ++i)
    {
      //std::cout << "Events: " << (*i)->get_type () << std::endl;

      switch (i->type)
	{
	case Input::ButtonEventType:
          {
            const Input::ButtonEvent& ev = i->button;

            if (ev.state == Input::pressed)
              {
                if (ev.name >= Input::action_1 && ev.name <= Input::action_10)
                  {
                    button_panel->set_button(ev.name - Input::action_1);
                  }
                else if (ev.name == Input::action_down)
                  {
                    button_panel->next_action();
                  }
                else if (ev.name == Input::action_up)
                  {
                    button_panel->previous_action();
                  }
              }
          }
	  break;

	case Input::PointerEventType:
          // Ignore, is handled in GUIScreen
	  //process_pointer_event (dynamic_cast<class Input::PointerEvent* const>(*i));
	  break;

	case Input::AxisEventType:
          // ???
	  process_axis_event (i->axis);
	  break;

        case Input::ScrollEventType:
          process_scroll_event(i->scroll);
          break;

	default:
	  // unhandled event
	  std::cout << "Client::process_events (): unhandled event: " << i->type << std::endl;
	  break;
	}
    }
}

void
Client::process_scroll_event (const Input::ScrollEvent& ev)
{
  playfield->scroll(static_cast<int>(-ev.x_delta),
                    static_cast<int>(-ev.y_delta));
}

void
Client::process_axis_event (const Input::AxisEvent& event)
{
  std::cout << "Client::process_axis_event ()" << std::endl;
  UNUSED_ARG(event);
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
      server->send_armageddon_event();
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
  server->send_armageddon_event();
}

void
Client::on_action_axis_move (float move)
{
  if (move > 0)
    button_panel->next_action ();
  else if (move < 0)
    button_panel->previous_action ();
}

bool
Client::draw (DrawingContext& gc)
{
  GUIScreen::draw (gc);
  if (!server->get_plf()->get_playable())
    gc.draw(unplayable, Vector(400, 50));
  return true;
}

void
Client::on_startup ()
{
  do_replay = false;
  is_finished = false;
  skip_frame = 0;

  if (maintainer_mode)
    std::cout << "Starting Music: " << server->get_plf()->get_music() << std::endl;

  if (server->get_plf()->get_music() == "none")
    {
      Sound::PingusSound::stop_music();
    }
  else
    {
      Sound::PingusSound::play_music(server->get_plf ()->get_music());
    }

  if (verbose)
    std::cout << "Client: Entering main_loop. Startup time: "
	      << CL_System::get_time() << " msec." << std::endl;
}

void
Client::on_shutdown ()
{
}

} // namespace Pingus

/* EOF */
