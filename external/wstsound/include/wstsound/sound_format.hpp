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

#ifndef HEADER_WSTSOUND_SOUND_FORMAT_HPP
#define HEADER_WSTSOUND_SOUND_FORMAT_HPP

#include <stddef.h>

#include <al.h>

namespace wstsound {

class SoundFormat final
{
public:
  SoundFormat();
  SoundFormat(int bits_per_sample, int rate, int channels);

  /** Bits per sample, usually 8 or 16 */
  int get_bits_per_sample() const { return m_bits_per_sample; }

  /** The sample rate or frequency of the file, usually 44100 or 48000 */
  int get_rate() const { return m_rate; }

  /** The number of channels, 1 for mono, 2 for stereo */
  int get_channels() const { return m_channels; }

  /** Calculate the number of bytes used by the given samples */
  size_t sample2bytes(int sample) const;

  ALenum get_openal_format() const;

private:
  int m_rate;
  int m_channels;
  int m_bits_per_sample;
};

} // namespace wstsound

#endif

/* EOF */
