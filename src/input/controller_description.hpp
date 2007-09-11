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

#ifndef HEADER_INPUT_CONTROLLER_DESCRIPTION_HPP
#define HEADER_INPUT_CONTROLLER_DESCRIPTION_HPP

#include <map>
#include <string>
#include "event.hpp"

namespace Input {

struct InputEventDefinition 
{
  EventType   type;
  int         id;
  std::string name;
};

class ControllerDescription
{
private:
  std::map<std::string, InputEventDefinition> str_to_event;
  std::map<int,         InputEventDefinition> id_to_event;

public:
  ControllerDescription();
  ~ControllerDescription();

  void add_button  (const std::string& name, int id);
  void add_axis    (const std::string& name, int id); 
  void add_scroller(const std::string& name, int id); 
  void add_pointer (const std::string& name, int id); 
  void add_keyboard(const std::string& name, int id); 

  const InputEventDefinition& get_definition(int id) const;
  const InputEventDefinition& get_definition(const std::string& name) const;

  std::vector<int> get_buttons() const;
  std::vector<int> get_axes() const;
  std::vector<int> get_pointers() const;
  std::vector<int> get_scrollers() const;
  std::vector<int> get_keyboards() const;
};

} // namespace Input

#endif

/* EOF */
