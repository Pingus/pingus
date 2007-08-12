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

#include <iostream>
#include "../world.hpp"
#include "../ground_map.hpp"
#include "../col_map.hpp"
#include "../resource.hpp"
#include "groundpiece.hpp"

namespace WorldObjs {

Groundpiece::Groundpiece(const FileReader& reader)
{
  reader.read_vector("position", pos);
  reader.read_desc  ("surface",  desc);
  
  gptype = Groundtype::GP_GROUND;
  reader.read_enum("type", gptype, &Groundtype::string_to_type);
}

void
Groundpiece::on_startup ()
{
  // FIXME: using a CollisionMask is kind of unneeded here 
  CollisionMask mask = Resource::load_collision_mask(desc);

  // FIXME: overdrawing of bridges and similar things aren't handled here
  if (gptype == Groundtype::GP_REMOVE)
    get_world()->remove(mask, static_cast<int>(pos.x), static_cast<int>(pos.y));
  else
    get_world()->put(mask, static_cast<int>(pos.x), static_cast<int>(pos.y), gptype);
}

} // namespace WorldObjs

/* EOF */
