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

#include "listener.hpp"
#include "sound_manager.hpp"

namespace wstsound {

Listener::Listener(SoundManager& sound_manager) :
  m_sound_manager(sound_manager)
{
}

void
Listener::set_position(float x, float y, float z)
{
  if (!m_sound_manager.is_dummy()) { return; }
  alListener3f(AL_POSITION, x, y, z);
}

void
Listener::set_velocity(float x, float y, float z)
{
  if (!m_sound_manager.is_dummy()) { return; }
  alListener3f(AL_VELOCITY, x, y, z);
}

void
Listener::set_orientation(float at_x, float at_y, float at_z,
                          float up_x, float up_y, float up_z)
{
  if (!m_sound_manager.is_dummy()) { return; }
  ALfloat orientation[] = { at_x, at_y, at_z, up_x, up_y, up_z };
  alListenerfv(AL_ORIENTATION, orientation);
}

} // namespace wstsound

/* EOF */
