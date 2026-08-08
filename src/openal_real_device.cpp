/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "openal_real_device.hpp"

#include "openal_system.hpp"
#include "sound_error.hpp"

namespace wstsound {

OpenALRealDevice::OpenALRealDevice(OpenALSystem& openal) :
  OpenALDevice(openal)
{
  m_device = alcOpenDevice(nullptr);

  if (!m_device) {
    throw SoundError("Couldn't open audio device.");
  } else {
    m_context = alcCreateContext(m_device, nullptr);
    check_alc_error("Couldn't create audio context: ");

    alcMakeContextCurrent(m_context);
    check_alc_error("Couldn't select audio context: ");

    OpenALSystem::check_al_error("Audio error after init: ");
  }
}

} // namespace wstsound

/* EOF */
