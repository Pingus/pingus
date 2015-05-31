// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/input/controller_description.hpp"

#include <stdexcept>

#include "util/raise_exception.hpp"

namespace Input {

ControllerDescription::ControllerDescription() :
  str_to_event(),
  id_to_event()
{
}

ControllerDescription::~ControllerDescription()
{
}

void
ControllerDescription::add_button(const std::string& name, int id)
{
  InputEventDefinition event;

  event.type = BUTTON_EVENT_TYPE;
  event.name = name;
  event.id   = id;

  str_to_event[event.name] = event;
  id_to_event[event.id]    = event;
}

void
ControllerDescription::add_scroller(const std::string& name, int id)
{
  InputEventDefinition event;

  event.type = SCROLLER_EVENT_TYPE;
  event.name = name;
  event.id   = id;

  str_to_event[event.name] = event;
  id_to_event[event.id]    = event;
}

void
ControllerDescription::add_pointer (const std::string& name, int id)
{
  InputEventDefinition event;

  event.type = POINTER_EVENT_TYPE;
  event.name = name;
  event.id   = id;

  str_to_event[event.name] = event;
  id_to_event[event.id]    = event;
}

void
ControllerDescription::add_axis  (const std::string& name, int id)
{
  InputEventDefinition event;

  event.type = AXIS_EVENT_TYPE;
  event.name = name;
  event.id   = id;

  str_to_event[event.name] = event;
  id_to_event[event.id]    = event;
}

void
ControllerDescription::add_keyboard  (const std::string& name, int id)
{
  InputEventDefinition event;

  event.type = KEYBOARD_EVENT_TYPE;
  event.name = name;
  event.id   = id;

  str_to_event[event.name] = event;
  id_to_event[event.id]    = event;
}

const InputEventDefinition&
ControllerDescription::get_definition(int id) const
{
  std::map<int, InputEventDefinition>::const_iterator i = id_to_event.find(id);
  if (i == id_to_event.end())
  {
    raise_exception(std::runtime_error, "Unknown event id");
  }
  return i->second;
}

const InputEventDefinition&
ControllerDescription::get_definition(const std::string& name) const
{
  std::map<std::string, InputEventDefinition>::const_iterator i = str_to_event.find(name);
  if (i == str_to_event.end())
    raise_exception(std::runtime_error, "Unknown event str: " << name);

  return i->second;
}

static std::vector<int> map2vector(const std::map<int, InputEventDefinition>& map,
                                   EventType type)
{
  std::vector<int> lst;
  for(std::map<int, InputEventDefinition>::const_iterator i = map.begin();
      i != map.end(); ++i)
  {
    if (i->second.type == type)
      lst.push_back(i->second.id);
  }
  return lst;
}

std::vector<int>
ControllerDescription::get_buttons() const
{
  return map2vector(id_to_event, BUTTON_EVENT_TYPE);
}

std::vector<int>
ControllerDescription::get_axes() const
{
  return map2vector(id_to_event, AXIS_EVENT_TYPE);
}

std::vector<int>
ControllerDescription::get_pointers() const
{
  return map2vector(id_to_event, POINTER_EVENT_TYPE);
}

std::vector<int>
ControllerDescription::get_scrollers() const
{
  return map2vector(id_to_event, SCROLLER_EVENT_TYPE);
}

std::vector<int>
ControllerDescription::get_keyboards() const
{
  return map2vector(id_to_event, KEYBOARD_EVENT_TYPE);
}

} // namespace Input

/* EOF */
