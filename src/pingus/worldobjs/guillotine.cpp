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

namespace WorldObjs {

Guillotine::Guillotine(const ReaderMapping& reader) :
  sprite_kill_right("traps/guillotinekill/right"),
  sprite_kill_left("traps/guillotinekill/left"),
  sprite_idle("traps/guillotineidle"),
  pos(),
  direction(),
  killing(false)
{
  reader.read_vector("position", pos);

  sprite_kill_right.set_play_loop(false);
  sprite_kill_left.set_play_loop(false);
  sprite_idle.set_play_loop(true);
}

void
Guillotine::draw (SceneContext& gc)
{
  if (killing) {
    if (direction.is_left())
      gc.color().draw (sprite_kill_left, pos);
    else
      gc.color().draw (sprite_kill_right, pos);
  } else {
    gc.color().draw (sprite_idle, pos);
  }
}

float
Guillotine::get_z_pos () const
{
  return pos.z;
}

void
Guillotine::update ()
{
  // Only have to check one sprite because they update simultaneously
  if (sprite_kill_left.is_finished())
    killing = false;

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    catch_pingu(*pingu);

  if (killing) {
    // Update both sprites so they finish at the same time.
    sprite_kill_left.update();
    sprite_kill_right.update();
    // FIXME: Should be a different sound
    if (sprite_kill_left.get_current_frame() == 7)
      WorldObj::get_world()->play_sound("splash", pos);
  } else {
    sprite_idle.update();
  }
}

void
Guillotine::catch_pingu (Pingu* pingu)
{
  if (!killing)
  {
    if (pingu->is_inside (static_cast<int>(pos.x + 38), static_cast<int>(pos.y + 90),
                          static_cast<int>(pos.x + 42), static_cast<int>(pos.y + 98)))
    {
      killing = true;
      pingu->set_status(Pingu::PS_DEAD);
      direction = pingu->direction;
      sprite_kill_left.restart();
      sprite_kill_right.restart();
    }
  }
}

} // namespace WorldObjs

/* EOF */
