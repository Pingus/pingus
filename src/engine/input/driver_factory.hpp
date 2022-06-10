// Pingus - A free Lemmings clone
// Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_INPUT_DRIVER_FACTORY_HPP
#define HEADER_PINGUS_ENGINE_INPUT_DRIVER_FACTORY_HPP

#include <memory>
#include <string>

#include <SDL.h>

#include "engine/input/fwd.hpp"

namespace pingus::input {

class DriverFactory
{
public:
  virtual ~DriverFactory() {}

  virtual Driver* get(std::string const& name, Manager* manager) = 0;
  virtual void update(float delta) = 0;
};

class SDLDriverFactory : public DriverFactory
{
public:
  SDLDriverFactory();
  ~SDLDriverFactory() override;

  Driver* get(std::string const& name, Manager* manager) override;
  void update(float delta) override;

  void dispatch_event(SDL_Event const& event);

private:
  std::unique_ptr<CoreDriver> m_core_driver;
  std::unique_ptr<SDLDriver> m_sdl_driver;

public:
  SDLDriverFactory(SDLDriverFactory const&) = delete;
  SDLDriverFactory& operator=(SDLDriverFactory const&) = delete;
};

} // namespace pingus::input

#endif

/* EOF */
