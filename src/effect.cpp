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

#include "effect.hpp"

#include <al.h>
#include <efx.h>

namespace wstsound {

Effect::Effect(int effect_type) :
  m_effect()
{
  alGenEffects(1, &m_effect);

  alEffecti(m_effect, AL_EFFECT_TYPE, effect_type);
}

Effect::~Effect()
{
  alDeleteEffects(1, &m_effect);
}

void
Effect::seti(int param, int value) // NOLINT
{
  alEffecti(m_effect, param, value);
}

void
Effect::setf(int param, float value) // NOLINT
{
  alEffectf(m_effect, param, value);
}

} // namespace wstsound

/* EOF */
