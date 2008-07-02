//  $Id: axis.hpp 3582 2008-06-29 16:56:56Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_INPUT_AXIS_HPP
#define HEADER_PINGUS_INPUT_AXIS_HPP

#include "../pingus.hpp"

namespace Input {

/// abstract base class which defines the axis interface
class Axis {

public:
  Axis () { }
  virtual ~Axis () { }

  /// yields the position of the axis in [-1;1]
  virtual const float& get_pos () const =0;

  /// yields the angle of axis in [0;360[ degree
  virtual const float& get_angle () const =0;

  virtual void  update(float) =0;

private:
  Axis (const Axis&);
  Axis& operator= (const Axis&);
};

} // namespace Input

#endif

/* EOF */
