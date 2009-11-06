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

#ifndef HEADER_PINGUS_ENGINE_INPUT_OLD_AXES_MULTIPLE_AXIS_HPP
#define HEADER_PINGUS_ENGINE_INPUT_OLD_AXES_MULTIPLE_AXIS_HPP

#include <vector>
#include "../axis.hpp"

namespace Input {
namespace Axes {

/**
   @brief wrapper class, mapping multiple axes to one

   XML definition: <multiple-axis> <axis 1>...<axis n></multiple-axis>
   <br><br>
   angle and pos returned by this class will be the values returned by the first class
   yielding a pos that is not null or of the first axis if none is found
*/
class MultipleAxis : public Axis {

private:
  std::vector<Axis*> axes;
  float              pos;
  float              angle;

public:

  MultipleAxis (const std::vector<Axis*>& axes_);
  ~MultipleAxis ();

  virtual const float& get_pos   () const;
  virtual const float& get_angle () const;

  virtual void  update (float delta);

private:
  MultipleAxis (const MultipleAxis&);
  MultipleAxis& operator= (const MultipleAxis&);
};

} // namespace Axes
} // namespace Input

#endif

/* EOF */
