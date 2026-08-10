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

#include "modplug_sound_file.hpp"

#include <iostream>
#include <vector>

namespace wstsound {

ModplugSoundFile::ModplugSoundFile(std::unique_ptr<std::istream> istream) :
  m_istream(std::move(istream)),
  m_file(nullptr),
  m_bytes_read(0),
  m_format()
{
  // get the file size
  m_istream->seekg(0, std::ios::end);
  size_t file_size = static_cast<size_t>(m_istream->tellg());
  m_istream->seekg(0, std::ios::beg);

  std::vector<char> buffer(file_size);
  m_istream->read(buffer.data(), buffer.size());

  m_file = ModPlug_Load(buffer.data(), static_cast<int>(m_istream->gcount()));

  ModPlug_Settings settings;
  ModPlug_GetSettings(&settings);

  m_format = SoundFormat(settings.mFrequency,
                         settings.mChannels,
                         settings.mBits);
}

ModplugSoundFile::~ModplugSoundFile()
{
  ModPlug_Unload(m_file);
}

size_t
ModplugSoundFile::read(void* buffer, size_t buffer_size)
{
 size_t len = ModPlug_Read(m_file, buffer, static_cast<int>(buffer_size));
 m_bytes_read += len;
 return len;
}

size_t
ModplugSoundFile::tell() const
{
  return m_bytes_read;
}

void
ModplugSoundFile::seek_to_sample(int sample)
{
  m_bytes_read = m_format.sample2bytes(sample);
  long int msec = (1000L * static_cast<long>(sample) / static_cast<long>(m_format.get_rate()));
  ModPlug_Seek(m_file, static_cast<int>(msec));
}

size_t
ModplugSoundFile::get_size() const
{
  long int duration_msec = ModPlug_GetLength(m_file);
  return duration_msec * m_format.get_rate() * m_format.get_channels() * m_format.get_bits_per_sample() / 8  / 1000;
}

} // namespace wstsound

/* EOF */
