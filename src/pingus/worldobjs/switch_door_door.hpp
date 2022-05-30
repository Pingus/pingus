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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_SWITCH_DOOR_DOOR_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_SWITCH_DOOR_DOOR_HPP

#include "math/vector2f.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/worldobj.hpp"

namespace pingus::worldobjs {

class SwitchDoorDoor : public WorldObj
{
private:
  /// The upper/middle pos of the door
  Vector2f door_pos;
  float m_z_index;

  // The height of the door in graphic tiles
  int door_height;

  Sprite door_box;
  CollisionMask door_box_cmap;
  Sprite door_tile;
  CollisionMask door_tile_cmap;

  /** True if the door is opening */
  bool is_opening;

  /** The current height of the door, it might decrease when the door
      is opening, it will be zero when the door is fully opened */
  int current_door_height;

public:
  SwitchDoorDoor(ReaderMapping const& reader);

  void on_startup() override;
  void draw (SceneContext& gc) override;
  void update () override;

  /// The switch and the door should stay above the pingus
  float z_index() const override { return 100; }
  void set_z_index(float z_index) override { m_z_index = z_index; }
  void set_pos(Vector2f const& p) override { door_pos = p; }
  Vector2f get_pos() const override { return door_pos; }

  void open_door();

private:
  SwitchDoorDoor(SwitchDoorDoor const&);
  SwitchDoorDoor& operator=(SwitchDoorDoor const&);
};

} // namespace pingus::worldobjs

#endif

/* EOF */
