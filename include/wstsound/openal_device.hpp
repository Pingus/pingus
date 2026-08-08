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

#ifndef HEADER_WINDSTILLE_OPENAL_DEVICE_HPP
#define HEADER_WINDSTILLE_OPENAL_DEVICE_HPP

#include <string>

#include <al.h>
#include <alc.h>

namespace wstsound {

class OpenALSystem;

class OpenALDevice
{
public:
  OpenALDevice(OpenALSystem& openal);
  OpenALDevice(OpenALSystem& openal, ALCdevice* device, ALCcontext* context);
  ~OpenALDevice();

  ALCdevice*  device() { return m_device; }
  ALCcontext* context() { return m_context; }

  void check_alc_error(char const* message);
  void update();

  bool is_extension_present(std::string const& ext) const;
  int max_auxiliary_sends() const;

protected:
  OpenALSystem& m_openal;
  ALCdevice*  m_device;
  ALCcontext* m_context;

private:
  OpenALDevice(const OpenALDevice&) = delete;
  OpenALDevice& operator=(const OpenALDevice&) = delete;
};

} // namespace wstsound

#endif

/* EOF */
