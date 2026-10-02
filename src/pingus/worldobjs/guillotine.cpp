// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/worldobjs/guillotine.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

namespace pingus::worldobjs {

Guillotine::Guillotine(ReaderMapping const& reader) :
  sprite_kill_right("traps/guillotinekill/right"),
  sprite_kill_left("traps/guillotinekill/left"),
  sprite_idle("traps/guillotineidle"),
  kill_clock(AnimationClock::from_sprite("traps/guillotinekill/left")),
  idle_clock(AnimationClock::from_sprite("traps/guillotineidle")),
  pos(),
  m_z_index(0.0f),
  direction(),
  killing(false)
{
  InVector2fZ in_vec{pos, m_z_index};
  reader.read("position", in_vec);

  kill_clock.set_loop(false);
  idle_clock.set_loop(true);
}

void
Guillotine::draw (SceneContext& gc)
{
  if (killing) {
    Sprite& sprite = direction.is_left() ? sprite_kill_left : sprite_kill_right;
    kill_clock.apply_to(sprite);
    gc.color().draw (sprite, pos);
  } else {
    idle_clock.apply_to(sprite_idle);
    gc.color().draw (sprite_idle, pos);
  }
}

float
Guillotine::z_index() const
{
  return m_z_index;
}

void
Guillotine::update()
{
  if (kill_clock.is_finished())
    killing = false;

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
    catch_pingu(*pingu);

  if (killing) {
    kill_clock.update();
    // FIXME: Should be a different sound
    if (kill_clock.frame() == 7)
      WorldObj::get_world()->play_sound("splash", pos);
  } else {
    idle_clock.update();
  }
}

void
Guillotine::catch_pingu (Pingu* pingu)
{
  if (!killing)
  {
    if (pingu->is_inside (pos.x() + 38, pos.y() + 90,
                          pos.x() + 42, pos.y() + 98))
    {
      killing = true;
      pingu->set_status(Pingu::PS_DEAD);
      direction = pingu->direction;
      kill_clock.restart();
    }
  }
}

} // namespace pingus::worldobjs

/* EOF */
