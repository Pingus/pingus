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
#include "display/display.hpp"
#include "screen/game_delta.hpp"
#include "gui_manager.hpp"

using namespace Input;

namespace GUI { 

GUIManager::GUIManager ()
  : GroupComponent(Rect(0, 0, Display::get_width(), Display::get_height()), false),
    mouse_pos(400,300)
{
}

GUIManager::GUIManager(const Rect& rect)
  : GroupComponent(rect),
    mouse_pos(400,300)
{
}

GUIManager::~GUIManager ()
{
}

void
GUIManager::update(const GameDelta& delta)
{
  process_input (delta);
  GroupComponent::update(delta.get_time());
}

void
GUIManager::process_input(const GameDelta& delta)
{
  const std::vector<Input::Event>& events = delta.get_events();

  for (std::vector<Input::Event>::const_iterator i = events.begin (); i != events.end (); ++i)
    {
      switch (i->type)
	{
          case Input::POINTER_EVENT_TYPE:
            mouse_pos.x = int(i->pointer.x);
            mouse_pos.y = int(i->pointer.y);
            on_pointer_move(mouse_pos.x, mouse_pos.y);
            break;

          case Input::BUTTON_EVENT_TYPE:
            if (i->button.name == PRIMARY_BUTTON)
              {
                if (i->button.state == Input::BUTTON_PRESSED)
                  on_primary_button_press(mouse_pos.x, mouse_pos.y);
                else if (i->button.state == Input::BUTTON_RELEASED)
                  on_primary_button_release(mouse_pos.x, mouse_pos.y);
              }
            else if (i->button.name == SECONDARY_BUTTON)
              {
                if (i->button.state == Input::BUTTON_PRESSED)
                  on_secondary_button_press(mouse_pos.x, mouse_pos.y);
                else if (i->button.state == Input::BUTTON_RELEASED)
                  on_secondary_button_release(mouse_pos.x, mouse_pos.y);
              }
            break;

          case Input::AXIS_EVENT_TYPE:
            // AxisEvents can be ignored in the GUI, they are handled elsewhere
            pout(PINGUS_DEBUG_GUI) << "GUIManager: AxisEvent: " << i->axis.dir << std::endl;
            break;
	
          case Input::KEYBOARD_EVENT_TYPE:
            on_key_pressed(i->keyboard.key);
            break;

          case Input::SCROLLER_EVENT_TYPE:
            break;

          default:
            pwarn (PINGUS_DEBUG_GUI) << "GUIManager: unhandled event type " << i->type << std::endl;
            break;
	}
    }
}

} // namespace GUI

/* EOF */
