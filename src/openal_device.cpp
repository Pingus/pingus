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

#include "openal_device.hpp"

#include <assert.h>
#include <sstream>

#include <efx.h>

#include "sound_error.hpp"

namespace wstsound {

OpenALDevice::OpenALDevice(OpenALSystem& openal) :
  m_openal(openal),
  m_device(nullptr),
  m_context(nullptr)
{
}

OpenALDevice::OpenALDevice(OpenALSystem& openal, ALCdevice* device, ALCcontext* context) :
  m_openal(openal),
  m_device(device),
  m_context(context)
{
}

OpenALDevice::~OpenALDevice()
{
  if (m_context) {
    alcDestroyContext(m_context);
  }

  if (m_device) {
    alcCloseDevice(m_device);
  }
}

void
OpenALDevice::check_alc_error(char const* message)
{
  assert(m_device != nullptr);

  int err = alcGetError(m_device);

  if (err != ALC_NO_ERROR) {
    std::ostringstream msg;
    msg << message << alcGetString(m_device, err);
    throw SoundError(msg.str());
  }
}

void
OpenALDevice::update()
{
  alcProcessContext(m_context);
  check_alc_error("Error while processing audio context: ");
}

bool
OpenALDevice::is_extension_present(std::string const& ext) const
{
  return alcIsExtensionPresent(m_device, ext.c_str()) == AL_TRUE;
}

int
OpenALDevice::max_auxiliary_sends() const
{
  ALint sends;
  alcGetIntegerv(m_device, ALC_MAX_AUXILIARY_SENDS, 1, &sends);
  return sends;
}

} // namespace wstsound

/* EOF */
