//  $Id: EditorGroundpieceObj.cc,v 1.3 2001/08/04 12:46:22 grumbel Exp $
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
#include "../XMLhelper.hh"
#include "../algo.hh"
#include "../globals.hh"
#include "../PingusResource.hh"

#include "EditorGroundpieceObj.hh"

EditorGroundpieceObj::EditorGroundpieceObj(const GroundpieceData& data)
  : GroundpieceData (data)
{
  *position = data.pos;

  surf = PingusResource::load_surface(data.desc);
  width = surf.get_width ();
  height = surf.get_height ();
}

EditorGroundpieceObj::EditorGroundpieceObj(const EditorGroundpieceObj& data)
  : GroundpieceData (data)
{
  *position = *data.position;
  
  surf  = data.surf;
  type  = data.type;
  width = data.width;
  height = data.height;
}

EditorGroundpieceObj::~EditorGroundpieceObj()
{
}

boost::shared_ptr<EditorObj>   
EditorGroundpieceObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new EditorGroundpieceObj(*this));
}

void   
EditorGroundpieceObj::draw (boost::dummy_ptr<EditorView> view)
{
  EditorObj::draw (view);
  /*CL_Display::fill_rect (offset.x, offset.y,
			 offset.x + get_width (), offset.y + get_height (),
			 0.0, 0.0, 0.0, 0.5);*/
}

void
EditorGroundpieceObj::save(std::ofstream* plf, std::ofstream* psm)
{
  if (type == GroundpieceData::GP_SOLID) {
    (*psm) << "solid : ";
  } else if (type == GroundpieceData::GP_GROUND) {
    (*psm) << "ground : ";
  } else if (type == GroundpieceData::GP_TRANSPARENT) {
    (*psm) << "transparent : ";
  } else if (type == GroundpieceData::GP_BRIDGE) {
    (*psm) << "bridge : ";
  } else {
    std::cout << "Warning: EditorGroundpieceObj: type not set!" << std::endl;
    (*psm) << "ground : ";
  }
  
  (*psm) << "(resource:" << desc.datafile << ")" << desc.res_name << " : " 
	 << position->x << " : " << position->y << ";" << std::endl;
}

void EditorGroundpieceObj::save_xml(std::ofstream* xml)
{
  (*xml) << "<groundpiece type=\"" << GroundpieceData::type_to_string(type) << "\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_vector_xml(xml, *position);
  (*xml) << "</groundpiece>\n" << std::endl;
}

std::string 
EditorGroundpieceObj::status_line()
{
  // FIXME: This is a potential buffer overrun, to lazy to fix it right now
  char str[1024];  
  std::string type_name = GroundpieceData::type_to_string(type);
  
  sprintf (str, "Groundpiece: %s Type: %s",
	   desc.res_name.c_str(), type_name.c_str());

  return std::string(str);
}

void
EditorGroundpieceObj::gui_edit_obj()
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
