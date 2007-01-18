//  $Id: axis_pointer.hxx,v 1.5 2003/10/20 19:28:55 grumbel Exp $
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

#ifndef HEADER_PINGUS_INPUT_AXIS_POINTER_HXX
#define HEADER_PINGUS_INPUT_AXIS_POINTER_HXX

#include <vector>
#include "../pointer.hxx"

namespace Input {

class Axis;

namespace Pointers {

/**
   @brief maps two or more axes into a pointer

   XML definition: <axis-pointer> <axis 1><axis 2>[... <axis N>] </axis-pointer>

   The number of axes used to create the pointer and their respective angles is
   unlimited as long as there are at least two axes and the first two axes must have
   different angles.
*/
class AxisPointer : public Pointer {

private:

  const float        speed;
  std::vector<Axis*> axes;

  float              x_pos;
  float              y_pos;

public:

  AxisPointer (float speed, const std::vector<Axis*>& axes_);
  ~AxisPointer ();

  virtual const float& get_x_pos () const;
  virtual const float& get_y_pos () const;

  virtual void  set_pos(float new_x, float new_y);

  virtual void  update (float delta);

private:
  AxisPointer (const AxisPointer&);
  AxisPointer& operator= (const AxisPointer&);
};

} // namespace Pointers
} // namespace Input

#endif

/* EOF */
