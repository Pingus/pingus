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

#include "effect_slot.hpp"

#define AL_ALEXT_PROTOTYPES
#include <efx.h>

#include "effect.hpp"
#include "openal_system.hpp"

namespace wstsound {

EffectSlot::EffectSlot() :
  m_slot(),
  m_effect()
{
  alGenAuxiliaryEffectSlots(1, &m_slot);
  OpenALSystem::check_al_error("EffectSlot::EffectSlot()");
}

EffectSlot::~EffectSlot()
{
  alDeleteAuxiliaryEffectSlots(1, &m_slot);
  OpenALSystem::warn_al_error("EffectSlot::~EffectSlot()");
}

void
EffectSlot::set_effect(EffectPtr const& effect)
{
  m_effect = effect;

  if (effect) {
    alAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT, effect->handle());
  } else {
    alAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);
  }
  OpenALSystem::check_al_error("EffectSlot::set_effect()");
}

} // namespace wstsound

/* EOF */
