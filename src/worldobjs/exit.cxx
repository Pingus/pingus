//  $Id: exit.cxx,v 1.8 2003/02/19 09:50:36 grumbel Exp $
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
#include "../col_map.hxx"
#include "../world.hxx"
#include "../globals.hxx"
#include "../pingu_holder.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "../worldobjsdata/exit_data.hxx"
#include "../smallmap.hxx"
#include "exit.hxx"

namespace WorldObjs {

Exit::Exit (const WorldObjsData::ExitData& data_)
  : data(new WorldObjsData::ExitData(data_)),
    sprite(data->desc, 10.0f),
    flag("misc/flag" + to_string(data->owner_id), "core"),
    smallmap_symbol("misc/smallmap_exit", "core")
{
  flag.set_align_center_bottom();
  if (verbose > 2)
    std::cout << "Creating Exit" << std::endl;

  sprite.set_align_center_bottom();

  smallmap_symbol.set_align_center_bottom();

  if (data->use_old_pos_handling) {
    data->pos.x += sprite.get_width() / 2;
    data->pos.y += sprite.get_height();
    data->use_old_pos_handling = false;
  }
}

Exit::~Exit ()
{
  delete data;
}

void
Exit::on_startup ()
{
  world->get_colmap()->remove(sprite.get_surface(),
			      static_cast<int>(data->pos.x) - sprite.get_width()/2,
			      static_cast<int>(data->pos.y) - sprite.get_height());
}

void
Exit::draw (GraphicContext& gc)
{
  gc.draw(sprite, data->pos);
  gc.draw(flag, data->pos + Vector(40, 0));
}

void
Exit::draw_smallmap(SmallMap* smallmap)
{
  smallmap->draw_sprite(smallmap_symbol, data->pos);
}

void
Exit::update ()
{
  sprite.update();

  PinguHolder* holder = world->get_pingus();

  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
    {
      if (   (*pingu)->get_x() > data->pos.x - 1 && (*pingu)->get_x() < data->pos.x + 1
	  && (*pingu)->get_y() > data->pos.y - 5 && (*pingu)->get_y() < data->pos.y + 1)
	{
	  if (   (*pingu)->get_status() != PS_EXITED
	      && (*pingu)->get_status() != PS_DEAD
	      && (*pingu)->get_action() != Actions::Exiter)
	    {
	      (*pingu)->set_action(Actions::Exiter);
	    }
	}
    }
}

float
Exit::get_z_pos () const
{
  return data->pos.z;
}

} // namespace WorldObjs

/* EOF */
