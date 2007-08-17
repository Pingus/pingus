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

#include <algorithm>
#include "../debug.hpp"
#include "../globals.hpp"
#include "../input/event.hpp"
#include "game_delta.hpp"
#include "gui_manager.hpp"


using namespace GUI;
using namespace Input;

GUIManager::GUIManager ()
  : primary_pressed_component (0),
    secondary_pressed_component (0),
		focussed_component(0),
    mouse_over_component (0),
    x_pos (400), y_pos (300)
{
}

GUIManager::~GUIManager ()
{
  for (std::vector<Component*>::iterator i = components_for_delete.begin ();
       i != components_for_delete.end (); ++i)
    {
      delete (*i);
    }

  components_for_delete.clear();
}

void
GUIManager::draw (DrawingContext& gc)
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
  process_input (delta);

  for (std::vector<Component*>::iterator i = components.begin ();
       i != components.end (); ++i)
    {
      (*i)->update (delta.get_time ());
    }
}

void
GUIManager::process_input (const GameDelta& delta)
{
  const std::vector<Input::Event>& events = delta.get_events();

  for (std::vector<Input::Event>::const_iterator i = events.begin (); i != events.end (); ++i)
    {
      switch (i->type)
	{
	case Input::PointerEventType:
          process_pointer_event (i->pointer);
          break;

	case Input::ButtonEventType:
          process_button_event (delta.get_absolute_time(), i->button);
          break;

	case Input::AxisEventType:
          // AxisEvents can be ignored in the GUI, they are handled elsewhere
          pout (PINGUS_DEBUG_GUI) << "GUIManager: AxisEvent: " << i->axis.dir << std::endl;
          break;
	
	case Input::KeyboardEventType:
          process_keyboard_event(i->keyboard);
          break;

	default:
	  pwarn (PINGUS_DEBUG_GUI) << "GUIManager: unhandled event type " << i->type << std::endl;
	  break;
	}
    }
}

void
GUIManager::add(Component* c, bool delete_component)
{
  components.push_back(c);
  if( delete_component )
    components_for_delete.push_back(c);
}

void
GUIManager::remove(Component* c)
{
  Components::iterator i = std::find(components.begin(), components.end(), c);
  if (i != components.end())
    components.erase(i); 
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
GUIManager::process_pointer_event (const Input::PointerEvent& event)
{
  x_pos = static_cast<int>(event.x);
  y_pos = static_cast<int>(event.y);

  Component* comp = component_at(x_pos, y_pos);//FIXME

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
GUIManager::process_button_event (unsigned int time_stamp, const Input::ButtonEvent& event)
{
  //std::cout << "GUIManager: Got button event: " << event.name << " " << event.state << std::endl;

  Component* comp = component_at (x_pos, y_pos);//FIXME: x/y_pos should be inside controller

  if (comp)
    {
      if (event.name == primary && event.state == Input::pressed)
	{
	  primary_pressed_component = comp;
		change_focussed_comp(comp);
	  comp->on_primary_button_press (x_pos, y_pos);

          // FIXME: add double click detection here
	}
      else if (event.name == primary && event.state == Input::released)
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
	      if (maintainer_mode)
                std::cout << "GUIManager: Got a release without a press, possibly a bug" << std::endl;
	    }
	}

      // Secondary button
      if (event.name == secondary && event.state == Input::pressed)
	{
	  secondary_pressed_component = comp;
		change_focussed_comp(comp);
	  comp->on_secondary_button_press (x_pos, y_pos);
	}
      else if (event.name == secondary && event.state == Input::released)
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
              if (maintainer_mode)
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

  UNUSED_ARG(time_stamp);
}

void 
GUIManager::process_keyboard_event (const Input::KeyboardEvent &event)
{
	// Pass key value to last pressed component.
	if (focussed_component)
		focussed_component->on_key_pressed(event.key);
}

void
GUIManager::change_focussed_comp(Component* c)
{
	if (focussed_component)
		focussed_component->set_focus(false);
	focussed_component = c;
	if (focussed_component)
		focussed_component->set_focus(true);
}


/* EOF */
