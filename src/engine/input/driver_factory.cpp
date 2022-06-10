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

#include "engine/input/driver_factory.hpp"

#include "engine/input/core_driver.hpp"
#include "engine/input/sdl_driver.hpp"

namespace pingus::input {

SDLDriverFactory::SDLDriverFactory() :
  m_core_driver(),
  m_sdl_driver()
{
}

SDLDriverFactory::~SDLDriverFactory()
{
}

Driver*
SDLDriverFactory::get(std::string const& name, Manager* manager)
{
  if (name == "sdl")
  {
    if (!m_sdl_driver) {
      m_sdl_driver = std::make_unique<SDLDriver>();
    }

    return m_sdl_driver.get();
  }
  else if (name == "core")
  {
    if (!m_core_driver) {
      m_core_driver = std::make_unique<CoreDriver>(manager);
    }

    return m_core_driver.get();
  }
  else
  {
    return nullptr;
  }
}

void
SDLDriverFactory::update(float delta)
{
  if (m_sdl_driver) {
    m_sdl_driver->update(delta);
  }

  if (m_core_driver) {
    m_core_driver->update(delta);
  }
}

void
SDLDriverFactory::dispatch_event(SDL_Event const& event)
{
  m_sdl_driver->dispatch_event(event);
}

} // namespace pingus::input

/* EOF */
