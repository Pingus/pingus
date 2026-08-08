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

#ifndef HEADER_WSTSOUND_MODPLUG_SOUND_FILE_HPP
#define HEADER_WSTSOUND_MODPLUG_SOUND_FILE_HPP

#include "sound_file.hpp"

#include <libmodplug/modplug.h>

namespace wstsound {

class ModplugSoundFile : public SoundFile
{
private:
public:
  ModplugSoundFile(std::unique_ptr<std::istream> istream);
  ~ModplugSoundFile() override;

  size_t read(void* buffer, size_t buffer_size) override;
  size_t tell() const override;
  void   seek_to_sample(int sample) override;
  SoundFormat get_format() const override { return m_format; }
  size_t get_size() const override;

private:
  std::unique_ptr<std::istream> m_istream;
  ModPlugFile* m_file;
  size_t m_bytes_read;
  SoundFormat m_format;

private:
  ModplugSoundFile(const ModplugSoundFile&) = delete;
  ModplugSoundFile& operator=(const ModplugSoundFile&) = delete;
};

} // namespace wstsound

#endif

/* EOF */
