// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldobjs/teleporter.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobjs/teleporter_target.hpp"
#include "util/log.hpp"

namespace WorldObjs {

Teleporter::Teleporter(const ReaderMapping& reader) :
  pos(),
  sprite("worldobjs/teleporter"),
  target_id(),
  target()
{
  reader.read_vector("position", pos);
  reader.read_string("target-id", target_id);
}

float
Teleporter::get_z_pos () const
{
  return pos.z;
}

void
Teleporter::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pos);
}

void
Teleporter::on_startup()
{
  if (target_id.empty())
  {
    log_error("target-id is empty");
  }
  else
  {
    // FIXME: find the target
    target = dynamic_cast<TeleporterTarget*>(world->get_worldobj(target_id));
    if (!target)
      log_error("Teleporter: Couldn't find matching target-id or object isn't a TeleporterTarget");
  }
}

void
Teleporter::update ()
{
  sprite.update();

  if (target)
  {
    PinguHolder* holder = world->get_pingus();
    for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if (   (*pingu)->get_x() > pos.x - 3  && (*pingu)->get_x() < pos.x + 3
             && (*pingu)->get_y() > pos.y - 52 && (*pingu)->get_y() < pos.y)
      {
        (*pingu)->set_pos(target->get_pos().x, target->get_pos().y);
        target->teleporter_used();
        sprite.restart();
      }
    }
  }
}

} // namespace WorldObjs

/* EOF */
