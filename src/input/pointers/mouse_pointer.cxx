//  $Id: mouse_pointer.cxx,v 1.4 2003/10/19 12:25:47 grumbel Exp $
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

#include <ClanLib/Display/mouse.h>
#include "mouse_pointer.hxx"

namespace Input {
namespace Pointers {

MousePointer::MousePointer () 
  : x_pos(0),
    y_pos(0)
#ifdef CLANLIB_0_6
,
    move_slot(CL_Mouse::sig_move().connect(this, &Input::Pointers::MousePointer::move_signal))
#endif
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
#ifdef CLANLIB_0_6
  CL_Mouse::set_position(new_x, new_y);
#endif
}

void
MousePointer::update (float)
{
}

void
MousePointer::move_signal (int x, int y)
{
  x_pos = x;
  y_pos = y;
}

}
}

/* EOF */
