//  $Id: hotspot.cxx,v 1.3 2002/09/04 19:40:19 grumbel Exp $
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
#include "hotspot.hxx"
#include "globals.hxx"
#include "pingus_resource.hxx"

Hotspot::Hotspot(const HotspotData& spot)
{
  if (verbose > 2)
    std::cout << "Creating Hotspot" << std::endl;

  pos   = spot.pos;
  para  = spot.para;
  speed = spot.speed;

  surface = PingusResource::load_surface(spot.desc);
  
  if (speed != -1)
    {
      count.set_size(surface.get_num_frames());
      count.set_speed(speed);
      count = 0;
    }
}

void 
Hotspot::draw (GraphicContext& gc)
{
  gc.draw (surface, pos * para, static_cast<int>(count));
  ++count;
}

/* EOF */
