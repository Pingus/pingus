//  $Id: liquid.cxx,v 1.1 2002/09/25 17:21:39 torangan Exp $
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

#include "../col_map.hxx"
#include "../graphic_context.hxx"
#include "../pingus_resource.hxx"
#include "../world.hxx"
#include "../worldobjsdata/liquid_data.hxx"
#include "liquid.hxx"

namespace WorldObjs {

Liquid::Liquid (WorldObjsData::LiquidData* data_) :
  data(new WorldObjsData::LiquidData(*data_)),
  sur(PingusResource::load_surface(data->desc.res_name, "global"))
{
  if (!data->old_width_handling)
    data->width *= sur.get_width();

  counter.set_size(sur.get_num_frames());
  counter.set_speed(data->speed);
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
  CL_Surface sur = PingusResource::load_surface("Liquid/water_cmap", "global");

  for(int i=0; i < data->width; ++i)
    world->get_colmap()->put(sur,
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
    gc.draw(sur, x, static_cast<int>(data->pos.y), static_cast<int>(counter));
  
  ++counter;
}

} // namespace WorldObjs

/* EOF */
