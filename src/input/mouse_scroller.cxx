//  $Id: mouse_scroller.cxx,v 1.2 2002/08/14 12:41:22 torangan Exp $
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
#include <ClanLib/Display/Input/inputcursor.h>
#include "mouse_scroller.hxx"

namespace Input {

  const float&
  MouseScroller::get_x_delta () const
  {
    return x_delta;
  }
  
  const float&
  MouseScroller::get_y_delta () const
  {
    return y_delta;
  }
  
  void
  MouseScroller::get_delta (float& x, float& y) const
  {
    x = x_delta;
    y = y_delta;
  }
  
  void
  MouseScroller::update (float)
  {
    x_delta = CL_Input::pointers[0]->get_cursor(0)->get_x() - old_x;
    y_delta = CL_Input::pointers[0]->get_cursor(0)->get_x() - old_y;
    
    old_x = CL_Input::pointers[0]->get_cursor(0)->get_x();
    old_y = CL_Input::pointers[0]->get_cursor(0)->get_y();
  }
}

/* EOF */

