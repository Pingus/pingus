//  $Id: gui_screen.cxx,v 1.2 2002/08/02 13:17:42 grumbel Exp $
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

#include <iostream>
#include "gui/gui_manager.hxx"
#include "input/button_event.hxx"
#include "gui_screen.hxx"

using namespace GUI;

GUIScreen::GUIScreen ()
  : gui_manager (new GUIManager ())
{
}

GUIScreen::~GUIScreen ()
{
  delete gui_manager;
}

  /** Draw this screen */
void
GUIScreen::draw ()
{
  gui_manager->draw ();
}

  /** Pass a delta to the screen */
void
GUIScreen::update (const GameDelta& delta)
{
  gui_manager->update (delta);

  for (std::list<Input::Event*>::const_iterator i = delta.get_events ().begin (); 
       i != delta.get_events ().end (); ++i)
    {
      switch ((*i)->get_type())
	{
	case Input::PointerEventType:
	  {
	    // ignored cause this is handled in the gui_manager
	  }
	  break;

	case Input::ButtonEventType:
	  {
	    process_button_event (dynamic_cast<Input::ButtonEvent*>(*i));
	  }
	  break;

	case Input::AxisEventType:
	  {
	    
	  }
	  break;

	case Input::ScrollEventType:
	  {
	    
	  }
	  break;

	default:
	  std::cout << "GUIScreen::update (): unhandled event type: " << (*i)->get_type() << std::endl;
	  break;
	}
    }
}

void
GUIScreen::process_button_event (Input::ButtonEvent* event)
{
  if (event->state == Input::pressed) // FIXME: We ignore releases for the moment
    {
      switch (event->name)
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
	  std::cout << "ButtonEvent: unhandled event: " << event->name << std::endl;
	  break;
	}
    }
}

/* EOF */
