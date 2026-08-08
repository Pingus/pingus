/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_WSTSOUND_OPENAL_LOOPBACK_DEVICE_HPP
#define HEADER_WSTSOUND_OPENAL_LOOPBACK_DEVICE_HPP

#define AL_ALEXT_PROTOTYPES
#include <al.h>
#include <alc.h>
#include <alext.h>

#include "openal_device.hpp"

namespace wstsound {

class OpenALLoopbackDevice : public OpenALDevice
{
public:
  OpenALLoopbackDevice(OpenALSystem& openal, int frequency, int channels);
  ~OpenALLoopbackDevice();

  size_t read(void* buffer, size_t buffer_size);

  int get_frequency() const { return m_frequency; }
  int get_channels() const { return m_channels; }

private:
  int m_frequency;
  int m_channels;

private:
  OpenALLoopbackDevice(const OpenALLoopbackDevice&) = delete;
  OpenALLoopbackDevice& operator=(const OpenALLoopbackDevice&) = delete;
};

} // namespace wstsound

#endif

/* EOF */
