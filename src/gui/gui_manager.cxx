//  $Id: gui_manager.cxx,v 1.13 2002/09/14 19:06:34 torangan Exp $
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

#include "../debug.hxx"
#include "../globals.hxx"
#include "../input/axis_event.hxx"
#include "../input/button_event.hxx"
#include "../input/pointer_event.hxx"
#include "../game_delta.hxx"

#include "gui_manager.hxx"

using namespace GUI;
using namespace Input;

GUIManager::GUIManager ()
  : primary_pressed_component (0),
    secondary_pressed_component (0),
    mouse_over_component (0),
    x_pos (400), y_pos (300)
{
}

void
GUIManager::draw (GraphicContext& gc)
{
  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      (*i)->draw (gc);
    }
}

void
GUIManager::update (const GameDelta& delta)
{
  process_input (delta.get_events ());

  for (std::vector<Component*>::iterator i = components.begin (); 
       i != components.end (); ++i)
    {
      (*i)->update (delta.get_time ());
    }
}

void
GUIManager::process_input (const std::list<Input::Event*>& events)
{
  for (std::list<Event*>::const_iterator i = events.begin (); i != events.end (); ++i)
    {
      switch ((*i)->get_type())
	{
	case Input::PointerEventType:
	  {
	    PointerEvent* event = dynamic_cast<PointerEvent*>(*i);
	    process_pointer_event (event);
	    break;
	  }
	case Input::ButtonEventType:
	  {
	    ButtonEvent* event = dynamic_cast<ButtonEvent*>(*i);
	    process_button_event (event);
	    break;
	  }
	  
	case Input::AxisEventType:
	  { // AxisEvents can be ignored in the GUI, they are handled elsewhere
	    AxisEvent* event = dynamic_cast<AxisEvent*>(*i);
	    pout (PINGUS_DEBUG_GUI) << "GUIManager: AxisEvent: " << event->dir << std::endl;
	    break;
	  }
	  
	default:
	  pwarn (PINGUS_DEBUG_GUI) << "GUIManager: unhandled event type " << (*i)->get_type() << std::endl;
	  break;
	}
    }
}

void
GUIManager::add (Component* c, bool delete_component) 
{ 
  components.push_back(c); 
  if(delete_component);
}

void
GUIManager::remove (Component* c)
{
  /* components.erase(c); */ 
  if(c);
}

Component*
GUIManager::component_at (int x, int y)
{
  // we travel reversly through the component list, so that we get the
  // top most component at first
  for (std::vector<Component*>::reverse_iterator i = components.rbegin (); 
       i != components.rend (); ++i)
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

void
GUIManager::process_pointer_event (Input::PointerEvent* event)
{
  x_pos = int(event->x);
  y_pos = int(event->y);

  Component* comp = component_at (x_pos, y_pos);//FIXME

  if (primary_pressed_component)
    primary_pressed_component->on_pointer_move (x_pos, y_pos);
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
	    
}

void
GUIManager::process_button_event (Input::ButtonEvent* event)
{
  //std::cout << "GUIManager: Got button event: " << event->name << " " << event->state << std::endl;

  Component* comp = component_at (x_pos, y_pos);//FIXME

  if (comp)
    {
      if (event->name == primary && event->state == Input::pressed)
	{
	  primary_pressed_component = comp;
	  comp->on_primary_button_press (x_pos, y_pos);
	}
      else if (event->name == primary && event->state == Input::released) 
	{
	  /** Send the release event to the same component
	      which got the press event */
	  if (primary_pressed_component)
	    {
	      primary_pressed_component->on_primary_button_release (x_pos, y_pos);

	      if (primary_pressed_component == comp)
		{
		  //std::cout << "GUIManager: calling on_primary_button_click ()" << std::endl;
		  comp->on_primary_button_click (x_pos, y_pos);
		}
	      else
		{
		  // discard click
		}
	      primary_pressed_component = 0;
	    }
	  else
	    {
	      /* FIXME: This happens when you press a button
		 FIXME: in one GUIManager and switch in the
		 FIXME: on_primary_button_press() method to another
		 FIXME: manager, not sure if there is or
		 FIXME: should be a workaround */
	      std::cout << "GUIManager: Got a release without a press, possibly a bug" << std::endl;
	    }
	}
		
      // Secondary button
      if (event->name == secondary && event->state == Input::pressed) 
	{
	  secondary_pressed_component = comp;
	  comp->on_secondary_button_press (x_pos, y_pos);
	}
      else if (event->name == secondary && event->state == Input::released)
	{
	  /** Send the release event to the same component
	      which got the press event */
	  if (secondary_pressed_component)
	    {
	      secondary_pressed_component->on_secondary_button_release (x_pos, y_pos);

	      if (secondary_pressed_component == comp)
		{
		  //std::cout << "GUIManager: calling on_secondary_button_click ()" << std::endl;
		  comp->on_secondary_button_click (x_pos, y_pos);
		}
	      else
		{
		  // discard click
		}
	      secondary_pressed_component = 0;
	    }
	  else
	    {
	      /* FIXME: This happens when you press a button
		 FIXME: in one GUIManager and switch in the
		 FIXME: on_secondary_button_press() method to another
		 FIXME: manager, not sure if there is or
		 FIXME: should be a workaround */
	      std::cout << "GUIManager: Got a release without a press, possibly a bug" << std::endl;
	    }
	}

    }
  else
    {
      if (secondary_pressed_component)
	{
	  secondary_pressed_component->on_secondary_button_release (x_pos, y_pos);
	  secondary_pressed_component = 0;
	}
    }

}

/* EOF */
