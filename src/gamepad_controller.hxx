//  $Id: gamepad_controller.hxx,v 1.2 2002/06/24 22:52:54 grumbel Exp $
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

#ifndef HEADER_PINGUS_GAMEPAD_CONTROLLER_HXX
#define HEADER_PINGUS_GAMEPAD_CONTROLLER_HXX

#include "delta_manager.hxx"
#include "controller.hxx"

class CL_InputDevice;
class CL_InputAxis;

/** Controller for the Microsoft Sidewinder Gamepad, might work with
    other, but the button layout might be suboptimal. */
class GamepadController : public Controller
{
private:
  CL_InputDevice* device;
  CL_InputAxis* x_axis;
  CL_InputAxis* y_axis;
  CL_Vector pos;
  DeltaManager delta;
  float acceleration;

public:
  GamepadController (CL_InputDevice*, int arg_owner_id = 0);
  ~GamepadController () {}
  
  int get_x ();
  int get_y ();
  
  CL_Vector get_pos ();
  
  void keep_alive ();
};

#endif

/* EOF */
