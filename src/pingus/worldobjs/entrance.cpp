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

#include "pingus/worldobjs/entrance.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

namespace WorldObjs {

Entrance::Entrance(const ReaderMapping& reader) :
  direction(MISC),
  pos(),
  release_rate(150),
  owner_id(0),
  smallmap_symbol("core/misc/smallmap_entrance"),
  surface(),
  last_release(),
  last_direction(0)
{
  reader.read_int   ("owner-id",     owner_id);
  reader.read_vector("position",     pos);
  reader.read_int   ("release-rate", release_rate);

  // Set default owner ID to 0
  if (owner_id < 0 || owner_id > 3) owner_id = 0;

  std::string direction_str;
  reader.read_string("direction", direction_str);

  if (direction_str == "left")
    direction = LEFT;
  else if (direction_str == "right")
    direction = RIGHT;
  else if (direction_str == "misc")
    direction = MISC;
  else
  {
    log_error("unknown direction: '%1%'", direction_str);
    direction = MISC;
  }

  last_release = 150 - release_rate; // wait ~2sec at startup to allow a 'lets go' sound
}

Entrance::~Entrance ()
{
}

float
Entrance::get_z_pos () const
{
  return pos.z;
}

bool
Entrance::pingu_ready ()
{
  if (last_release + release_rate < (world->get_time())) {
    last_release = world->get_time();
    return true;
  } else {
    return false;
  }
}

void
Entrance::create_pingu ()
{
  Direction d;

  Pingu* pingu = world->get_pingus()->create_pingu(pos, owner_id);

  if (pingu) // still pingus in the pool
  {
    switch (direction)
    {
      case LEFT:
        d.left();
        pingu->set_direction(d);
        break;

      case MISC:
        if (last_direction)
        {
          d.left();
          last_direction = 0;
        }
        else
        {
          d.right();
          last_direction = 1;
        }
        pingu->set_direction(d);
        break;

      case RIGHT:
        d.right();
        pingu->set_direction(d);
        break;

      default:
        log_error("warning direction is wrong: %1%", direction);
        d.right();
        pingu->set_direction(d);
        break;
    }

    // FIXME: Find the "oing" sound
    //world->play_sound("oing", pos);
  }
  else
  {
    //log_error("entrance: pingu couldn't get created");
  }
}

void
Entrance::update ()
{
  if (pingu_ready() && (! world->check_armageddon()))
  {
    create_pingu();
  }
}

void
Entrance::draw (SceneContext& gc)
{
  if (!surface)
  {
    // Entrances have only a surface for historical reasons
    // log_error("entrance without a surface?!");
    return;
  }

  // FIXME: Why do we still have these hardcoded offsets?!
  gc.color().draw(surface, Vector3f(pos.x - 32, pos.y - 16));
}

void
Entrance::draw_smallmap(SmallMap* smallmap)
{
  smallmap->draw_sprite(smallmap_symbol, pos);
}

} // namespace WorldObjs

/* EOF */
