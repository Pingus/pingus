//  $Id: PLFObj.cc,v 1.45 2001/07/24 21:39:46 grumbel Exp $
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
#include <cstdio>

#include "../XMLhelper.hh"
#include "../Display.hh"
#include "../PingusError.hh"
#include "../PingusResource.hh"
#include "PLFObj.hh"

using namespace std;

PLFObj::PLFObj()
{
  mark_color.r = 1.0;
  mark_color.g = 0.0;
  mark_color.b = 0.0;
  mark_color.a = 1.0;
}

PLFObj::~PLFObj()
{
}

std::string
PLFObj::status_line()
{
  return std::string("--- unsupported object ---");
}

HotspotObj::HotspotObj(const HotspotData& data)
  : HotspotData (data)
{
  *position = data.pos;
  surf = PingusResource::load_surface(desc);
  width = surf.get_width ();
  height = surf.get_height ();
}

HotspotObj::~HotspotObj()
{
}

boost::shared_ptr<EditorObj>
HotspotObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new HotspotObj(*this));
}

void
HotspotObj::save(ofstream* plf, ofstream* psm)
{
  (*plf) << "hotspot {\n"
	 << "  image = (resource:" << desc.datafile << ")\"" << desc.res_name << "\";\n"
	 << "  x_pos = " << position->x << ";\n"
	 << "  y_pos = " << position->y << ";\n"
	 << "  z_pos = " << position->z << ";\n"
    	 << "  speed = " << speed << ";\n"
	 << "  para = \"" << para << "\";\n"
    //	 << "  para_y = \"" << para.y << "\";\n"
	 << "}\n"
	 << endl;  
}

void
HotspotObj::save_xml(ofstream* xml)
{
  (*xml) << "<hotspot>\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_position_xml(xml, *position);
  (*xml) << "  <speed>" << speed << "</speed>\n"
	 << "  <parallax>" << para << "</parallax>\n"
	 << "</hotspot>\n"
	 << std::endl;  
}

std::string
HotspotObj::status_line()
{
  char str[256];

  sprintf(str, "Hotspot - Speed: %d - X: %.2f - Y: %.2f - Z: %.2f",  speed, position->x, position->y, position->z);

  return std::string(str);
}

EntranceObj::EntranceObj(const EntranceData& data)
  : EntranceData (data)
{
  *position  = data.pos;

  if (type == "generic")
    {
      surf = PingusResource::load_surface("Entrances/generic", "entrances");
      x_of = -(surf.get_width()/2);
      y_of = -surf.get_height();
    } 
  else if (type == "woodthing") 
    {
      surf = PingusResource::load_surface("Entrances/woodthing_mov", "entrances");
      cout << "Loading woodthing..." << endl;
      if (!surf) {
	throw PingusError("EntranceObj: Fatal error!");
      }
      // FIXME: This are hardcoded, because the values are incorrectf!?
      x_of = - (int)(surf.get_width())/2;
      y_of = 32 - (int)(surf.get_height());
      cout << "Loading woodthing..." << x_of << " " << y_of << endl;
    } 
  else if (type == "cloud")
    {
      surf = PingusResource::load_surface("Entrances/cloud", "entrances");
      x_of = -115;
      y_of = -75;
    } 
  else 
    {
      cout << "Entrance obj error!" << endl;
      throw PingusError("EntranceObj: Unknown entrance type: " + type);
    }

  width = surf.get_width ();
  height = surf.get_height ();
}

EntranceObj::~EntranceObj()
{
}

boost::shared_ptr<EditorObj>
EntranceObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new EntranceObj(*this));
}

void
EntranceObj::save(ofstream* plf, ofstream* psm)
{
  std::string dir_str = "not set - this is a bug";

  switch(direction)
    {
    case EntranceData::LEFT:
      dir_str = "left";
      break;
    case EntranceData::RIGHT:
      dir_str = "right";
      break;
    case EntranceData::MISC:
      dir_str = "misc";
      break;
    }

  (*plf) << "entrance {\n"
	 << "  type  = " << type << ";\n"
	 << "  x_pos = " << position->x << ";\n"
	 << "  y_pos = " << position->y << ";\n"
	 << "  z_pos = 0;\n"
	 << "  release_rate = " << release_rate << ";\n"
	 << "  direction = " << dir_str << ";\n"
	 << "}\n"
	 << endl;
}

