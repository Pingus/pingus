//  $Id: teleporter.cxx,v 1.12 2002/09/28 11:52:26 torangan Exp $
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
#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../worldobjsdata/teleporter_data.hxx"
#include "teleporter.hxx"

namespace WorldObjs {

Teleporter::Teleporter (const WorldObjsData::TeleporterData& data_)
  : data(new WorldObjsData::TeleporterData(data_))

{  
  data->sprite.set_align_center_bottom();
  data->target_sprite.set_align_center();

  //FIXME: we need a Sprite::set_frame()

  std::cout << "Teleporter: pos: " << data->pos.x << " "  << data->pos.y << " " << data->pos.z << std::endl;
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
Teleporter::draw (GraphicContext& gc)
{
  gc.draw(data->sprite, data->pos);
  gc.draw(data->target_sprite, data->target_pos);
}

void 
Teleporter::update (float delta)
{
  data->sprite.update(delta);
  data->target_sprite.update(delta);

  PinguHolder* holder = world->get_pingu_p();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if (   (*pingu)->get_x() > data->pos.x - 3  && (*pingu)->get_x() < data->pos.x + 3
	     && (*pingu)->get_y() > data->pos.y - 52 && (*pingu)->get_y() < data->pos.y)
	{
	  (*pingu)->set_pos (static_cast<int>(data->target_pos.x), static_cast<int>(data->target_pos.y));
	  data->sprite.reset ();
	  data->target_sprite.reset ();
	}
    }
}

} // namespace WorldObjs

/* EOF */
