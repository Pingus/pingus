//  $Id: GroundpieceData.cc,v 1.1 2001/08/10 11:05:32 grumbel Exp $
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

#include <list>
#include "editor/EditorGroundpieceObj.hh"

GroundpieceData::Type 
GroundpieceData::string_to_type(const std::string& arg_type) 
{
  if (arg_type == "solid")
    return GroundpieceData::GP_SOLID;
  else if (arg_type == "transparent")    
    return GroundpieceData::GP_TRANSPARENT;
  else if (arg_type == "ground")
    return GroundpieceData::GP_GROUND;
  else if (arg_type == "bridge")
    return GroundpieceData::GP_BRIDGE;
  else if (arg_type == "water")
    return GroundpieceData::GP_WATER;
  else if (arg_type == "lava") 
    return GroundpieceData::GP_LAVA;
  else if (arg_type == "remove") 
    return GroundpieceData::GP_REMOVE;
  else
    {
      std::cout << "GroundpieceData: Unhandled type: " << arg_type << std::endl;
      return GroundpieceData::GP_GROUND;
    }
}


std::string 
GroundpieceData::type_to_string(Type arg_type) 
  {
    switch (arg_type)
      { 
      case GroundpieceData::GP_SOLID:
	return "solid";
      case GroundpieceData::GP_TRANSPARENT:
	return "transparent";
      case GroundpieceData::GP_GROUND:
	return "ground";
      case GroundpieceData::GP_BRIDGE:
	return "bridge";
      case GroundpieceData::GP_WATER:
	return "water";
      case GroundpieceData::GP_LAVA:
	return "lava";
      case GroundpieceData::GP_REMOVE:
	return "remove";
      default:
	std::cout << "GroundpieceData: Unhandled type: " << arg_type << std::endl;
	return "ground";
      }
  }


std::list<boost::shared_ptr<EditorObj> >
GroundpieceData::create_EditorObj()
{
  std::list<boost::shared_ptr<EditorObj> > lst;
  lst.push_back(boost::shared_ptr<EditorObj>(new EditorGroundpieceObj(*this)));
  return lst;
}


/* EOF */
