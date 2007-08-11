//  $Id: gui_screen.cxx,v 1.5 2003/10/21 21:37:06 grumbel Exp $
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
#include "gui_screen.hpp"
#include "gui_manager.hpp"


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

  /** Pass a delta to the screen */
void
GUIScreen::update (const GameDelta& delta)
{
  // Dispatch the recieved input events
  gui_manager->update (delta);

  update(delta.get_time ());

  for (Input::EventLst::const_iterator i = delta.get_events ().begin ();
       i != delta.get_events ().end (); ++i)
    {
      switch (i->type)
	{
	case Input::PointerEventType:
	  {
	    // ignored cause this is handled in the gui_manager
	  }
	  break;

	case Input::ButtonEventType:
	  {
	    process_button_event (i->button);
	  }
	  break;

	case Input::AxisEventType:
	  {
	    if (i->axis.name == Input::action)
	      {
		on_action_axis_move (i->axis.dir);
	      }
	  }
	  break;

	case Input::ScrollEventType:
	  {

	  }
	  break;
		
	case Input::KeyboardEventType:
	{
		
	}
	break;

	default:
	  std::cout << "GUIScreen::update (): unhandled event type: " << i->type << std::endl;
	  break;
	}
    }
}

void
GUIScreen::process_button_event (const Input::ButtonEvent& event)
{
  //std::cout << "GUIScreen::process_button_event (Input::ButtonEvent* event)" << std::endl;

  if (event.state == Input::pressed)
    {
      switch (event.name)
	{
	case Input::primary:
	  // ignoring, handled in the gui_manager
	  break;
	case Input::secondary:
	  // ignoring, handled in the gui_manager
	  break;
	case Input::pause:
	  on_pause_press ();
	  break;
	case Input::fast_forward:
	  on_fast_forward_press ();
	  break;
	case Input::armageddon:
	  on_armageddon_press ();
	  break;
	case Input::escape:
	  on_escape_press ();
	  break;
	default:
	  perr(PINGUS_DEBUG_GUI) << "GUIScreen: ButtonEvent: unhandled event: " << event.name << std::endl;
	  break;
	}
    }
  else if (event.state == Input::released)
    {
      switch (event.name)
	{
	case Input::primary:
	  // ignoring, handled in the gui_manager
	  break;
	case Input::secondary:
	  // ignoring, handled in the gui_manager
	  break;
	case Input::pause:
	  on_pause_release ();
	  break;
	case Input::fast_forward:
	  on_fast_forward_release ();
	  break;
	case Input::armageddon:
	  on_armageddon_release ();
	  break;
	case Input::escape:
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
