// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldobjs/switch_door_door.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

namespace WorldObjs {

SwitchDoorDoor::SwitchDoorDoor(const ReaderMapping& reader) :
  WorldObj(reader),
  door_pos(),
  door_height(),
  door_box     ("worldobjs/switchdoor_box"),
  door_box_cmap("worldobjs/switchdoor_box"),
  door_tile    ("worldobjs/switchdoor_tile"),
  door_tile_cmap("worldobjs/switchdoor_tile_cmap"),
  is_opening(false),
  current_door_height()
{
  reader.read_vector("position", door_pos);
  reader.read_int("height", door_height);
  current_door_height = door_height;
}

void
SwitchDoorDoor::on_startup()
{
  world->get_colmap()->put(door_box_cmap,
                           static_cast<int>(door_pos.x),
                           static_cast<int>(door_pos.y),
                           Groundtype::GP_SOLID);

  for (int i=0; i < door_height; ++i)
    world->get_colmap()->put(door_tile_cmap,
                             static_cast<int>(door_pos.x),
                             static_cast<int>(door_pos.y)
                             + i * door_tile.get_height()
                             + door_box.get_height(),
                             Groundtype::GP_SOLID);
}

void
SwitchDoorDoor::draw(SceneContext& gc)
{
  gc.color().draw (door_box, door_pos);
  for (int i = 0; i < current_door_height; ++i)
    gc.color().draw(door_tile, Vector3f(door_pos.x,
                                        door_pos.y + static_cast<float>(i * door_tile.get_height() + door_box.get_height())));
}

void
SwitchDoorDoor::update ()
{
  if (current_door_height > 0)
  {
    if (is_opening)
    {
      // Open the door
      --current_door_height;

      // If the door is opend enough, so that a pingus fits under
      // it, we remove the door from the colmap
      if (current_door_height + 10 < door_height)
      {
        world->get_colmap()->put(door_box_cmap,
                                 static_cast<int>(door_pos.x),
                                 static_cast<int>(door_pos.y),
                                 Groundtype::GP_NOTHING);
        for (int i=0; i < door_height; ++i)
          world->get_colmap()->put(door_tile_cmap,
                                   static_cast<int>(door_pos.x),
                                   static_cast<int>(door_pos.y) + i * door_tile.get_height()
                                   + door_box.get_height(),
                                   Groundtype::GP_NOTHING);
      }
    }
  }
}

void
SwitchDoorDoor::open_door()
{
  is_opening = true;
}

} // namespace WorldObjs

/* EOF */
