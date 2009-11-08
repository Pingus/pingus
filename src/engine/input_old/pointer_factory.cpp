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

#include <iostream>

#include "axis_factory.hpp"
#include "pingus_error.hpp"
#include "pointer_factory.hpp"
#include "pointers/axis_pointer.hpp"
#include "pointers/mouse_pointer.hpp"
#include "pointers/multiple_pointer.hpp"

namespace Input {

using namespace Pointers;

Pointer* PointerFactory::create (const FileReader& reader)
{
  if (reader.get_name() == "axis-pointer")
    return axis_pointer(reader);

  else if (reader.get_name() == "mouse-pointer")
    return mouse_pointer();

  else if (reader.get_name() == "multiple-pointer")
    return multiple_pointer(reader);

  else
    throw std::runtime_error(std::string("Unknown pointer type: ") + reader.get_name());

  return 0; // never reached
}

Pointer* PointerFactory::axis_pointer (const FileReader& reader)
{
  float speed;
  if (!reader.read_float("speed", speed))
    throw std::runtime_error("AxisPointer without speed parameter");

  std::vector<Axis*> axes;
  
  const std::vector<FileReader>& sections = reader.get_sections();
  for(std::vector<FileReader>::const_iterator i = sections.begin() + 1;
      i != sections.end(); ++i)
  {
    axes.push_back(AxisFactory::create(*i));
  }

  return new AxisPointer(speed, axes);
}

Pointer* PointerFactory::mouse_pointer ()
{
  return new MousePointer;
}

Pointer* PointerFactory::multiple_pointer (const FileReader& reader)
{
  std::vector<Pointer*> pointers;

  const std::vector<FileReader>& sections = reader.get_sections();
  for(std::vector<FileReader>::const_iterator i = sections.begin();
      i != sections.end(); ++i)
    pointers.push_back(create(*i));
  
  return new MultiplePointer(pointers);
}

} // namespace Input

/* EOF */
