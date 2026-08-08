/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2010 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_WINDSTILLE_SOUND_STATIC_SOUND_SOURCE_HPP
#define HEADER_WINDSTILLE_SOUND_STATIC_SOUND_SOURCE_HPP

#include "openal_buffer.hpp"
#include "openal_sound_source.hpp"

namespace wstsound {

class StaticSoundSource : public OpenALSoundSource
{
public:
  StaticSoundSource(SoundChannel& channel, OpenALBufferPtr buffer);
  ~StaticSoundSource() override {}

  float get_duration() const override { return m_duration; }
  int get_sample_duration() const override  { return m_sample_duration; }

  float sample_to_sec(int sample) const override;
  int sec_to_sample(float sec) const override;

private:
  OpenALBufferPtr m_buffer;
  float m_duration;
  int m_sample_duration;

private:
  StaticSoundSource(const StaticSoundSource&);
  StaticSoundSource& operator=(const StaticSoundSource&);
};

} // namespace wstsound

#endif

/* EOF */
