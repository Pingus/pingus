//  $Id: key_button.cxx,v 1.5 2003/10/20 13:33:44 grumbel Exp $
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

#include <ClanLib/Display/keyboard.h>
#include <ClanLib/Display/input_event.h>
#include "key_button.hpp"

#include <iostream>

namespace Input {
namespace Buttons {

KeyButton::KeyButton (int button_) 
  : button(button_),
    key_press  (CL_Keyboard::sig_key_down().connect(this, &Input::Buttons::KeyButton::key_press_handler)),
    key_release(CL_Keyboard::sig_key_up().connect(this, &Input::Buttons::KeyButton::key_release_handler)),
    pressed(false)
{
}

void
KeyButton::update (float)
{
}

bool
KeyButton::is_pressed () const
{
  return pressed;
}

void
KeyButton::key_press_handler (const CL_InputEvent& event)
{
  if (event.id == button)
    {
      pressed = true;
      button_down();
    }
}

void
KeyButton::key_release_handler (const CL_InputEvent& event)
{
  if (event.id == button)
    {
      pressed = false;
      button_up();
    }
}

} // namespace Buttons
} // namespace Input

/* EOF */
