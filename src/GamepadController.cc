//  $Id: GamepadController.cc,v 1.5 2001/04/13 13:45:09 grumbel Exp $
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

GamepadController::GamepadController (CL_InputDevice* arg_device, int arg_owner_id)
  : Controller (arg_owner_id),
    device (arg_device),
    pos (CL_Display::get_width ()/2,
	 CL_Display::get_height ()/2),
    acceleration (1.0)
{
  x_axis = device->get_axis (0);
  y_axis = device->get_axis (1);

  if (!x_axis || !y_axis)
    throw PingusError ("Couldn't find enough axis on joystick");
  
  set_range (0, 0, CL_Display::get_width () - 1, CL_Display::get_height () - 1);

  left   = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (4)));
  middle = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (2)));
  right  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (0)));
  scroll_left  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (7)));
  scroll_right  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (6)));
  next_action  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (3)));
  previous_action  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (5)));
  abort  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (8)));
  pause  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (9)));
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
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
}

void 
GamepadController::keep_alive ()
{
  float cdelta = delta.getset ();

  if (device->get_button (1) && device->get_button (1)->is_pressed ())
    pos += CL_Vector(x_axis->get_pos (), y_axis->get_pos ()) * cdelta * 200 * 3.5;
  else
    pos += CL_Vector(x_axis->get_pos (), y_axis->get_pos ()) * cdelta * 200 * 1.0;

  if (pos.x <= x1)
    pos.x = x1;
  if (pos.y <= y1)
    pos.y = y1;

  if (pos.x >= x2)
    pos.x = x2;
  if (pos.y >= y2)
    pos.y = y2;

  Controller::keep_alive();
}

/* EOF */
