//  $Id: GamepadController.hh,v 1.1 2001/04/12 19:51:41 grumbel Exp $
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

#ifndef GAMEPADCONTROLLER_HH
#define GAMEPADCONTROLLER_HH

#include <ClanLib/display.h>
#include "DeltaManager.hh"
#include "Controller.hh"

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

public:
  GamepadController (int arg_owner_id, CL_InputDevice*);
  ~GamepadController () {}
  
  int get_x ();
  int get_y ();
  
  CL_Vector get_pos ();

  /** Limit the range to the given area, might be used in multiplayer
      modes to limit the cursor to the players view */
  void set_range (int x1, int y1, int x2, int y2) =0;
  
  /// @return true when the left mouse button is pressed
  bool left_pressed ();
  /// @return true when the middle mouse button is pressed
  bool middle_pressed ();
  /// @return true when the right mouse button is pressed
  bool right_pressed ();
  
  bool next_action ();
  bool previous_action ();
  
  /** @return true if the abort button was pressed (usually the escape
      key) */
  bool abort_pressed ();

  bool pause_pressed ();
  
  void keep_alive ();
};

#endif

/* EOF */
