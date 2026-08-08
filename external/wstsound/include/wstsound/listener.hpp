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

#ifndef HEADER_WINDSTILLE_LISTENER_HPP
#define HEADER_WINDSTILLE_LISTENER_HPP

namespace wstsound {

class SoundManager;

class Listener
{
public:
  Listener(SoundManager& sound_manager);

  void set_position(float x, float y, float z);
  void set_velocity(float x, float y, float z);
  void set_orientation(float at_x, float at_y, float at_z,
                       float up_x, float up_y, float up_z);

private:
  SoundManager& m_sound_manager;

private:
  Listener(const Listener&) = delete;
  Listener& operator=(const Listener&) = delete;
};

} // namespace wstsound

#endif

/* EOF */
