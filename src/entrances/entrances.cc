//  $Id: entrances.cc,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

#include "../Entrance.hh"
#include "../PingusError.hh"

#include "WoodThing.hh"

Entrance*
get_entrance(entrance_data data)
{
  if (data.type == "generic") {
    return new Entrance(data);
  } else if (data.type == "woodthing") {
    return new WoodThing(data);
  } else {
    throw PingusError("Entrance: Entrane type in PLF file is unknow: " + data.type);
  }
}

/* EOF */