void
EntranceObj::save_xml(ofstream* xml)
{
  std::string dir_str = "not set - this is a bug";

  switch(direction)
    {
    case EntranceData::LEFT:
      dir_str = "left";
      break;
    case EntranceData::RIGHT:
      dir_str = "right";
      break;
    case EntranceData::MISC:
    default:
      dir_str = "misc";
      break;
    }

  (*xml) << "<entrance>\n";
  XMLhelper::write_position_xml(xml, *position);
  (*xml) << "  <type>" << type << "</type>\n"
	 << "  <direction>" << dir_str << "</direction>\n"
	 << "  <release-rate>" << release_rate << "</release-rate>\n"
	 << "  <owner-id>" << owner_id << "</owner-id>\n"
	 << "</entrance>\n"
	 << std::endl;  
}

std::string
EntranceObj::status_line()
{
  std::string dir_str = "not set - this is a bug";
  char str[256];

  switch(direction)
    {
    case EntranceData::LEFT:
      dir_str = "left";
      break;
    case EntranceData::RIGHT:
      dir_str = "right";
      break;
    case EntranceData::MISC:
      dir_str = "misc";
      break;
    }

  sprintf(str, "Entrance: %s Rate: %d Direction: %s Owner: %d",
	  type.c_str(), release_rate, dir_str.c_str(), owner_id);

  return std::string(str);
}

ExitObj::ExitObj(const ExitData& data)
  : ExitData (data)
{
  surf = PingusResource::load_surface(desc);
  width = surf.get_width ();
  height = surf.get_height ();
  *position  = data.pos;

  if (!use_old_pos_handling)
    {
      position->x -= surf.get_width ()/2;
      position->y -= surf.get_height ();
    }
}

ExitObj::~ExitObj()
{
}

boost::shared_ptr<EditorObj>   
ExitObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new ExitObj(*this));
}

void
ExitObj::save(ofstream* plf, ofstream* psm)
{
  (*plf) << "exit {\n"
	 << "  image = (resource:" << desc.datafile << ")\"" << desc.res_name << "\";\n"
	 << "  x_pos = " << position->x << ";\n"
	 << "  y_pos = " << position->y << ";\n"
	 << "  z_pos = " << position->z << ";\n"
	 << "}\n"
	 << endl;
}

void
ExitObj::save_xml(std::ofstream* xml)
{
  (*xml) << "<exit use-old-pos-handling=\"0\">\n";

  Position pos = *position;
  pos.x_pos += surf.get_width ()/2;
  pos.y_pos += surf.get_height ();
  XMLhelper::write_position_xml(xml, pos);
  
  XMLhelper::write_desc_xml(xml, desc);
  (*xml) << "  <owner-id>" << owner_id << "</owner-id>"
	 << "</exit>\n"
	 << std::endl;
}

std::string 
ExitObj::status_line()
{
  char str[256];

  sprintf(str, "Exit - %s - X:%4.2f Y:%4.2f Z:%4.2f OwnerId: %d",
	  desc.res_name.c_str(),
	  position->x, position->y, position->z, owner_id);

  return std::string(str);
}

TrapObj::TrapObj(const TrapData& data)
  : TrapData (data)
{
  *position = data.pos;
  frame = 0;

  if (type == "guillotine") {
    surf = PingusResource::load_surface("Traps/guillotineidle", "traps");
  } else if (type == "fake_exit") {
    surf = PingusResource::load_surface("Traps/fake_exit", "traps");
  } else if (type == "laser_exit") {
    frame = 5;
    position->z = -100;
    surf = PingusResource::load_surface("Traps/laser_exit", "traps");
  } else if (type == "spike") {
    frame = 5;
    surf = PingusResource::load_surface("Traps/spike", "traps");
    position->z = 100;
  } else if (type == "hammer") {
    surf = PingusResource::load_surface("Traps/hammer", "traps");    
  } else if (type == "smasher") {
    surf = PingusResource::load_surface("Traps/smasher", "traps");
    position->z = 100;
  } else if (type == "bumper") {
    surf = PingusResource::load_surface("Traps/bumper", "traps");
    position->z = -100;
  } else if (type == "teleport") {
    surf = PingusResource::load_surface("Traps/teleporter", "traps");
    position->z = 100;
  } else {
    throw PingusError("'" + type + "': trap is not implemented in editor");
  }
  width = surf.get_width ();
  height = surf.get_height ();
}

TrapObj::~TrapObj()
{
}

boost::shared_ptr<EditorObj>
TrapObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new TrapObj(*this));
}

void
TrapObj::draw (boost::dummy_ptr<EditorView> view)
{
  if (surf) {
    view->draw(surf, *position, frame);
  } else {
    EditorObj::draw(view);
  }
}

std::string  
TrapObj::status_line()
{
  char str[1024];
  sprintf (str, "TrapObj: %4.2fx%4.2fx%4.2f", position->x, position->y, position->z);
  return str;
}

