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

#ifndef HEADER_PINGUS_ENGINE_INPUT_MANAGER_HPP
#define HEADER_PINGUS_ENGINE_INPUT_MANAGER_HPP

#include <memory>

#include "engine/input/controller_description.hpp"
#include "engine/input/driver.hpp"

namespace Input {

class Manager 
{
private:
  typedef std::vector<Driver*> Drivers;
  Drivers drivers;

  std::vector<ControllerPtr> controllers;
  ControllerDescription desc;

public:
  Manager();
  ~Manager();

  /** Resents events for the current controller state */
  void refresh();

  void update(float delta);

  ControllerPtr create_controller(const Pathname& filename);

  Button*   create_button  (const FileReader& reader, Control* parent);
  Axis*     create_axis    (const FileReader& reader, Control* parent);
  Pointer*  create_pointer (const FileReader& reader, Control* parent);
  Scroller* create_scroller(const FileReader& reader, Control* parent);
  Keyboard* create_keyboard(const FileReader& reader, Control* parent);

private:
  Driver* load_driver(const std::string& name);
  Driver* get_driver(const std::string& name);
};

} // namespace Input

#endif

/* EOF */
