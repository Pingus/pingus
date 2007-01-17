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
#include "../world.hxx"
#include "../ground_map.hxx"
#include "../col_map.hxx"
#include "../resource.hxx"
#include "groundpiece.hxx"

namespace WorldObjs {

Groundpiece::Groundpiece(const FileReader& reader)
{
  reader.read_vector("position", pos);
  reader.read_desc  ("surface",  desc);
  
  gptype = Groundtype::GP_GROUND;
  reader.read_enum("type", gptype, &Groundtype::string_to_type);

  // FIXME: we don't need to load surfaces here, providers would be
  // FIXME: enough and should be faster
}

void
Groundpiece::on_startup ()
{
  //// FIXME: We discard rotation here!
  CollisionMask surface = Resource::load_collision_mask(desc.res_name);

  // FIXME: overdrawing of bridges and similar things aren't handled
  // FIXME: here
  if (gptype == Groundtype::GP_REMOVE)
    get_world()->remove(surface, static_cast<int>(pos.x), static_cast<int>(pos.y));
  else
    get_world()->put(surface, static_cast<int>(pos.x), static_cast<int>(pos.y), gptype);
}

} // namespace WorldObjs

/* EOF */
