//  $Id: groundpiece.cxx,v 1.5 2003/10/20 13:11:09 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "../world.hxx"
#include "../pingu_map.hxx"
#include "../col_map.hxx"
#include "../pingus_resource.hxx"
#include "../worldobjsdata/groundpiece_data.hxx"
#include "groundpiece.hxx"

namespace Pingus {
namespace WorldObjs {

Groundpiece::Groundpiece (const WorldObjsData::GroundpieceData& data_)
  : data(new WorldObjsData::GroundpieceData(data_)),
    surface (PingusResource::load_sprite(data->desc))
{
  // FIXME: we don't need to load surfaces here, providers would be
  // FIXME: enough and should be faster
}

Groundpiece::~Groundpiece ()
{
  delete data;
}

void
Groundpiece::on_startup ()
{
  CL_PixelBuffer surface = PingusResource::load_surface_provider(data->desc);

  // FIXME: overdrawing of bridges and similar things aren't handled
  // FIXME: here
  if (data->gptype == Groundtype::GP_REMOVE)
    get_world()->get_gfx_map()->remove(surface, static_cast<int>(data->pos.x), static_cast<int>(data->pos.y));
  else
    get_world()->get_gfx_map()->put(surface, static_cast<int>(data->pos.x), static_cast<int>(data->pos.y));

  if (data->gptype == Groundtype::GP_REMOVE)
    get_world()->get_colmap()->remove(surface, static_cast<int>(data->pos.x), static_cast<int>(data->pos.y));
  else
    get_world()->get_colmap()->put(surface, static_cast<int>(data->pos.x), static_cast<int>(data->pos.y), data->gptype);
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
