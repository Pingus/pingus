//  $Id$
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
#include "globals.hpp"
#include "components/playfield.hpp"
#include "timer.hpp"
#include "resource.hpp"
#include "sound/sound.hpp"
#include "components/time_display.hpp"
#include "components/pingus_counter.hpp"
#include "components/smallmap.hpp"
#include "components/hurry_up.hpp"
#include "display/cursor.hpp"
#include "display/display.hpp"
#include "true_server.hpp"
#include "components/button_panel.hpp"
#include "world.hpp"
#include "math.hpp"
#include "gui/gui_manager.hpp"

Client::Client (TrueServer * s)
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
{
  Timer timer("Client UI generation");

  // These object will get deleted by the gui_manager
  button_panel = new ButtonPanel(this, 2, Display::get_height()/2);

  int world_width  = server->get_world()->get_width();
  int world_height = server->get_world()->get_height();

  playfield    = new Playfield(this, Rect(Vector2i(Math::max((Display::get_width()  - world_width)/2,  0),
                                                   Math::max((Display::get_height() - world_height)/2, 0)), 
                                          Size(Math::min(Display::get_width(),  world_width),
                                               Math::min(Display::get_height(), world_height))));

  hurry_up     = new HurryUp(this);
  pcounter     = new PingusCounter(get_server());
  small_map    = new SmallMap(this);
  time_display = new TimeDisplay(this);

  gui_manager->add(playfield,    true);
  gui_manager->add(button_panel, true);
  gui_manager->add(hurry_up,     true);
  gui_manager->add(pcounter,     true);
  gui_manager->add(small_map,    true);
  gui_manager->add(time_display, true);

#if defined(PINGUS_MODE_NEUTRAL) || defined(PINGUS_MODE_EVIL) || defined(PINGUS_MODE_NICE)
  gui_manager->add(new ArmageddonButton(server, Display::get_width() - 40,     Display::get_height() - 62), true);
#endif

  gui_manager->add(new ForwardButton   (server, Display::get_width() - 40 * 2, Display::get_height() - 62), true);
  gui_manager->add(new PauseButton     (server, Display::get_width() - 40 * 3, Display::get_height() - 62), true);
  // Connect the button_panel with the playfield
  playfield->set_server(server);

  timer.stop();
}

Client::~Client()
{
}

void
Client::draw_background (DrawingContext& gc)
{
  Rect rect = playfield->get_rect();
  if (rect != Rect(Vector2i(0,0), Size(Display::get_width(), Display::get_height())))
  {
    Color border_color(50, 65, 75);
    // top
    gc.draw_fillrect(0, 0, Display::get_width(), rect.top,
                     border_color);
    // bottom
    gc.draw_fillrect(0, rect.bottom, Display::get_width(), Display::get_height(),
                 border_color);
    // left
    gc.draw_fillrect(0, rect.top, rect.left, rect.bottom,
                 border_color);
    // right
    gc.draw_fillrect(rect.right, rect.top, Display::get_width(), rect.bottom,
                 border_color);
  }
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
          case Input::BUTTON_EVENT_TYPE:
          {
            const Input::ButtonEvent& ev = i->button;

            if (ev.state == Input::BUTTON_PRESSED)
              {
                if (ev.name >= Input::ACTION_1_BUTTON && ev.name <= Input::ACTION_10_BUTTON)
                  {
                    button_panel->set_button(ev.name - Input::ACTION_1_BUTTON);
                  }
                else if (ev.name == Input::ACTION_DOWN_BUTTON)
                  {
                    button_panel->next_action();
                  }
                else if (ev.name == Input::ACTION_UP_BUTTON)
                  {
                    button_panel->previous_action();
                  }
              }
          }
	  break;

	case Input::POINTER_EVENT_TYPE:
					// Ignore, is handled in GUIScreen
	  break;

	case Input::AXIS_EVENT_TYPE:
          // ???
	  process_axis_event (i->axis);
	  break;

        case Input::SCROLLER_EVENT_TYPE:
          process_scroll_event(i->scroll);
          break;

        case Input::KEYBOARD_EVENT_TYPE:
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
  // std::cout << "Client::process_axis_event ()" << std::endl;
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

void
Client::on_startup ()
{
  do_replay = false;
  is_finished = false;
  skip_frame = 0;

  if (maintainer_mode)
    std::cout << "Starting Music: " << server->get_plf().get_music() << std::endl;

  if (server->get_plf().get_music() == "none")
    {
      Sound::PingusSound::stop_music();
    }
  else
    {
      Sound::PingusSound::play_music(server->get_plf().get_music());
    }

  if (verbose)
    std::cout << "Client: Entering main_loop. Startup time: "
	      << SDL_GetTicks() << " msec." << std::endl;
}

void
Client::on_shutdown ()
{
}


/* EOF */
