/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2005 Matthias Braun <matze@braunis.de>
**                2020 Ingo Ruhnke <grumbel@gmail.com>
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

#include "openal_system.hpp"

#include <sstream>
#include <iostream>

#define AL_ALEXT_PROTOTYPES
#include <alext.h>

#include "openal_buffer.hpp"
#include "openal_device.hpp"
#include "openal_loopback_device.hpp"
#include "openal_real_device.hpp"
#include "sound_error.hpp"
#include "sound_file.hpp"

namespace {

std::vector<std::string> string_split(std::string_view text, char delimiter)
{
  std::vector<std::string> result;

  std::string::size_type start = 0;
  for(std::string::size_type i = 0; i != text.size(); ++i)
  {
    if (text[i] == delimiter)
    {
      result.emplace_back(text.substr(start, i - start));
      start = i + 1;
    }
  }

  result.emplace_back(text.substr(start));

  return result;
}

/** OpenAL string has embedded \0s, it's terminated by a double \0 */
std::vector<std::string> al_string_split(char const* text)
{
  std::vector<std::string> result;

  char const* cur = text;
  while (*cur != '\0') {
    char const* end = cur;
    while(*end++ != '\0');

    result.emplace_back(cur, end - 1);

    cur = end + 1;
  }

  return result;
}

} // namespace

namespace wstsound {

OpenALSystem::OpenALSystem() :
  m_device(),
  m_buffers()
{
}

OpenALSystem::~OpenALSystem()
{
}

OpenALRealDevice&
OpenALSystem::open_real_device()
{
  std::unique_ptr<OpenALRealDevice> real_device = std::make_unique<OpenALRealDevice>(*this);
  OpenALRealDevice& real_device_ref = *real_device;
  m_device = std::move(real_device);
  return real_device_ref;
}

OpenALLoopbackDevice&
OpenALSystem::open_loopback_device(int frequency, int channels)
{
  std::unique_ptr<OpenALLoopbackDevice> loopback_device = std::make_unique<OpenALLoopbackDevice>(*this, frequency, channels);
  OpenALLoopbackDevice& loopback_device_ref = *loopback_device;
  m_device = std::move(loopback_device);
  return loopback_device_ref;
}

OpenALBufferPtr
OpenALSystem::create_buffer(ALenum format,
                            ALvoid const* data,
                            ALsizei size,
                            ALsizei freq)
{
  auto buffer = OpenALBuffer::create();
  alBufferData(buffer->get_handle(), format, data, size, freq);
  OpenALSystem::check_al_error("Couldn't fill audio buffer: ");

  return buffer;
}

void
OpenALSystem::update()
{
  if (m_device) {
    m_device->update();
  }
}

void
OpenALSystem::print_openal_version(std::ostream& out)
{
  if (!m_device) {
    out << "error: print_openal_version() called without OpenAL context\n";
  } else {
    out << "OpenAL Vendor: " << alGetString(AL_VENDOR) << "\n"
        << "OpenAL Version: " << alGetString(AL_VERSION) << "\n"
        << "OpenAL Renderer: " << alGetString(AL_RENDERER) << "\n"
        << "OpenAL Extensions:\n";

    for (auto const& ext : string_split(alGetString(AL_EXTENSIONS), ' ')) {
      out << "  " << ext << '\n';
    }
  }

  if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") == AL_TRUE) {
    out << "OpenAL Devices:\n";

    ALCchar const* default_device_name = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    for (auto const& device : al_string_split(alcGetString(nullptr, ALC_DEVICE_SPECIFIER))) {
      out << "  " << device
          << ((device == default_device_name) ? " (default)" : "")
          << '\n';
    }
  }
}

void
OpenALSystem::check_al_error(const char* message)
{
  int err = alGetError();

  if (err != AL_NO_ERROR)
  {
    std::ostringstream msg;
    msg << message << alGetString(err);
    throw SoundError(msg.str());
  }
}

void
OpenALSystem::warn_al_error(const char* message)
{
  int err = alGetError();

  if (err != AL_NO_ERROR)
  {
    std::cerr << message << alGetString(err) << std::endl;
  }
}

} // namespace wstsound

/* EOF */
