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

#include "pingus/worldobjs/ice_block.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

namespace WorldObjs {

IceBlock::IceBlock(const ReaderMapping& reader) :
  pos(),
  width(),
  thickness(1.0),
  is_finished(false),
  last_contact(0),
  block_sur("worldobjs/iceblock"),
  block_sur_cmap("worldobjs/iceblock_cmap")
{
  reader.read_vector("position", pos);
  if (!reader.read_int("repeat", width))
  {
    log_warn("old 'width' tag used");
    reader.read_int("width", width);
  }
}

void
IceBlock::on_startup ()
{
  world->put(block_sur_cmap,
             static_cast<int>(pos.x),
             static_cast<int>(pos.y),
             Groundtype::GP_GROUND);
}

void
IceBlock::draw (SceneContext& gc)
{
  if (is_finished)
    return;

  gc.color().draw(block_sur,
                  pos);
  //, static_cast<int>((1.0 - thickness) * (block_sur.get_frame_count() - 1)));
}

void
IceBlock::update()
{
  if (is_finished)
    return;

  PinguHolder* holder = world->get_pingus();

  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
  {
    if ((*pingu)->get_x() > pos.x     && (*pingu)->get_x() < pos.x + static_cast<float>(block_sur.get_width()) &&
        (*pingu)->get_y() > pos.y - 4 && (*pingu)->get_y() < pos.y + static_cast<float>(block_sur.get_height()))
    {
      last_contact = world->get_time();
    }
  }

  if (last_contact && last_contact + 1000 > world->get_time())
  {
    //log_error("IceBlock: Catched Pingu: " << thickness);
    thickness -= 0.01f;

    if (thickness < 0)
    {
      is_finished = true;
      thickness = 0;

      world->remove(block_sur_cmap, static_cast<int>(pos.x), static_cast<int>(pos.y));
      return;
    }
  }
}

} // namespace WorldObjs

/* EOF */
