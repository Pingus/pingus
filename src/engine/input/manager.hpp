// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_INPUT_MANAGER_HPP
#define HEADER_PINGUS_ENGINE_INPUT_MANAGER_HPP

#include <filesystem>
#include <memory>

#include "engine/input/controller_description.hpp"
#include "engine/input/driver.hpp"

namespace pingus::input {

class Manager
{
public:
  Manager();
  ~Manager();

  /** Resents events for the current controller state */
  void refresh();

  void update(float delta);

  ControllerPtr create_controller(std::filesystem::path const& filename);

  std::unique_ptr<Button> create_button(prio::ReaderObject const& reader, Control* parent);
  std::unique_ptr<Axis> create_axis(prio::ReaderObject const& reader, Control* parent);
  std::unique_ptr<Pointer> create_pointer(prio::ReaderObject const& reader, Control* parent);
  std::unique_ptr<Scroller> create_scroller(prio::ReaderObject const& reader, Control* parent);
  std::unique_ptr<Keyboard> create_keyboard(prio::ReaderObject const& reader, Control* parent);

private:
  std::vector<std::unique_ptr<Driver> > m_drivers;
  std::vector<ControllerPtr> m_controllers;
  ControllerDescription m_desc;

private:
  Driver* load_driver(std::string const& name);
  Driver* get_driver(std::string const& name);
};

} // namespace pingus::input

#endif

/* EOF */
