//  $Id: root_gui_manager.cxx,v 1.1 2002/07/29 10:44:12 grumbel Exp $
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
#include <list>
#include "../input/event.hxx"
#include "../input/button_event.hxx"
#include "../input/pointer_event.hxx"
#include "root_gui_manager.hxx"

using namespace GUI;
using namespace Input;

RootGUIManager::RootGUIManager (Input::Controller* c)
  : controller(c), pressed_component (0), mouse_over_component (0),
    x_pos (400), y_pos (300)
{  
}

RootGUIManager::~RootGUIManager ()
{
}

void
RootGUIManager::draw ()
{
  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      (*i)->draw ();
    }
}

void
RootGUIManager::update (float delta)
{
  assert (controller);

  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      (*i)->update (delta);
    }

  process_input (delta);
}

void
RootGUIManager::process_input (float delta)
{
  assert (controller);

  std::list<Event*>& events = controller->get_events ();

  for (std::list<Event*>::iterator i = events.begin (); i != events.end (); ++i)
    {
      switch ((*i)->get_type())
	{
	case Input::PointerEventType:
	  {
	    PointerEvent* event = dynamic_cast<PointerEvent*>(*i);
	    x_pos = event->x;
	    y_pos = event->y;

	    Component* comp = component_at (x_pos, y_pos);//FIXME

	    if (comp)
	      {
		if (comp != mouse_over_component)
		  {
		    if (mouse_over_component != 0)
		      mouse_over_component->on_pointer_leave ();
		
		    comp->on_pointer_enter ();
		    mouse_over_component = comp;
		  }
		else
		  {
		    // nothing changed, so we don't trigger events
		  }
	      }
	    else
	      {
		if (mouse_over_component)
		  {
		    mouse_over_component->on_pointer_leave ();
		    mouse_over_component = 0;
		  }
	      }
	    
	    break;
	  }
	case Input::ButtonEventType:
	  {
	    ButtonEvent* event = dynamic_cast<ButtonEvent*>(*i);
	    //std::cout << "RootGUIManager: Got button event: " << event->name << " " << event->state << std::endl;

	    Component* comp = component_at (x_pos, y_pos);//FIXME

	    if (comp)
	      {
		if (event->name == primary && event->state == Input::pressed) 
		  {
		    pressed_component = comp;
		    comp->on_button_press (x_pos, y_pos); // FIXME: dummy data
		  }
		else if (event->name == primary && event->state == Input::released) 
		  {
		    /** Send the release event to the same component
			which got the press event */
		    assert (pressed_component);
		    pressed_component->on_button_release (x_pos, y_pos);

		    if (pressed_component == comp)
		      {
			comp->on_button_click (x_pos, y_pos);
		      }
		    else
		      {
			// discard click
		      }

		    pressed_component = 0;
		  }
	      }
	    else
	      {
		std::cout << "RootGUIManager: Clicked into a non managed region" << std::endl;
		if (pressed_component)
		  {
		    pressed_component->on_button_release (x_pos, y_pos);
		    pressed_component = 0;
		  }
	      }
	    break;
	  }
	default:
	  std::cout << "RootGUIManager: unhandled event type " << (*i)->get_type() << std::endl;
	  break;
	}
    }
}

Component*
RootGUIManager::component_at (int x, int y)
{
  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      if ((*i)->is_at (x, y))
	return *i;
    }
  return 0;
}

/* EOF */
