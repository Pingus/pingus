//  $Id: bumper.cxx,v 1.14 2003/10/18 23:17:28 grumbel Exp $
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
#include <ClanLib/Display/surfaceprovider.h>
#include "../col_map.hxx"
#include "../gui/graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../pingus_resource.hxx"
#include "../world.hxx"
#include "../worldobjsdata/bumper_data.hxx"
#include "bumper.hxx"

namespace WorldObjs {

Bumper::Bumper (const WorldObjsData::BumperData& data_)
  : data(new WorldObjsData::BumperData(data_)),
    upwards(false),
    count(0)
{
}

Bumper::~Bumper ()
{
  delete data;
}

float
Bumper::get_z_pos () const
{
  return data->pos.z;
}

void
Bumper::update ()
{
  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu) {
    catch_pingu(*pingu);
  }

  if (upwards)
    {
      ++count;
      if (count >= static_cast<int>(data->surface.get_num_frames()) )
	{
	  count = 0;
	  upwards = false;
	}
    }
}

void
Bumper::on_startup ()
{
  std::cout << "Drawing colmap entry" << std::endl;

  CL_SurfaceProvider* prov = CL_SurfaceProvider::load("Traps/bumper_cmap", PingusResource::get("traps"));
  world->get_colmap()->put(prov, static_cast<int>(data->pos.x), static_cast<int>(data->pos.y), Groundtype::GP_SOLID);
}

void
Bumper::draw (GraphicContext& gc)
{
  gc.draw(data->surface, data->pos, count);
}

void
Bumper::catch_pingu (Pingu* pingu)
{
  if (   pingu->get_y() > data->pos.y + 60
      && pingu->get_y() < data->pos.y + 100)
    {
      if (   pingu->get_x() > data->pos.x + 28
          && pingu->get_x() < data->pos.x + 32)
	{
	  if (!upwards)
	    upwards = true;
	}

      if (   upwards
          && pingu->get_x() > data->pos.x + 0
	  && pingu->get_x() < data->pos.x + 60)
	{
	  pingu->apply_force(Vector((pingu->get_x() - 30)/6, -5));
	}
    }
}

} // namespace WorldObjs

/* EOF */
