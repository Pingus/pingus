//  $Id: GamepadController.cc,v 1.2 2001/04/12 20:52:40 grumbel Exp $
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

#include "PingusError.hh"
#include "GamepadController.hh"

GamepadController::GamepadController (int arg_owner_id, CL_InputDevice* arg_device)
  : Controller (arg_owner_id),
    device (arg_device),
    pos (320, 200),
    acceleration (1.0)
{
  x_axis = device->get_axis (0);
  y_axis = device->get_axis (1);

  if (!x_axis || !y_axis)
    throw PingusError ("Couldn't find enough axis on joystick");
}

int
GamepadController::get_x ()
{
  return int(pos.x);
}

int 
GamepadController::get_y ()
{
  return int(pos.y);
}
  
CL_Vector
GamepadController::get_pos ()
{
  return pos;
}

void 
GamepadController::set_range (int x1, int y1, int x2, int y2)
{
  // FIXME: Not implemented
}

bool 
GamepadController::left_pressed ()
{
  CL_InputButton* button = device->get_button (4);
  std::cout << "Button 4: " << button << std::endl;
  if (button)
    return button->is_pressed ();
  else
    return false;
}

bool 
GamepadController::middle_pressed ()
{
  CL_InputButton* button = device->get_button (5);
  if (button)
    return button->is_pressed ();
  else
    return false;
}

bool 
GamepadController::right_pressed ()
{
  CL_InputButton* button = device->get_button (1);
  if (button)
    return button->is_pressed ();
  else
    return false;
}
  
bool 
GamepadController::next_action ()
{
  CL_InputButton* button = device->get_button (7);
  if (button)
    return button->is_pressed ();
  else
    return false;
}

bool 
GamepadController::previous_action ()
{
  CL_InputButton* button = device->get_button (6);
  if (button)
    return button->is_pressed ();
  else
    return false;
}

bool 
GamepadController::scroll_left ()
{
  CL_InputButton* button = device->get_button (7);
  if (button) 
    return button->is_pressed ();
  else
    return false;
}

bool 
GamepadController::scroll_right ()
{
  CL_InputButton* button = device->get_button (6);
    if (button)
    return button->is_pressed ();
  else
    return false;
}

bool 
GamepadController::abort_pressed ()
{
  CL_InputButton* button = device->get_button (8);
  if (button)
    return button->is_pressed ();
  else
    return false;
}

bool 
GamepadController::pause_pressed ()
{
  CL_InputButton* button = device->get_button (7);
  if (button)
    return button->is_pressed ();
  else
    return false;  
}

void 
GamepadController::keep_alive ()
{
  float cdelta = delta.getset ();
  pos += CL_Vector(x_axis->get_pos (), y_axis->get_pos ()) * cdelta * 200 * acceleration;
  Controller::keep_alive();
}

/* EOF */
