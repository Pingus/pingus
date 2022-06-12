//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2022 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_APPLICATION_HPP
#define HEADER_PINGUS_PINGUS_APPLICATION_HPP

#include <memory>

#include "fwd.hpp"

namespace pingus {

class Application
{
public:
  Application(CommandLineOptions const& cmd_options);

  void run();

private:
  std::unique_ptr<input::SDLDriverFactory> m_driver_factory;
  std::unique_ptr<input::Manager> m_input_manager;
  input::ControllerPtr m_input_controller;
  std::unique_ptr<ScreenManager> m_screen_manager;

public:
  Application(Application const&) = delete;
  Application& operator=(Application const&) = delete;
};

} // namespace pingus

#endif

/* EOF */
