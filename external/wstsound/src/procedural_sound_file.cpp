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

#include "procedural_sound_file.hpp"

#include <iostream>
#include <limits>
#include <math.h>

namespace {

template<typename T>
T map_to(float value)
{
  if (value >= 0) {
    return static_cast<T>(static_cast<float>(std::numeric_limits<T>::max()) * value);
  } else {
    return static_cast<T>(-static_cast<float>(std::numeric_limits<T>::min()) * value);
  }
}

} // namespace

namespace wstsound {

ProceduralSoundFile::ProceduralSoundFile() :
  m_format(48000, 1, 16),
  m_size(0),
  m_sample_pos(0)
{
}

size_t
ProceduralSoundFile::read(void* buffer, size_t buffer_size)
{
  int16_t* samples = static_cast<int16_t*>(buffer);
  size_t len = buffer_size / sizeof(int16_t);

  for(size_t i = 0; i < len; ++i) {
    size_t const pos = m_sample_pos + i;
    float const pos_sec = static_cast<float>(pos) / static_cast<float>(m_format.get_rate());
    float const value = sinf(pos_sec * 5000.0f);
    samples[i] = map_to<int16_t>(value);
    //samples[i] = static_cast<int16_t>(rand() % 30000) / 4 + map_to<int16_t>(value);
  }

  m_sample_pos += len;

  return buffer_size;
}

} // namespace wstsound

/* EOF */
