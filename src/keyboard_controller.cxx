//  $Id: keyboard_controller.cxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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

#include <ClanLib/Display/Input/key.h>
#include <ClanLib/Display/Input/keyboard.h>        
#include "keyboard_controller.hxx"

KeyboardController::KeyboardController (int owner_id)
  : Controller (owner_id)
{
  left   = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_R));
  //middle = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_LALT));
  right  = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_LCTRL));

  scroll_left  = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_A));
  scroll_right = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_D)); 
  scroll_up    = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_W));
  scroll_down  = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_S)); 

  next_action      = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_Q));
  previous_action  = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_E));

  abort  = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_ESCAPE));
  pause  = boost::shared_ptr<ControllerButton>(new KeyboardButton (this, CL_KEY_P));
}

KeyboardController::~KeyboardController ()
{
}

void
KeyboardController::keep_alive ()
{
  if (CL_Keyboard::get_keycode (CL_KEY_I)) { // Up
    pos.y -= 10.0f;
  }
  if (CL_Keyboard::get_keycode (CL_KEY_K)) { // Down
    pos.y += 10.0f;
  }

  if (CL_Keyboard::get_keycode (CL_KEY_J)) { // Left
    pos.x -= 10.0f;
  }

  if (CL_Keyboard::get_keycode (CL_KEY_L)) { // Right
    pos.x += 10.0f;
  }
  
  // Cutting down... FIXME: should be a seperate function
  if (pos.x <= rect.x1)
    pos.x = rect.x1;
  if (pos.y <= rect.y1)
    pos.y = rect.y1;

  if (pos.x >= rect.x2)
    pos.x = rect.x2;
  if (pos.y >= rect.y2)
    pos.y = rect.y2;

  Controller::keep_alive ();
}

/* EOF */

