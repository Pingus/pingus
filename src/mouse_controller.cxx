//  $Id: mouse_controller.cxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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

#include <ClanLib/Display/Input/mouse.h>
#include <ClanLib/Display/Input/keyboard.h>
#include "mouse_controller.hxx"

/* All this should be rewritten to have a MultiplexButton, a KeyboardButton, etc. */

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
  bool is_pressed () { 
    return (CL_Keyboard::get_keycode (CL_KEY_P) || CL_Keyboard::get_keycode (CL_KEY_PAUSE)); 
  }
};

class FastForwardMouseButton : public ControllerButton
{
public:
  FastForwardMouseButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { 
    return (CL_Keyboard::get_keycode (CL_KEY_SPACE) || CL_Keyboard::get_keycode (CL_KEY_F)); 
  }
};

class ScrollLeftButton : public ControllerButton
{
public:
  ScrollLeftButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return (CL_Keyboard::get_keycode (CL_KEY_LEFT)); }
};

class ScrollRightButton : public ControllerButton
{
public:
  ScrollRightButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return (CL_Keyboard::get_keycode (CL_KEY_RIGHT)); }
};

class ScrollUpButton : public ControllerButton
{
public:
  ScrollUpButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return (CL_Keyboard::get_keycode (CL_KEY_UP)); }
};

class ScrollDownButton : public ControllerButton
{
public:
  ScrollDownButton (Controller* c) : ControllerButton (c) {};
  bool is_pressed () { return (CL_Keyboard::get_keycode (CL_KEY_DOWN)); }
};


MouseController::MouseController (int arg_owner_id)
  : Controller (arg_owner_id)
{
  left         = boost::shared_ptr<ControllerButton> (new LeftMouseButton (this));
  middle       = boost::shared_ptr<ControllerButton> (new MiddleMouseButton (this));
  right        = boost::shared_ptr<ControllerButton> (new RightMouseButton (this));
  abort        = boost::shared_ptr<ControllerButton> (new AbortMouseButton (this));
  pause        = boost::shared_ptr<ControllerButton> (new PauseMouseButton (this));
  fast_forward = boost::shared_ptr<ControllerButton> (new FastForwardMouseButton (this));
  scroll_left  = boost::shared_ptr<ControllerButton> (new ScrollLeftButton (this));
  scroll_right = boost::shared_ptr<ControllerButton> (new ScrollRightButton(this));
  scroll_up    = boost::shared_ptr<ControllerButton> (new ScrollUpButton (this));
  scroll_down  = boost::shared_ptr<ControllerButton> (new ScrollDownButton(this));
}

MouseController::~MouseController ()
{
}

int 
MouseController::get_x ()
{
  return int(pos.x);
}

int 
MouseController::get_y ()
{
  return int(pos.y);
}
  
CL_Vector
MouseController::get_pos ()
{
  return pos;
}

void 
MouseController::keep_alive ()
{
  pos.x = CL_Mouse::get_x ();
  pos.y = CL_Mouse::get_y ();

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