void
TrapObj::save(ofstream* plf, ofstream* psm)
{
  (*plf) << "trap {\n"
	 << "  name = \"" << type << "\";\n"
	 << "  x_pos = " << position->x << ";\n"
	 << "  y_pos = " << position->y << ";\n"
	 << "  z_pos = " << position->z << ";\n"
	 << "}\n"
	 << std::endl;
}

void
TrapObj::save_xml(std::ofstream* xml)
{
  // FIXME: Move this to trap data!
  (*xml) << "<trap>\n"
	 << "  <type>" << type << "</type>\n";
  XMLhelper::write_position_xml(xml, *position);
  (*xml) << "</trap>\n"
	 << std::endl;
}

LiquidObj::LiquidObj(const LiquidObj& data)
  : LiquidData (data)
{
  *position = data.pos;
  surf = data.surf;
  EditorObj::width = surf.get_width ();
  EditorObj::height = surf.get_height ();
}

LiquidObj::LiquidObj(const LiquidData& data)
  : LiquidData (data)
{
  *position = data.pos;
  surf = PingusResource::load_surface(desc);
  EditorObj::width = surf.get_width ();
  EditorObj::height = surf.get_height ();
}

LiquidObj::~LiquidObj()
{
}

boost::shared_ptr<EditorObj>
LiquidObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new LiquidObj(*this));
}

void
LiquidObj::draw (boost::dummy_ptr<EditorView> view)
{
  std::cout << "Liquid Drawing unfinished and buggy" << std::endl;
  int x1 = int(position->x);
  int x2 = int(position->x + LiquidData::width);
  int y1 = int(position->y);
  int y2 = int(position->y + surf.get_height());

  if (x1 < 0) {
    x1 = 0;
    if (x2 < 0)
      x2 = 0;
  }

  if (y1 < 0) {
    y1 = 0;
    if (y2 < 0)
      y2 = 0;
  }

  if (x2 >= CL_Display::get_width()) {
    x2 = CL_Display::get_width() - 1;
    if (x1 >= CL_Display::get_width())
      x1 = x2;
  }

  if (y2 >= CL_Display::get_height()) {
    y2 = CL_Display::get_height();
    if (y1 >= CL_Display::get_height())
      y1 = y2;
  }

  CL_Display::push_clip_rect();
  CL_Display::set_clip_rect(CL_ClipRect(x1, y1, x2, y2));

  for(float x = position->x; x <= position->x + LiquidData::width; x += surf.get_width())
    surf.put_screen(int(x), int(position->y));

  CL_Display::pop_clip_rect();
}

void
LiquidObj::draw_mark (boost::dummy_ptr<EditorView> view) 
{
  view->draw_rect(int(position->x),
		  int(position->y),
		  int(position->x + LiquidData::width),
		  int(position->y + surf.get_height()),
		  mark_color.r, 
		  mark_color.g,
		  mark_color.b,
		  mark_color.a);
}

bool
LiquidObj::mouse_over(int x_offset, int y_offset)
{
  int height = surf.get_height();
  int mouse_x = CL_Mouse::get_x();
  int mouse_y = CL_Mouse::get_y();  

  if (   mouse_x > position->x + x_offset 
      && mouse_x < position->x + LiquidData::width + x_offset
      && mouse_y > position->y + y_offset 
      && mouse_y < position->y + height + y_offset)
    {
      return true;
    }
  else 
    {
      return false;
    }
}

void
LiquidObj::save(ofstream* plf, ofstream* psm)
{
  (*plf) << "liquid {\n"
	 << "  image = (resource:" << desc.datafile << ")\"" << desc.res_name << "\";\n"
	 << "  x_pos = " << position->x << ";\n"
	 << "  y_pos = " << position->y << ";\n"
	 << "  z_pos = " << position->z << ";\n"
	 << "  width = " << LiquidData::width << ";\n"
    	 << "  speed = " << speed << ";\n"
	 << "}\n" 
	 << endl;
}

void
LiquidObj::save_xml(std::ofstream* xml)
{
  (*xml) << "<liquid>\n";
  XMLhelper::write_desc_xml(xml, desc);
  XMLhelper::write_position_xml(xml, *position);
  (*xml) << "  <width>" << LiquidData::width << "</width>\n"
	 << "  <speed>" << speed << "</speed>\n"
	 << "</liquid>\n" << std::endl;
}

std::string  
LiquidObj::status_line()
{
  char str[256];

  sprintf(str, "%4.2f:%4.2f:%3.2f:%2d", position->x, position->y, position->z, speed);

  return std::string(str);
}

/* EOF */

