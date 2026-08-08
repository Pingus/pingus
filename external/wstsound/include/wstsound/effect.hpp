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

#ifndef HEADER_WSTSOUND_EFFECT_HPP
#define HEADER_WSTSOUND_EFFECT_HPP

#include <al.h>
#define AL_ALEXT_PROTOTYPES
#include <efx.h>

namespace wstsound {

class Effect
{
private:
public:
  Effect(int effect_type);
  ~Effect();

  void seti(int param, int value);
  void setf(int param, float value);

  ALuint handle() const { return m_effect; }

private:
  ALuint m_effect;

private:
  Effect(const Effect&) = delete;
  Effect& operator=(const Effect&) = delete;
};

} // namespace wstsound

#endif

/* EOF */
