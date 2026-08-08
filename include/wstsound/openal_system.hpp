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

#ifndef HEADER_WINDSTILLE_SOUND_OPENAL_SYSTEM_HPP
#define HEADER_WINDSTILLE_SOUND_OPENAL_SYSTEM_HPP

#include <iosfwd>
#include <map>
#include <memory>
#include <vector>

#include <alc.h>
#include <al.h>

#include "fwd.hpp"

namespace wstsound {

class OpenALSystem
{
public:
  /** Check for error and throw SoundError */
  static void check_al_error(const char* message);

  /** Check for error and print a warning */
  static void warn_al_error(const char* message);

public:
  OpenALSystem();
  ~OpenALSystem();

  OpenALRealDevice& open_real_device();
  OpenALLoopbackDevice& open_loopback_device(int frequency = 44100, int channels = 2);

  void print_openal_version(std::ostream& out);
  void check_alc_error(char const* message);

  /** Create an OpenAL buffer, the returned handle is held by
      OpenALSystem and must not be deleted */
  OpenALBufferPtr create_buffer(ALenum format, ALvoid const* data, ALsizei size, ALsizei freq);
  void update();

private:
  std::unique_ptr<OpenALDevice> m_device;
  std::vector<OpenALBuffer> m_buffers;

public:
  OpenALSystem(const OpenALSystem&) = delete;
  OpenALSystem& operator=(const OpenALSystem&) = delete;
};

} // namespace wstsound

#endif

/* EOF */
