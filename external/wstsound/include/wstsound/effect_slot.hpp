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

#ifndef HEADER_WSTSOUND_EFFECT_SLOT_HPP
#define HEADER_WSTSOUND_EFFECT_SLOT_HPP

#include "fwd.hpp"

#include <al.h>

namespace wstsound {

class EffectSlot
{
private:
public:
  EffectSlot();
  ~EffectSlot();

  void set_effect(EffectPtr const& effect);

  ALuint handle() const { return m_slot; }

private:
  ALuint m_slot;
  EffectPtr m_effect;

private:
  EffectSlot(const EffectSlot&) = delete;
  EffectSlot& operator=(const EffectSlot&) = delete;
};

} // namespace wstsound

#endif

/* EOF */
