//  $Id: cloud.cxx,v 1.2 2003/04/19 10:23:19 torangan Exp $
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

#include "../../pingus_resource.hxx"
#include "../../worldobjsdata/entrance_data.hxx"
#include "cloud.hxx"

namespace WorldObjs {
namespace Entrances {

Cloud::Cloud (const WorldObjsData::EntranceData& data_)
  : Entrance(data_)
{
  data->pos.z = 100;

  surface = PingusResource::load_surface("Entrances/cloud","entrances");
}

void
Cloud::draw_offset (int x, int y, float s)
{
  if (s == 1.0) {
    surface.put_screen(static_cast<int>(x + data->pos.x - 115),
		       static_cast<int>(y + data->pos.y - 100));
  } else {
    surface.put_screen(static_cast<int>((data->pos.x - 32 + x) * s),
		       static_cast<int>((data->pos.y - 16 + y) * s),
		       s, s);
  }
}

} // namespace Entrances
} // namespace WorldObjs

/* EOF */
