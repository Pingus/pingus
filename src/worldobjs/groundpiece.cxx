//  $Id: groundpiece.cxx,v 1.1 2002/09/16 19:18:56 grumbel Exp $
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
#include "groundpiece.hxx"

namespace WorldObjs {

Groundpiece::Groundpiece(const GroundpieceData& data_)
  : data(data_),
    surface (PingusResource::load_surface(data.desc))
{
  // FIXME: we don't need to load surfaces here, providers would be
  // FIXME: enough and should be faster
}

void
Groundpiece::on_startup()
{
  // FIXME: overdrawing of bridges and similar things aren't handled
  // FIXME: here
  if (data.gptype == GroundpieceData::GP_REMOVE)
    get_world()->get_gfx_map()->remove(surface, int(data.pos.x), int(data.pos.y));
  else
    get_world()->get_gfx_map()->put(surface, int(data.pos.x), int(data.pos.y));

  if (data.gptype == GroundpieceData::GP_REMOVE)
    get_world()->get_colmap()->remove(surface, int(data.pos.x), int(data.pos.y));
  else
    get_world()->get_colmap()->put(surface, int(data.pos.x), int(data.pos.y),
				   data.gptype);
}

} // namespace WorldObjs

/* EOF */
