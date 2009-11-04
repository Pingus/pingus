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

#include "pingus_error.hpp"
#include "axis_factory.hpp"
#include "button.hpp"
#include "button_factory.hpp"
#include "axes/button_axis.hpp"
#include "axes/inverted_axis.hpp"
#include "axes/joystick_axis.hpp"
#include "axes/multiple_axis.hpp"
#include "debug.hpp"

namespace Input {

using namespace Axes;

Axis* AxisFactory::create(FileReader reader)
{
  if (reader.get_name() == "button-axis")
    return button_axis(reader);

  else if (reader.get_name() == "inverted-axis")
    return inverted_axis(reader);

  else if (reader.get_name() == "joystick-axis")
    return joystick_axis(reader);

  else if (reader.get_name() == "multiple-axis")
    return multiple_axis(reader);

  else
    PingusError::raise(std::string("Unknown axis type: ") + reader.get_name());

  return 0; // never reached
}

Axis* AxisFactory::button_axis(FileReader reader)
{
  float angle;
  if (!reader.read_float("angle", angle))
    PingusError::raise("ButtonAxis without angle parameter");

  const std::vector<FileReader>& sections = reader.get_sections();
  
  if (sections.size() != 3)
    PingusError::raise("ButtonAxis isn't <angle><button><button>");

  Button* button1 = ButtonFactory::create(sections[1]);
  Button* button2 = ButtonFactory::create(sections[2]);

  return new ButtonAxis(angle, button1, button2);
}

Axis* AxisFactory::inverted_axis (FileReader reader)
{
  return new InvertedAxis(create(reader));
}

Axis* AxisFactory::joystick_axis(FileReader reader)
{
  float angle;
  if (!reader.read_float("angle", angle))
    PingusError::raise("JoystickAxis without angle parameter");

  int id;
  if (!reader.read_int("id", id))
    PingusError::raise("JoystickAxis without id parameter");

  int axis;
  if (!reader.read_int("axis", axis))
    PingusError::raise("JoystickAxis without axis parameter");

  return new JoystickAxis(id, axis, angle);
}

Axis* AxisFactory::multiple_axis(FileReader reader)
{
  std::vector<Axis*> axes;

  const std::vector<FileReader>& sections = reader.get_sections();
  for(std::vector<FileReader>::const_iterator i = sections.begin();
      i != sections.end(); ++i)
    axes.push_back(create(*i));

  if (!axes.size())
    PingusError::raise("MultipleAxis without any axis");

  return new MultipleAxis(axes);
}

} // namespace Input

/* EOF */
