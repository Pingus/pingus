//  $Id: teleporter.cxx,v 1.18 2003/10/19 12:25:47 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../resource.hxx"
#include "../worldobjsdata/teleporter_data.hxx"
#include "teleporter.hxx"

namespace Pingus {
namespace WorldObjs {

Teleporter::Teleporter (const WorldObjsData::TeleporterData& data_)
  : data(new WorldObjsData::TeleporterData(data_)),
    sprite(Resource::load_sprite("teleporter", "worldobjs")),
    target_sprite(Resource::load_sprite("teleportertarget", "worldobjs"))
{
}

Teleporter::~Teleporter ()
{
  delete data;
}

float
Teleporter::get_z_pos () const
{
  return data->pos.z;
}

void
Teleporter::draw (SceneContext& gc)
{
  gc.color().draw(sprite, data->pos);
  gc.color().draw(target_sprite, data->target_pos);
}

void
Teleporter::update ()
{
  sprite.update();
  target_sprite.update();

  PinguHolder* holder = world->get_pingus();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if (   (*pingu)->get_x() > data->pos.x - 3  && (*pingu)->get_x() < data->pos.x + 3
	     && (*pingu)->get_y() > data->pos.y - 52 && (*pingu)->get_y() < data->pos.y)
	{
	  (*pingu)->set_pos (data->target_pos.x, data->target_pos.y);
	  sprite.restart();
          target_sprite.restart();
	}
    }
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
