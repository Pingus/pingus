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
#include "../gui/graphic_context.hxx"
#include "../world.hxx"
#include "../pingu_holder.hxx"
#include "../globals.hxx"
#include "../pingu.hxx"
#include "../game_time.hxx"
#include "../worldobjsdata/entrance_data.hxx"
#include "../smallmap.hxx"
#include "entrance.hxx"

namespace Pingus {
namespace WorldObjs {

Entrance::Entrance (const WorldObjsData::EntranceData& data_)
  : data(new WorldObjsData::EntranceData(data_)),
    smallmap_symbol("misc/smallmap_entrance", "core"),
    last_release(150 - data->release_rate) // wait ~2sec at startup to allow a 'lets go' sound
{
  smallmap_symbol.set_align_center_bottom();
  if (verbose > 2)
    std::cout << "Creating Entrance" << std::endl;
}

Entrance::~Entrance ()
{
  delete data;
}

float
Entrance::get_z_pos () const
{
  return data->pos.z;
}

bool
Entrance::pingu_ready ()
{
  if (last_release + data->release_rate < (world->get_game_time()->get_ticks())) {
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

  Pingu* pingu = world->get_pingus()->create_pingu(data->pos, data->owner_id);

  if (pingu) // still pingus in the pool
    {
      switch (data->direction)
        {
        case WorldObjsData::EntranceData::LEFT:
          d.left();
          pingu->set_direction(d);
          break;

        case WorldObjsData::EntranceData::MISC:
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

        case WorldObjsData::EntranceData::RIGHT:
          d.right();
          pingu->set_direction(d);
          break;

        default:
          std::cout << "Entrance:: Warning direction is wrong: " << data->direction << std::endl;
          d.right();
          pingu->set_direction(d);
          break;
        }

      world->play_sound("oing", data->pos);
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
Entrance::draw (GraphicContext& gc)
{
  if (!surface)
    {
      // Entrances have only a surface for historical reasons
      //std::cout << "Entrance::draw (GraphicContext& gc): entrance without a surface?!" << std::endl;
      return;
    }

  // FIXME: Why do we still have these hardcoded offsets?!
  gc.draw(surface, Vector(data->pos.x - 32, data->pos.y - 16));
}

void
Entrance::draw_smallmap(SmallMap* smallmap)
{
  smallmap->draw_sprite(smallmap_symbol, data->pos);
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
