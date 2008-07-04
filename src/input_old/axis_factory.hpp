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

#ifndef HEADER_PINGUS_INPUT_AXIS_FACTORY_HPP
#define HEADER_PINGUS_INPUT_AXIS_FACTORY_HPP

#include "../file_reader.hpp"

namespace Input {

class Axis;

class AxisFactory
{
private:
  static Axis* button_axis(FileReader reader);
  static Axis* inverted_axis(FileReader reader);
  static Axis* joystick_axis(FileReader reader);
  static Axis* multiple_axis(FileReader reader);

public:
  static Axis* create(FileReader reader);

private:
  AxisFactory();
  AxisFactory(const AxisFactory&);
  AxisFactory& operator= (const AxisFactory&);
};

} // namespace Input

#endif

/* EOF */
