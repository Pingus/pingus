//  $Id: SurfaceData.hh,v 1.3 2001/05/18 19:17:08 grumbel Exp $
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

#ifndef SURFACEDATA_HH
#define SURFACEDATA_HH

#include <ClanLib/core.h>
#include "ResDescriptor.hh"
#include "Position.hh"

///
class SurfaceData 
{
public:
  ///
  CL_Surface surface;
  ///
  CL_Resource* resource;
  ///
  ResDescriptor desc;
  ///
  Position pos;
  ///
  std::string type_str; 
  ///
  enum Type { SOLID, TRANSPARENT, GROUND, BRIDGE, WATER, LAVA, NOTHING } type; 

  static Type string_to_type(const std::string& arg_type) 
  {
    if (arg_type == "solid")
      return SurfaceData::SOLID;
    else if (arg_type == "transparent")    
      return SurfaceData::TRANSPARENT;
    else if (arg_type == "ground")
      return SurfaceData::GROUND;
    else if (arg_type == "bridge")
      return SurfaceData::BRIDGE;
    else if (arg_type == "water")
      return SurfaceData::WATER;
    else if (arg_type == "lava") 
      return SurfaceData::LAVA;
    else
      {
	std::cout << "SurfaceData: Unhandled type: " << arg_type << std::endl;
	return SurfaceData::GROUND;
      }
  }

  static std::string type_to_string(Type arg_type) 
  {
    switch (arg_type)
      { 
      case SurfaceData::SOLID:
	return "solid";
      case SurfaceData::TRANSPARENT:
	return "transparent";
      case SurfaceData::GROUND:
	return "ground";
      case SurfaceData::BRIDGE:
	return "bridge";
      case SurfaceData::WATER:
	return "water";
      case SurfaceData::LAVA:
	return "lava";
      default:
	std::cout << "SurfaceData: Unhandled type: " << arg_type << std::endl;
	return "ground";
      }
  }
};

/* EOF */

#endif

/* EOF */
