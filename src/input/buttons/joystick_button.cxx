//  $Id: joystick_button.cxx,v 1.2 2003/04/15 15:12:15 grumbel Exp $
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

#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/inputdevice.h>
#include <ClanLib/Display/Input/inputbutton.h>
#include "joystick_button.hxx"
#include "../../string_converter.hxx"
#include "../../pingus_error.hxx"

namespace Input {

  namespace Buttons {
  
    JoystickButton::JoystickButton(int id_, int button_) : id(id_), button(button_)
    {
      if (static_cast<unsigned int>(id) >= CL_Input::joysticks.size())
        PingusError::raise("JoystickButton: Invalid joystick id: " + to_string(id));
      
      if (button > CL_Input::joysticks[id]->get_num_buttons())
        PingusError::raise("JoystickButton: Invalid joystick button id: " + to_string(button));
    }

    void
    JoystickButton::update(float)
    {
    }
  
    bool
    JoystickButton::is_pressed() const
    {
      return CL_Input::joysticks[id]->get_button(button)->is_pressed();
    }
  
  }
}

/* EOF */
