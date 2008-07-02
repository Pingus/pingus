//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "../globals.hpp"
#include "../debug.hpp"
#include "gui/gui_manager.hpp"
#include "gui_screen.hpp"


GUIScreen::GUIScreen()
  : gui_manager (new GUI::GUIManager())
{
}

GUIScreen::~GUIScreen ()
{
  delete gui_manager;
}

/** Draw this screen */
bool
GUIScreen::draw(DrawingContext& gc)
{
  draw_background(gc);
  gui_manager->draw(gc);
  draw_foreground(gc);
  return true;
}

void
GUIScreen::update(float delta)
{
  gui_manager->update(delta);  
}

void
GUIScreen::update (const Input::Event& event)
{
  // Dispatch the recieved input events
  gui_manager->update(event);

  switch (event.type)
    {
      case Input::POINTER_EVENT_TYPE:
        {
          // ignored cause this is handled in the gui_manager
        }
        break;

      case Input::BUTTON_EVENT_TYPE:
        {
          process_button_event (event.button);
        }
        break;

      case Input::AXIS_EVENT_TYPE:
        {
          if (event.axis.name == Input::ACTION_AXIS)
            {
              on_action_axis_move(event.axis.dir);
            }
        }
        break;

      case Input::SCROLLER_EVENT_TYPE:
        {

        }
        break;
		
      case Input::KEYBOARD_EVENT_TYPE:
        {
		
        }
        break;

      default:
        std::cout << "GUIScreen::update (): unhandled event type: " << event.type << std::endl;
        break;
    }
}

void
GUIScreen::process_button_event (const Input::ButtonEvent& event)
{
  //std::cout << "GUIScreen::process_button_event (Input::ButtonEvent* event)" << std::endl;

  if (event.state == Input::BUTTON_PRESSED)
    {
      switch (event.name)
	{
	case Input::PRIMARY_BUTTON:
	  // ignoring, handled in the gui_manager
	  break;
	case Input::SECONDARY_BUTTON:
	  // ignoring, handled in the gui_manager
	  break;
	case Input::PAUSE_BUTTON:
	  on_pause_press ();
	  break;
	case Input::FAST_FORWARD_BUTTON:
	  on_fast_forward_press ();
	  break;
	case Input::ARMAGEDDON_BUTTON:
	  on_armageddon_press ();
	  break;
	case Input::ESCAPE_BUTTON:
	  on_escape_press ();
	  break;
	default:
	  perr(PINGUS_DEBUG_GUI) << "GUIScreen: ButtonEvent: unhandled event: " << event.name << std::endl;
	  break;
	}
    }
  else if (event.state == Input::BUTTON_RELEASED)
    {
      switch (event.name)
	{
	case Input::PRIMARY_BUTTON:
	  // ignoring, handled in the gui_manager
	  break;
	case Input::SECONDARY_BUTTON:
	  // ignoring, handled in the gui_manager
	  break;
	case Input::PAUSE_BUTTON:
	  on_pause_release ();
	  break;
	case Input::FAST_FORWARD_BUTTON:
	  on_fast_forward_release ();
	  break;
	case Input::ARMAGEDDON_BUTTON:
	  on_armageddon_release ();
	  break;
	case Input::ESCAPE_BUTTON:
	  on_escape_release ();
	  break;
	default:
	  perr(PINGUS_DEBUG_GUI) << "GUIScreen: ButtonEvent: unhandled event: " << event.name << std::endl;
	  break;
	}
    }
  else
    {
      perr(PINGUS_DEBUG_GUI) << "GUIScreen::process_button_event: got unknown event.state: "
			     << event.state << std::endl;;
    }
}


/* EOF */
