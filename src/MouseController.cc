//  $Id: MouseController.cc,v 1.2 2001/04/13 09:31:37 grumbel Exp $
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

#include <ClanLib/display.h>
#include "MouseController.hh"

class LeftMouseButton : public ControllerButton
{
public:
  LeftMouseButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return CL_Mouse::left_pressed (); }
};

class MiddleMouseButton : public ControllerButton
{
public:
  MiddleMouseButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return CL_Mouse::middle_pressed (); }
};

class RightMouseButton : public ControllerButton
{
public:
  RightMouseButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return CL_Mouse::right_pressed (); }
};

class AbortMouseButton : public ControllerButton
{
public:
  AbortMouseButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return CL_Keyboard::get_keycode (CL_KEY_ESCAPE); }
};

class PauseMouseButton : public ControllerButton
{
public:
  PauseMouseButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return (CL_Keyboard::get_keycode (CL_KEY_P) | CL_Keyboard::get_keycode (CL_KEY_PAUSE)); }
};

MouseController::MouseController (int arg_owner_id)
  : Controller (arg_owner_id)
{
  left = boost::shared_ptr<ControllerButton> (new LeftMouseButton (this));
  middle = boost::shared_ptr<ControllerButton> (new MiddleMouseButton (this));
  right = boost::shared_ptr<ControllerButton> (new RightMouseButton (this));
  abort = boost::shared_ptr<ControllerButton> (new AbortMouseButton (this));
  pause = boost::shared_ptr<ControllerButton> (new PauseMouseButton (this));
}

MouseController::~MouseController ()
{
}

int 
MouseController::get_x ()
{
  return CL_Mouse::get_x ();
}

int 
MouseController::get_y ()
{
  return CL_Mouse::get_y ();
}
  
CL_Vector
MouseController::get_pos ()
{
  return CL_Vector (CL_Mouse::get_x (), CL_Mouse::get_y ());
}

void 
MouseController::set_range (int x1, int y1, int x2, int y2)
{
  // FIXME: Not implemented
}
  
/* EOF */
