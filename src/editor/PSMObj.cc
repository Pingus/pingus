//  $Id: PSMObj.cc,v 1.9 2000/05/28 19:30:10 grumbel Exp $
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

#include <fstream>
#include <ClanLib/core.h>

#include "../PingusResource.hh"

#include "PSMObj.hh"

PSMObj::PSMObj(surface_data data)
{
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  type  = data.type;

  surf = CL_Surface::load(data.name.c_str(), PingusResource::get("global.dat"));
  desc.res_name = data.name;
}

PSMObj::PSMObj(const PSMObj& t)
{
  x_pos = t.x_pos + 5;
  y_pos = t.y_pos + 5;
  surf  = t.surf;
  desc  = t.desc;
  type  = t.type;
}

PSMObj::~PSMObj()
{
}

EditorObj*   
PSMObj::duplicate()
{
  return new PSMObj(*this);
}

void
PSMObj::save(std::ofstream* plf, std::ofstream* psm)
{
  if (type == surface_data::SOLID) {
    (*psm) << "solid : ";
  } else if (type == surface_data::GROUND) {
    (*psm) << "ground : ";
  } else {
    std::cout << "Warning: PSMObj: type not set!" << std::endl;
    (*psm) << "ground : ";
  }
  
  (*psm) << "(resource:" << desc.filename << ")" << desc.res_name << " : " 
	 << x_pos << " : " << y_pos << ";" << std::endl;
}

std::string 
PSMObj::status_line()
{
  // FIXME: This is a potential buffer overrun, to lazy to fix it right now
  char str[1024];  
  string type_name;

  switch(type)
    {
    case surface_data::SOLID:
      type_name = "solid";
      break;
    case surface_data::GROUND:
      type_name = "ground";	
      break;
    case surface_data::BRIDGE:
      type_name = "bridge";	
      break;
    default:
      type_name = "Unknown";
    }

  sprintf (str, "Groundpiece: %s Type: %s",
	   desc.res_name.c_str(), type_name.c_str());

  return std::string(str);
}

/* EOF */
