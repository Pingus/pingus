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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_SWITCH_DOOR_SWITCH_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_SWITCH_DOOR_SWITCH_HPP

#include "math/vector3f.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/worldobj.hpp"

namespace WorldObjs {

class SwitchDoorDoor;

/** A door and a switch, if a pingu is passing the switch, the door
    will open. */
class SwitchDoorSwitch : public WorldObj
{
private:
  /// The bottom/middle pos of the switch
  Vector3f switch_pos;
  Sprite switch_sur;
  std::string m_target;
  bool is_triggered;
  SwitchDoorDoor* m_door;

public:
  SwitchDoorSwitch(const ReaderMapping& reader);

  void on_startup();
  void draw (SceneContext& gc);
  void update ();

  /// The switch and the door should stay above the pingus
  float get_z_pos() const { return 100; }
  void set_pos(const Vector3f& p) { switch_pos = p; }
  Vector3f get_pos() const { return switch_pos; }

private:
  SwitchDoorSwitch (const SwitchDoorSwitch&);
  SwitchDoorSwitch& operator= (const SwitchDoorSwitch&);
};

} // namespace WorldObjs

#endif

/* EOF */
