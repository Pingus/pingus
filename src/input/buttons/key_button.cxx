//  $Id: key_button.cxx,v 1.4 2003/10/19 12:25:47 grumbel Exp $
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

//#include <ClanLib/Display/input.h>
#include "key_button.hxx"

#include <iostream>

namespace Input {
namespace Buttons {

KeyButton::KeyButton (int button_) 
  : button(button_),
    key_press  (CL_Input::sig_button_press  ().connect(this, &Input::Buttons::KeyButton::key_press_handler)),
    key_release(CL_Input::sig_button_release().connect(this, &Input::Buttons::KeyButton::key_release_handler)),
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
KeyButton::key_press_handler (CL_InputDevice*, const CL_Key& key)
{
  if (key.id == button)
    pressed = true;
}

void
KeyButton::key_release_handler (CL_InputDevice*, const CL_Key& key)
{
  if (key.id == button)
    pressed = false;
}

}
}

/* EOF */
