//  $Id: mouse_button.cxx,v 1.4 2002/08/16 15:14:00 torangan Exp $
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
#include "mouse_button.hxx"
#include "../pingus_error.hxx"

namespace Input {

  MouseButton::MouseButton (int button_) : button(button_)
  {
    if (button > CL_Input::pointers[0]->get_num_buttons())
      PingusError::raise("MouseButton: Invalid button id");
  }

  void
  MouseButton::update (float)
  {
  }
  
  bool
  MouseButton::is_pressed () const
  {
    return CL_Input::pointers[0]->get_button(button)->is_pressed();
  }
  
}

/* EOF */
