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

#include "pingus/worldobjs/smasher.hpp"

#include <assert.h>

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/particles/smoke_particle_holder.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

namespace WorldObjs {

Smasher::Smasher(const ReaderMapping& reader) :
  sprite("traps/smasher"),
  pos(),
  smashing(false),
  downwards(false),
  count(0)
{
  assert(sprite.get_frame_count() == 6);

  reader.read_vector("position", pos);
}

float
Smasher::get_z_pos () const
{
  return pos.z;
}

void
Smasher::update ()
{
  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
  {
    catch_pingu(*pingu);
  }

  if (smashing)
  {
    sprite.set_frame(count);

    if (downwards)
    {
      if (count >= 5)
      {
        // SMASH!!! The thing hitten earth and kills the pingus
        downwards = false;
        --count;
        Sound::PingusSound::play_sound("tenton");

        for(int i=0; i < 20; ++i)
        {
          world->get_smoke_particle_holder()->
            add_particle(pos.x + 20 + float(rand() % 260),
                         pos.y + 180, Math::frand()-0.5f, Math::frand()-0.5f);
        }

        for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
        {
          if ((*pingu)->is_inside(static_cast<int>(pos.x + 30),
                                  static_cast<int>(pos.y + 90),
                                  static_cast<int>(pos.x + 250),
                                  static_cast<int>(pos.y + 190)))
          {
            if ((*pingu)->get_action() != ActionName::SPLASHED)
              (*pingu)->set_action(ActionName::SPLASHED);
          }
        }
      }
      else
      {
        ++count;
      }
    }
    else
    {
      if (count <= 0)
      {
        count = 0;
        smashing = false;
      } else {
        --count;
      }
    }
  }
}

void
Smasher::on_startup ()
{
  log_info("Drawing colmap entry");
  CollisionMask buf("traps/smasher_cmap");
  world->put(buf,
             static_cast<int>(pos.x),
             static_cast<int>(pos.y),
             Groundtype::GP_SOLID);
}

void
Smasher::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pos);
}

void
Smasher::catch_pingu (Pingu* pingu)
{
  // Activate the smasher if a Pingu is under it
  if ((   pingu->direction.is_left()
          && pingu->get_pos().x > pos.x + 65
          && pingu->get_pos().x < pos.x + 85)
      ||
      (   pingu->direction.is_right()
          && pingu->get_pos().x > pos.x + 190
          && pingu->get_pos().x < pos.x + 210))
  {
    if (pingu->get_action() != ActionName::SPLASHED)
    {
      if (!smashing)
      {
        count = 0;
        downwards = true;
        smashing = true;
      }
    }
  }
}

} // namespace WorldObjs

/* EOF */
