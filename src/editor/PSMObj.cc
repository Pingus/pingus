//  $Id: PSMObj.cc,v 1.23 2000/10/18 20:16:36 grumbel Exp $
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
//#include <ClanLib/gui.h>
#include "../algo.hh"
#include "../globals.hh"
#include "../PingusResource.hh"

#include "PSMObj.hh"

PSMObj::PSMObj(GroundpieceData data)
{
  pos.z_pos = 0;
  pos = data.pos;
  type = data.type;
  desc = data.desc;

  surf = PingusResource::load_surface(data.desc);
  init();
}

PSMObj::PSMObj(const PSMObj& t)
{
  pos.x_pos = t.pos.x_pos + 5;
  pos.y_pos = t.pos.y_pos + 5;
  pos.z_pos = 0;
  surf  = t.surf;
  desc  = t.desc;
  type  = t.type;
  width = t.width;
  height = t.height;
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
  if (type == GroundpieceData::SOLID) {
    (*psm) << "solid : ";
  } else if (type == GroundpieceData::GROUND) {
    (*psm) << "ground : ";
  } else if (type == GroundpieceData::TRANSPARENT) {
    (*psm) << "transparent : ";
  } else if (type == GroundpieceData::BRIDGE) {
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

  if (type == GroundpieceData::SOLID) {
    type_str = "solid";
  } else if (type == GroundpieceData::GROUND) {
    type_str = "ground";
  } else if (type == GroundpieceData::TRANSPARENT) {
    type_str = "transparent";
  } else if (type == GroundpieceData::BRIDGE) {
    type_str = "bridge";
  } else {
    std::cout << "Warning: PSMObj: type not set!" << std::endl;
    type_str = "ground";
  }
  
  (*xml) << "<groundpiece type=\"" << type_str << "\">\n";
  save_desc_xml(xml, desc);
  save_position_xml(xml, pos);

  //  (*xml) << "  <type>" << GroundpieceData::type_to_string(type) << "</type>\n";
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
    case GroundpieceData::SOLID:
      type_name = "solid";
      break;
    case GroundpieceData::GROUND:
      type_name = "ground";	
      break;
    case GroundpieceData::BRIDGE:
      type_name = "bridge";	
      break;
    case GroundpieceData::TRANSPARENT:
      type_name = "transparent";
      break;
    default:
      type_name = "Unknown";
    }

  sprintf (str, "Groundpiece: %s Type: %s",
	   desc.res_name.c_str(), type_name.c_str());

  return std::string(str);
}

void
PSMObj::gui_edit_obj()
{
  /*
  std::cout << "Gui... trala" << std::endl;
  CL_StyleManager*     style;
  CL_ComponentManager* manager;
  CL_GUIManager*       gui;
  CL_ResourceManager*  resources;

  std::cout << "Step..." << std::endl;
  resources = PingusResource::get ("gui");
  std::cout << "Step..." << std::endl;
  style = new CL_StyleManager_Default(resources);
  std::cout << "Step..." << std::endl;
  manager = CL_ComponentManager::create("gui/editor-psm.gui", false, style);
  std::cout << "Step..." << std::endl;
  gui = CL_GUIManager::create(manager);

  std::cout << "Looping start loop" << std::endl;
  while(CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false) 
    {
      std::cout << "Loopy loop" << std::endl;
      CL_Display::clear_display(0.60f, 0.60f, 0.60f);
      gui->show();
      CL_Display::flip_display();       
      CL_System::keep_alive();
    }
  std::cout << "Gui... trala done" << std::endl;
  */
}

/* EOF */
