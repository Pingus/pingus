//  $Id: GamepadController.cc,v 1.11 2001/08/18 09:54:26 grumbel Exp $
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
  if (!device)
    throw PingusError ("GamepadController: Couldn't find a gamepad controlling device!");
  
  x_axis = device->get_axis (0);
  y_axis = device->get_axis (1);

  if (!x_axis || !y_axis)
    throw PingusError ("Couldn't find enough axes on joystick");
  
  set_range (CL_Rect(0, 0, CL_Display::get_width () - 1, CL_Display::get_height () - 1));

  try {
    left   = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (4)));
    middle = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (2)));
    right  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (0)));
    scroll_left  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (6)));
    scroll_right  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (7)));
    next_action  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (3)));
    previous_action  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (5)));
    abort  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (8)));
    pause  = boost::shared_ptr<ControllerButton>(new InputDeviceButton(this, device->get_button (9)));
  } catch (PingusError& e) {
    std::cout << "PingusError: " << e.get_message () << std::endl;
  }
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
GamepadController::keep_alive ()
{
  float cdelta = delta.getset ();

  if (device->get_button (1) && device->get_button (1)->is_pressed ())
    pos += CL_Vector(x_axis->get_pos (), y_axis->get_pos ()) * cdelta * 200 * 3.5;
  else
    pos += CL_Vector(x_axis->get_pos (), y_axis->get_pos ()) * cdelta * 200 * 1.0;

  //std::cout << "x_pos: " << pos.x << " y_pos: " <<  pos.y << std::endl;
  //std::cout << "Rect: " << rect.x1 << " " << rect.y1 << " " << rect.x2 << " " << rect.y2 << std::endl;

  if (pos.x <= rect.x1)
    pos.x = rect.x1;
  if (pos.y <= rect.y1)
    pos.y = rect.y1;

  if (pos.x >= rect.x2)
    pos.x = rect.x2;
  if (pos.y >= rect.y2)
    pos.y = rect.y2;

  Controller::keep_alive();
}

/* EOF */
