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

#include "../../pingus_error.hpp"
#include "mouse_button.hpp"

namespace Input {
namespace Buttons {
  
MouseButton::MouseButton(int button_)
  : button(button_),
    pressed(false)
{
  Controller::add_mouse_callback(&MouseButton::mouse_handler, this);
}

void
MouseButton::update (float)
{
}

void
MouseButton::mouse_handler(const SDL_Event& event, void* userdata)
{
  if (event.type != SDL_MOUSEBUTTONDOWN && event.type != SDL_MOUSEBUTTONUP)
    return;

  MouseButton* mb = (MouseButton*)userdata;
  switch (mb->button)
  {
    case 1:
      if (event.button.button == SDL_BUTTON_LEFT) {
        if (event.button.state == SDL_PRESSED) {
          mb->pressed = true;
          mb->button_down();
        } else {
          mb->pressed = false;
          mb->button_up();
        }
      }
      break;
            
    case 2:
      if (event.button.button == SDL_BUTTON_MIDDLE) {
        if (event.button.state == SDL_PRESSED) {
          mb->pressed = true;
          mb->button_down();
        } else {
          mb->pressed = false;
          mb->button_up();
        }
      }
      break;
            
    case 3:
      if (event.button.button == SDL_BUTTON_RIGHT) {
        if (event.button.state == SDL_PRESSED) {
          mb->pressed = true;
          mb->button_down();
        } else {
          mb->pressed = false;
          mb->button_up();
        }
      }
      break;
            
    default: // only three buttons support so far
      break;
  }
}

} // namespace Buttons
} // namespace Input

/* EOF */
