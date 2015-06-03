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

#include "pingus/worldobjs/switch_door_switch.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobjs/switch_door_door.hpp"
#include "util/log.hpp"

namespace WorldObjs {

SwitchDoorSwitch::SwitchDoorSwitch(const ReaderMapping& reader) :
  switch_pos(),
  switch_sur("worldobjs/switchdoor_switch"),
  m_target(),
  is_triggered(false),
  m_door()
{
  reader.read_string("target-id", m_target);
  reader.read_vector("position", switch_pos);
}

void
SwitchDoorSwitch::on_startup ()
{
  if (m_target.empty())
  {
    log_error("target-id is empty");
  }
  else
  {
    m_door = dynamic_cast<SwitchDoorDoor*>(world->get_worldobj(m_target));
    if (!m_door)
      log_error("given target-id is not a SwitchDoorDoor");
  }
}

void
SwitchDoorSwitch::draw (SceneContext& gc)
{
  gc.color().draw(switch_sur, switch_pos);
}

void
SwitchDoorSwitch::update ()
{
  if (!is_triggered)
  {
    if (m_door)
    {
      // Check if a pingu is passing the switch
      PinguHolder* holder = world->get_pingus();

      for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
      {
        if ((*pingu)->get_pos().x > switch_pos.x &&
            (*pingu)->get_pos().x < switch_pos.x + static_cast<float>(switch_sur.get_width()) &&
            (*pingu)->get_pos().y > switch_pos.y &&
            (*pingu)->get_pos().y < switch_pos.y + static_cast<float>(switch_sur.get_height()))
        {
          is_triggered = true;
          m_door->open_door();
          break;
        }
      }
    }
  }
}

} // namespace WorldObjs

/* EOF */
