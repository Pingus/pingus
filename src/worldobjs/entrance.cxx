//  $Id: entrance.cxx,v 1.14 2003/10/18 23:17:28 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <iostream>
#include "../display/scene_context.hxx"
#include "../world.hxx"
#include "../pingu_holder.hxx"
#include "../globals.hxx"
#include "../pingu.hxx"
#include "../game_time.hxx"
#include "../smallmap.hxx"
#include "../resource.hxx"
#include "entrance.hxx"

namespace Pingus {
namespace WorldObjs {

Entrance::Entrance(const FileReader& reader)
  : smallmap_symbol(Resource::load_sprite("misc/smallmap_entrance", "core"))
{
  reader.read_string("type",         type);
  reader.read_int   ("owner-id",     owner_id);
  reader.read_vector("position",     pos);
  reader.read_int   ("release-rate", release_rate);

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
      std::cout << "EntranceData: Unknown direction: '" << direction_str << "'" << std::endl;
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
  if (last_release + release_rate < (world->get_game_time()->get_ticks())) {
    last_release = world->get_game_time()->get_ticks();
    return true;
  } else {
    return false;
  }
}

void
Entrance::create_pingu ()
{
  static int last_direction;
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
          std::cout << "Entrance:: Warning direction is wrong: " << direction << std::endl;
          d.right();
          pingu->set_direction(d);
          break;
        }

      world->play_sound("oing", pos);
    }
  else
    {
      //std::cout << "Entrance: pingu couldn't get created" << std::endl;
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
      //std::cout << "Entrance::draw (SceneContext& gc): entrance without a surface?!" << std::endl;
      return;
    }

  // FIXME: Why do we still have these hardcoded offsets?!
  gc.color().draw(surface, Vector(pos.x - 32, pos.y - 16));
}

void
Entrance::draw_smallmap(SmallMap* smallmap)
{
  smallmap->draw_sprite(smallmap_symbol, pos);
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
