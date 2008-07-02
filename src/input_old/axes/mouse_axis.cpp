
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

#include "mouse_axis.hpp"

namespace Input {

  namespace Axes {

    MouseAxis::MouseAxis(int axis_, float angle_) : axis(axis_), angle(angle_), pos(0), old_pos(0)
    {
      if (angle < 0)
        angle = (float)((static_cast<int>(angle) % 360) + 360);
      else if (angle > 360)
        angle = (float)(static_cast<int>(angle) % 360);

#if 0
      switch (axis)
        {
          case 0:  old_pos = CL_Input::pointers[0]->get_cursor(0)->get_x();
	           break;
          case 1:  old_pos = CL_Input::pointers[0]->get_cursor(0)->get_y();
	           break;
	  default: old_pos = 0;
        }
#endif
    }

    const float&
    MouseAxis::get_pos () const
    {
      return pos;
    }

    const float&
    MouseAxis::get_angle () const
    {
      return angle;
    }

    void
    MouseAxis::update (float)
    {
#if 0
      switch (axis)
        {
          case 0:  if (old_pos != CL_Input::pointers[0]->get_cursor(0)->get_x())
	             {
		       pos     = CL_Input::pointers[0]->get_cursor(0)->get_x() - old_pos;
		       old_pos = CL_Input::pointers[0]->get_cursor(0)->get_x();

		       if (pos < -1)
		         pos = -1;
		       else if (pos > 1)
		         pos = 1;
 		     }
	           break;

          case 1:  if (old_pos != CL_Input::pointers[0]->get_cursor(0)->get_y())
	             {
		       pos     = CL_Input::pointers[0]->get_cursor(0)->get_y() - old_pos;
		       old_pos = CL_Input::pointers[0]->get_cursor(0)->get_y();

		       if (pos < -1)
		         pos = -1;
		       else if (pos > 1)
		         pos = 1;
		     }
	           break;
          default: break; // do nothing
        }
#endif
    }

  }
}

/* EOF */
