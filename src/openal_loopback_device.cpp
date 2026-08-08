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

#include "openal_loopback_device.hpp"

#include <stdexcept>

#include "openal_system.hpp"
#include "sound_error.hpp"

namespace wstsound {

OpenALLoopbackDevice::OpenALLoopbackDevice(OpenALSystem& openal, int frequency, int channels) :
  OpenALDevice(openal),
  m_frequency(frequency),
  m_channels(channels)
{
  m_device = alcLoopbackOpenDeviceSOFT(nullptr);
  if (!m_device) {
    throw SoundError("Couldn't open audio loopback device.");
  }

  ALCenum channels_al = (m_channels == 2) ? ALC_STEREO_SOFT : ALC_MONO_SOFT;
  ALCenum format_al = ALC_SHORT_SOFT;

  if (alcIsRenderFormatSupportedSOFT(m_device, m_frequency, channels_al, format_al) != AL_TRUE) {
    alcCloseDevice(m_device);
    throw SoundError("OpenALLoopbackDevice: format not supported");
  }

  std::vector<ALCint> attrs = {
    ALC_FREQUENCY, m_frequency,
    ALC_FORMAT_CHANNELS_SOFT, channels_al,
    ALC_FORMAT_TYPE_SOFT, ALC_SHORT_SOFT,
    0
  };

  m_context = alcCreateContext(m_device, attrs.data());
  check_alc_error("Couldn't create audio context: ");

  alcMakeContextCurrent(m_context);
  check_alc_error("Couldn't select audio context: ");
}

OpenALLoopbackDevice::~OpenALLoopbackDevice()
{
  alcCloseDevice(m_device);
}

size_t
OpenALLoopbackDevice::read(void* buffer, size_t buffer_size)
{
  int samples = static_cast<int>(buffer_size) / m_channels / 2;
  alcRenderSamplesSOFT(m_device, buffer, samples);
  return samples * m_channels * 2;
}

} // namespace wstsound

/* EOF */
