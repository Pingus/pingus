//  $Id: liquid.cxx,v 1.5 2002/09/14 19:06:33 torangan Exp $
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

#include "graphic_context.hxx"
#include "world.hxx"
#include "pingus_resource.hxx"
#include "col_map.hxx"
#include "liquid.hxx"

Liquid::Liquid(const LiquidData& data)
{
  pos = data.pos;  
  speed = data.speed;
  sur = PingusResource::load_surface(data.desc.res_name.c_str(), "global");

  if (data.old_width_handling)
    width = data.width;
  else
    width = data.width * sur.get_width ();

  counter.set_size(sur.get_num_frames());
  counter.set_speed(speed);
}

Liquid::~Liquid()
{
}

void
Liquid::draw_colmap()
{
  CL_Surface sur = PingusResource::load_surface("Liquid/water_cmap", "global");

  for(int i=0; i < width; ++i)
    world->get_colmap()->put(sur, (int)pos.x + i, (int)pos.y, GroundpieceData::GP_WATER);
}

void
Liquid::draw (GraphicContext& gc)
{
  for(int x = static_cast<int>(pos.x); x <= pos.x + width; x += sur.get_width())
    gc.draw(sur, x, static_cast<int>(pos.y), static_cast<int>(counter));
  
  ++counter;
}

/* EOF */
