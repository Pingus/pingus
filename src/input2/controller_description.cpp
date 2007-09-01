/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <stdexcept>
#include "controller_description.hpp"

namespace Input {


ControllerDescription::ControllerDescription()
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

const InputEventDefinition&
ControllerDescription::get_definition(int id) const
{
  std::map<int, InputEventDefinition>::const_iterator i = id_to_event.find(id);
  if (i == id_to_event.end())
    throw std::runtime_error("Unknown event id");

  return i->second;
}

const InputEventDefinition&
ControllerDescription::get_definition(const std::string& name) const
{
  std::map<std::string, InputEventDefinition>::const_iterator i = str_to_event.find(name);
  if (i == str_to_event.end())
    throw std::runtime_error("Unknown event str: " + name);

  return i->second;
}


} // namespace Input

/* EOF */
