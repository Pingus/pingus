//  $Id: gui_manager.cxx,v 1.3 2002/07/30 01:58:16 grumbel Exp $
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
#include <vector>
#include <list>
#include "../input/event.hxx"
#include "../input/button_event.hxx"
#include "../input/pointer_event.hxx"

#include "gui_manager.hxx"

using namespace GUI;
using namespace Input;

GUIManager::GUIManager ()
  : pressed_component (0), mouse_over_component (0),
    x_pos (400), y_pos (300)
{
}

void
GUIManager::draw ()
{
  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      (*i)->draw ();
    }
}

void
GUIManager::update (float delta)
{
  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      (*i)->update (delta);
    }
}

void
GUIManager::process_input (std::list<Event*>& events)
{
  for (std::list<Event*>::iterator i = events.begin (); i != events.end (); ++i)
    {
      switch ((*i)->get_type())
	{
	case Input::PointerEventType:
	  {
	    PointerEvent* event = dynamic_cast<PointerEvent*>(*i);
	    x_pos = int(event->x);
	    y_pos = int(event->y);

	    Component* comp = component_at (x_pos, y_pos);//FIXME

	    if (pressed_component)
	      pressed_component->on_pointer_move (x_pos, y_pos);
	    else if (comp)
	      {
		comp->on_pointer_move (x_pos, y_pos);
	      }

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

void
GUIManager::add (Component* c, bool delete_component) 
{ 
  components.push_back(c); 
}

void
GUIManager::remove (Component* c)
{
  /* components.erase(c); */ 
}

Component*
GUIManager::component_at (int x, int y)
{
  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      if ((*i)->is_at (x, y))
	return *i;
    }
  return 0;
}

bool
GUIManager::is_at (int x, int y)
{
  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      if ((*i)->is_at (x, y))
	return true;
    }

  return false;
}

/* EOF */
