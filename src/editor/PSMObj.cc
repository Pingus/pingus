//  $Id: PSMObj.cc,v 1.17 2000/07/30 01:47:37 grumbel Exp $
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

PSMObj::PSMObj(SurfaceData data)
{
  pos = data.pos;
  type = data.type;
  desc = data.desc;

  surf = PingusResource::load_surface(data.desc);
}

PSMObj::PSMObj(const PSMObj& t)
{
  pos.x_pos = t.pos.x_pos + 5;
  pos.y_pos = t.pos.y_pos + 5;
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
  if (type == SurfaceData::SOLID) {
    (*psm) << "solid : ";
  } else if (type == SurfaceData::GROUND) {
    (*psm) << "ground : ";
  } else if (type == SurfaceData::TRANSPARENT) {
    (*psm) << "transparent : ";
  } else if (type == SurfaceData::BRIDGE) {
    (*psm) << "bridge : ";
  } else {
    std::cout << "Warning: PSMObj: type not set!" << std::endl;
    (*psm) << "ground : ";
  }
  
  (*psm) << "(resource:" << desc.datafile << ")" << desc.res_name << " : " 
	 << pos.x_pos << " : " << pos.y_pos << ";" << std::endl;
}

void PSMObj::save_xml(std::ofstream* xml)
{
  std::string type_str;

  if (type == SurfaceData::SOLID) {
    type_str = "solid";
  } else if (type == SurfaceData::GROUND) {
    type_str = "ground";
  } else if (type == SurfaceData::TRANSPARENT) {
    type_str = "transparent";
  } else if (type == SurfaceData::BRIDGE) {
    type_str = "bridge";
  } else {
    std::cout << "Warning: PSMObj: type not set!" << std::endl;
    type_str = "ground";
  }
  
  (*xml) << "<groundpiece type=\"" << type_str << "\">\n";
  save_desc_xml(xml, desc);
  save_position_xml(xml, pos);

  //  (*xml) << "  <type>" << SurfaceData::type_to_string(type) << "</type>\n";
  (*xml) << "</groundpiece>\n" << std::endl;
}

std::string 
PSMObj::status_line()
{
  // FIXME: This is a potential buffer overrun, to lazy to fix it right now
  char str[1024];  
  std::string type_name;

  switch(type)
    {
    case SurfaceData::SOLID:
      type_name = "solid";
      break;
    case SurfaceData::GROUND:
      type_name = "ground";	
      break;
    case SurfaceData::BRIDGE:
      type_name = "bridge";	
      break;
    case SurfaceData::TRANSPARENT:
      type_name = "transparent";
      break;
    default:
      type_name = "Unknown";
    }

  sprintf (str, "Groundpiece: %s Type: %s",
	   desc.res_name.c_str(), type_name.c_str());

  return std::string(str);
}

/* EOF */
