// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/gui/gui_manager.hpp"

#include "engine/display/display.hpp"
#include "pingus/globals.hpp"
#include "util/log.hpp"

using namespace Input;

namespace GUI {

GUIManager::GUIManager ()
  : GroupComponent(Rect(0, 0, Display::get_width(), Display::get_height()), false),
    mouse_pos(400,300)
{
}

GUIManager::GUIManager(const Rect& rect_)
  : GroupComponent(rect_),
    mouse_pos(400,300)
{
}

GUIManager::~GUIManager ()
{
}

void
GUIManager::update(float delta)
{
  GroupComponent::update(delta);
}

void
GUIManager::update(const Input::Event& event)
{
  switch (event.type)
  {
    case Input::POINTER_EVENT_TYPE:
      mouse_pos.x = int(event.pointer.x);
      mouse_pos.y = int(event.pointer.y);
      on_pointer_move(mouse_pos.x, mouse_pos.y);
      break;

    case Input::BUTTON_EVENT_TYPE:
      if (event.button.name == PRIMARY_BUTTON)
      {
        if (event.button.state == Input::BUTTON_PRESSED)
          on_primary_button_press(mouse_pos.x, mouse_pos.y);
        else if (event.button.state == Input::BUTTON_RELEASED)
          on_primary_button_release(mouse_pos.x, mouse_pos.y);
      }
      else if (event.button.name == SECONDARY_BUTTON)
      {
        if (event.button.state == Input::BUTTON_PRESSED)
          on_secondary_button_press(mouse_pos.x, mouse_pos.y);
        else if (event.button.state == Input::BUTTON_RELEASED)
          on_secondary_button_release(mouse_pos.x, mouse_pos.y);
      }
      break;

    case Input::AXIS_EVENT_TYPE:
      // AxisEvents can be ignored in the GUI, they are handled elsewhere
      log_debug("GUIManager: AxisEvent: %1%", event.axis.dir);
      break;

    case Input::TEXT_INPUT_EVENT_TYPE:
      on_text_input(event.text);
      break;

    case Input::KEYBOARD_EVENT_TYPE:
      if (event.keyboard.state)
      {
        on_key_pressed(event.keyboard);
      }
      else
      {
        //FIXME: implement this on_key_release(event.keyboard);
      }
      break;

    case Input::SCROLLER_EVENT_TYPE:
      on_scroller_move(event.scroll.x_delta, event.scroll.y_delta);
      break;

    default:
      log_warn("unhandled event type %1%", event.type);
      break;
  }
}

} // namespace GUI

/* EOF */
