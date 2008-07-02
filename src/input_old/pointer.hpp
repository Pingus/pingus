
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

#ifndef HEADER_PINGUS_INPUT_POINTER_HPP
#define HEADER_PINGUS_INPUT_POINTER_HPP

#include "../pingus.hpp"

namespace Input {

/// abstract base class defining the pointer interface
class Pointer
{
public:
  Pointer() { }
  virtual ~Pointer() { }

  /// returns the X coordinate of the pointer
  virtual const float& get_x_pos() const =0;

  /// returns the Y coordinate of the pointer
  virtual const float& get_y_pos() const =0;

  /// sets the pointer to the given position
  virtual void set_pos(float, float) =0;

  virtual void update(float) =0;

private:
  Pointer (const Pointer&);
  Pointer& operator= (const Pointer&);
};

} // namespace Input

#endif

/* EOF */
