//  $Id: hotspot.cxx,v 1.9 2003/10/22 12:35:47 grumbel Exp $
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
#include "../worldobjsdata/hotspot_data.hxx"
#include "../globals.hxx"
#include "../gui/graphic_context.hxx"
#include "../resource.hxx"
#include "hotspot.hxx"

namespace Pingus {
namespace WorldObjs {

Hotspot::Hotspot (const WorldObjsData::HotspotData& data_)
  : data(new WorldObjsData::HotspotData(data_)),
    sprite(Resource::load_sprite(data->desc))
{
  if (verbose > 2)
    std::cout << "Creating Hotspot" << std::endl;
}

Hotspot::~Hotspot ()
{
  delete data;
}

void
Hotspot::update()
{
  sprite.update();
}

void
Hotspot::draw (GraphicContext& gc)
{
  // FIXME: para support doesnn't work correctly
  gc.draw (sprite, data->pos);
}

float
Hotspot::get_z_pos () const
{
  return data->pos.z;
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
