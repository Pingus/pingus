//  $Id: liquid.cxx,v 1.10 2003/10/19 12:25:47 grumbel Exp $
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
#include "../col_map.hxx"
#include "../gui/graphic_context.hxx"
#include "../pingus_resource.hxx"
#include "../world.hxx"
#include "../worldobjsdata/liquid_data.hxx"
#include "liquid.hxx"

namespace Pingus {
namespace WorldObjs {

Liquid::Liquid (const WorldObjsData::LiquidData& data_) :
  data(new WorldObjsData::LiquidData(data_)),
  sur(data->desc, (data->speed == 0) ? 30 : 1000.0f/data->speed)
{
  if (!data->old_width_handling)
    data->width *= sur.get_width();
}

Liquid::~Liquid ()
{
  delete data;
}

float
Liquid::get_z_pos () const
{
  return data->pos.z;
}

void
Liquid::on_startup ()
{
  CL_Surface colmap_sur = PingusResource::load_surface("Liquid/water_cmap", "global");

  for(int i=0; i < data->width; ++i)
    world->get_colmap()->put(colmap_sur,
                             static_cast<int>(data->pos.x + i),
			     static_cast<int>(data->pos.y),
			     Groundtype::GP_WATER);
}

void
Liquid::draw (GraphicContext& gc)
{
  for(int x = static_cast<int>(data->pos.x);
      x < data->pos.x + data->width;
      x += sur.get_width())
    gc.draw(sur, Vector(x, data->pos.y));
}

void
Liquid::update()
{
  sur.update();
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
