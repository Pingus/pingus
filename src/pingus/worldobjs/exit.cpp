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

#include "pingus/worldobjs/exit.hpp"

#include <strut/to_string.hpp>

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

namespace pingus::worldobjs {

Exit::Exit(ReaderMapping const& reader) :
  desc(),
  pos(),
  m_z_index(0.0f),
  owner_id(),
  sprite(),
  flag(),
  smallmap_symbol("core/misc/smallmap_exit")
{
  InVector2fZ in_vec{pos, m_z_index};
  reader.read("position", in_vec);
  reader.read("surface",  desc);
  reader.read("owner-id", owner_id);

  // Set default owner ID to 0
  if (owner_id < 0 || owner_id > 3) owner_id = 0;

  flag = Sprite("core/misc/flag" + strut::to_string(owner_id));

  sprite = Sprite(desc);
}

Exit::~Exit()
{
}

void
Exit::on_startup()
{
  CollisionMask mask(desc);
  world->get_colmap()->remove(mask,
                              static_cast<int>(pos.x()) - sprite.get_width()/2,
                              static_cast<int>(pos.y()) - sprite.get_height());
}

void
Exit::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pos);
  gc.color().draw(flag, pos + geom::foffset(40, 0));
}

void
Exit::draw_smallmap(SmallMap* smallmap)
{
  smallmap->draw_sprite(smallmap_symbol, pos);
}

void
Exit::update()
{
  sprite.update();

  PinguHolder* holder = world->get_pingus();

  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
  {
    // Make sure this particular exit is allowed for this pingu
    if ((*pingu)->get_owner()  == owner_id)
    {
      // Now, make sure the pingu is within range
      if (   (*pingu)->get_pos().x() > pos.x() - 1 && (*pingu)->get_pos().x() < pos.x() + 1
             && (*pingu)->get_pos().y() > pos.y() - 5 && (*pingu)->get_pos().y() < pos.y() + 5)
      {
        // Now, make sure the pingu isn't already exiting, gone, or dead
        if (   (*pingu)->get_status() != Pingu::PS_EXITED
               && (*pingu)->get_status() != Pingu::PS_DEAD
               && (*pingu)->get_action() != ActionName::EXITER)
        {
          // Pingu actually exits
          (*pingu)->set_action(ActionName::EXITER);
        }
      }
    }
  }
}

float
Exit::z_index() const
{
  return m_z_index;
}

} // namespace pingus::worldobjs

/* EOF */
