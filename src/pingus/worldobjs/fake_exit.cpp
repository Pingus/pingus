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

#include "pingus/worldobjs/fake_exit.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

namespace WorldObjs {

FakeExit::FakeExit(const ReaderMapping& reader) :
  surface("traps/fake_exit"),
  smallmap_symbol("core/misc/smallmap_exit"),
  pos(),
  smashing(false)
{
  reader.read_vector("position", pos);
}

float
FakeExit::get_z_pos () const
{
  return pos.z;
}

void
FakeExit::draw (SceneContext& gc)
{
  gc.color().draw (surface, pos);
}

void
FakeExit::update ()
{
  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    catch_pingu(*pingu);

  if (smashing)
    surface.update();
}

void
FakeExit::catch_pingu (Pingu* pingu)
{
  if (surface.is_finished())
    smashing = false;

  if (   pingu->get_pos().x > pos.x - 7  && pingu->get_pos().x < pos.x + 8
         && pingu->get_pos().y > pos.y - 56 && pingu->get_pos().y < pos.y)
  {
    if (pingu->get_action() != ActionName::SPLASHED)
    {
      if (!smashing)
      {
        surface.restart();
        smashing = true;
      }

      if (surface.get_current_frame() == 4)
        pingu->set_action(ActionName::SPLASHED);
    }
  }
}

void
FakeExit::draw_smallmap(SmallMap* smallmap)
{
  smallmap->draw_sprite(smallmap_symbol, pos);
}

} // namespace WorldObjs

/* EOF */
