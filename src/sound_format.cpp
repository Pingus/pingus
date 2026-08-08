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

#include "sound_format.hpp"

#include "sound_error.hpp"

namespace wstsound {

SoundFormat::SoundFormat() :
  m_rate(),
  m_channels(),
  m_bits_per_sample()
{
}

SoundFormat::SoundFormat(int rate, int channels, int bits_per_sample) :
  m_rate(rate),
  m_channels(channels),
  m_bits_per_sample(bits_per_sample)
{
}

size_t
SoundFormat::sample2bytes(int sample) const
{
  return sample * get_channels() * get_bits_per_sample() / 8;
}

ALenum
SoundFormat::get_openal_format() const
{
  if (m_channels == 2)
  {
    if (m_bits_per_sample == 16)
    {
      return AL_FORMAT_STEREO16;
    }
    else if (m_bits_per_sample == 8)
    {
      return AL_FORMAT_STEREO8;
    }
    else
    {
      throw SoundError("Only 16 and 8 bit samples supported");
    }
  }
  else if (m_channels == 1)
  {
    if (m_bits_per_sample == 16)
    {
      return AL_FORMAT_MONO16;
    }
    else if (m_bits_per_sample == 8)
    {
      return AL_FORMAT_MONO8;
    }
    else
    {
      throw SoundError("Only 16 and 8 bit samples supported");
    }
  }
  else
  {
    throw SoundError("Only 1 and 2 channel samples supported");
  }
}

} // namespace wstsound

/* EOF */
