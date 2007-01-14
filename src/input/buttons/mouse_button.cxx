//  $Id: mouse_button.cxx,v 1.6 2003/12/13 16:23:40 grumbel Exp $
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

#include <ClanLib/Core/System/clanstring.h>
#include <ClanLib/Display/input_event.h>
#include <ClanLib/Display/keys.h>
#include <ClanLib/Display/mouse.h>
#include "mouse_button.hxx"
#include "../../pingus_error.hxx"

namespace Input {
namespace Buttons {
  
MouseButton::MouseButton (int button_)
  : button(button_),
    button_press_slot  (CL_Mouse::sig_key_down().connect(this, &Input::Buttons::MouseButton::press_handler)),
    button_release_slot(CL_Mouse::sig_key_up().connect(this, &Input::Buttons::MouseButton::release_handler)),
    pressed(false)
{
}

void
MouseButton::update (float)
{
}

void
MouseButton::press_handler (const CL_InputEvent& signal) // may not be a member function since CL_Signal doesn't take a member function pointer
{
  switch (button)
    {
    case 0:
      if (signal.id == CL_MOUSE_LEFT) {
        pressed = true;
        button_down();
      }
      break;
            
    case 1:
      if (signal.id == CL_MOUSE_MIDDLE) {
        pressed = true;
        button_down();
      }
      break;
            
    case 2:
      if (signal.id == CL_MOUSE_RIGHT) {
        pressed = true;
        button_down();
      }
      break;
            
    default: // only three buttons support so far
      break;
    }
}

void
MouseButton::release_handler (const CL_InputEvent& signal) // may not be a member function since CL_Signal doesn't take a member function pointer
{
  switch (button)
    {
    case 0:
      if (signal.id == CL_MOUSE_LEFT) {
        pressed = false;
        button_up();
      }
      break;
            
    case 1:
      if (signal.id == CL_MOUSE_MIDDLE) {
        pressed = false;
        button_up();
      }
      break;
            
    case 2:
      if (signal.id == CL_MOUSE_RIGHT) {
        pressed = false;
        button_up();
      }
      break;
            
    default: // only three buttons support so far
      break;
    }
}
    
} // namespace Buttons
} // namespace Input

/* EOF */
