//  $Id: thunderstorm_background.cxx,v 1.5 2003/02/19 09:50:37 grumbel Exp $
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

#include "../pingus_resource.hxx"
#include "../worldobjsdata/thunderstorm_background_data.hxx"
#include "../gui/graphic_context.hxx"
#include "thunderstorm_background.hxx"

namespace WorldObjs {

ThunderstormBackground::ThunderstormBackground (const WorldObjsData::ThunderstormBackgroundData& data_)
  : data(new WorldObjsData::ThunderstormBackgroundData(data_)),
    clouds_sur(PingusResource::load_surface("Textures/thunderstorm", "textures")),
    x_pos(0)
{
  // flash_sur.push_back(PingusResource::load_surface("flash1", "thunderstorm"));
  // flash_sur.push_back(PingusResource::load_surface("flash2", "thunderstorm"));
}

ThunderstormBackground::~ThunderstormBackground ()
{
  delete data;
}

float
ThunderstormBackground::get_z_pos () const
{
  return data->pos.z;
}

void
ThunderstormBackground::update ()
{
  ++x_pos;
  if (x_pos >= static_cast<int>(clouds_sur.get_width()))
    x_pos = 0;
}

void
ThunderstormBackground::draw (GraphicContext& gc)
{
  gc.draw(clouds_sur, x_pos, 0);
}

} // namespace WorldObjs

/* EOF */
