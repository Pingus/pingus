//  $Id: groundtype.cxx,v 1.3 2003/10/18 23:17:27 grumbel Exp $
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
#include "groundtype.hxx"

namespace Pingus {

Groundtype::GPType
Groundtype::string_to_type(const std::string& arg_type)
{
  if (arg_type == "solid")
    return Groundtype::GP_SOLID;
  else if (arg_type == "transparent")
    return Groundtype::GP_TRANSPARENT;
  else if (arg_type == "ground")
    return Groundtype::GP_GROUND;
  else if (arg_type == "bridge")
    return Groundtype::GP_BRIDGE;
  else if (arg_type == "water")
    return Groundtype::GP_WATER;
  else if (arg_type == "lava")
    return Groundtype::GP_LAVA;
  else if (arg_type == "remove")
    return Groundtype::GP_REMOVE;
  else
    {
      std::cout << "Groundtype: Unhandled type: " << arg_type << std::endl;
      return Groundtype::GP_GROUND;
    }
}


std::string
Groundtype::type_to_string(GPType arg_type)
{
  switch (arg_type)
    {
    case Groundtype::GP_SOLID:
      return "solid";
    case Groundtype::GP_TRANSPARENT:
      return "transparent";
    case Groundtype::GP_GROUND:
      return "ground";
    case Groundtype::GP_BRIDGE:
      return "bridge";
    case Groundtype::GP_WATER:
      return "water";
    case Groundtype::GP_LAVA:
      return "lava";
    case Groundtype::GP_REMOVE:
      return "remove";
    default:
      std::cout << "Groundtype: Unhandled type: " << arg_type << std::endl;
      return "ground";
    }
}

} // namespace Pingus

/* EOF */
