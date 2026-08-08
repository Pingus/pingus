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

#include "static_sound_source.hpp"

#include "sound_manager.hpp"

namespace wstsound {

StaticSoundSource::StaticSoundSource(SoundChannel& channel, OpenALBufferPtr buffer) :
  OpenALSoundSource(channel),
  m_buffer(std::move(buffer)),
  m_duration(m_buffer->get_duration()),
  m_sample_duration(m_buffer->get_sample_duration())
{
  alSourcei(m_source, AL_BUFFER, m_buffer->get_handle());
  OpenALSystem::check_al_error("StaticSoundSource: ");
}

float
StaticSoundSource::sample_to_sec(int sample) const
{
  return static_cast<float>(sample) / static_cast<float>(m_buffer->get_frequency());
}

int
StaticSoundSource::sec_to_sample(float sec) const
{
  return static_cast<int>(sec * static_cast<float>(m_buffer->get_frequency()));
}

} // namespace wstsound

/* EOF */
