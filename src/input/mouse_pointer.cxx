//  $Id: mouse_pointer.cxx,v 1.4 2002/08/23 15:49:56 torangan Exp $
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
#include "mouse_pointer.hxx"

namespace Input
{
  MousePointer::MousePointer () : x_pos(0), y_pos(0)
  {
  }

  const float&
  MousePointer::get_x_pos () const
  {
    return x_pos;
  }

  const float&
  MousePointer::get_y_pos () const
  {
    return y_pos;
  }

  void
  MousePointer::set_pos (float new_x, float new_y)
  {
    CL_Input::pointers[0]->get_cursor(0)->set_position(new_x, new_y);
  }

  void
  MousePointer::update (float)
  {
    x_pos = CL_Input::pointers[0]->get_cursor(0)->get_x();
    y_pos = CL_Input::pointers[0]->get_cursor(0)->get_y();
  }

}

/* EOF */
