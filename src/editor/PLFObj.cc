//  $Id: PLFObj.cc,v 1.27 2000/08/04 16:08:40 grumbel Exp $
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
  char str[256];

  sprintf(str, "%s", obj_type().c_str());

  return std::string(str);
}

HotspotObj::HotspotObj(HotspotData data)
{
  pos   = data.pos;
  desc  = data.desc;
  speed = data.speed;
  para  = data.para;
  cout << "Lodaing Hotspot: " << desc.res_name << endl;
  surf = PingusResource::load_surface(desc);
  init();
}

HotspotObj::~HotspotObj()
{
}

EditorObj*
HotspotObj::duplicate()
{
  return new HotspotObj(*this);
}

void
HotspotObj::save(ofstream* plf, ofstream* psm)
{
  (*plf) << "hotspot {\n"
	 << "  image = (resource:" << desc.datafile << ")\"" << desc.res_name << "\";\n"
	 << "  x_pos = " << pos.x_pos << ";\n"
	 << "  y_pos = " << pos.y_pos << ";\n"
	 << "  z_pos = " << pos.z_pos << ";\n"
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
  save_desc_xml(xml, desc);
  save_position_xml(xml, pos);
  (*xml) << "  <speed>" << speed << "</speed>\n"
	 << "  <parallax>" << para << "</parallax>\n"
	 << "</hotspot>\n"
	 << std::endl;  
}

std::string
HotspotObj::status_line()
{
  char str[256];

  sprintf(str, "Hotspot - Speed: %d - X: %d - Y: %d - Z: %d",  speed, pos.x_pos, pos.y_pos, pos.z_pos);

  return std::string(str);
}

EntranceObj::EntranceObj(EntranceData data)
{
  pos  = data.pos;
  type = data.type;
  direction = data.direction;
  release_rate = data.release_rate;
  
  if (type == "generic")
    {
      surf = PingusResource::load_surface("Entrances/generic", "global");
      x_of = -(surf->get_width()/2);
      y_of = -surf->get_height();
    } 
  else if (type == "woodthing") 
    {
      surf = PingusResource::load_surface("Entrances/woodthing_mov", "global");
      cout << "Loading woodthing..." << endl;
      if (!surf) {
	throw PingusError("EntranceObj: Fatal error!");
      }
      // FIXME: This are hardcoded, because the values are incorrectf!?
      x_of = - (int)(surf->get_width())/2;
      y_of = 32 - (int)(surf->get_height());
      cout << "Loading woodthing..." << x_of << " " << y_of << endl;
    } 
  else if (type == "cloud")
    {
      surf = PingusResource::load_surface("Entrances/cloud", "global");
      x_of = -115;
      y_of = -75;
    } 
  else 
    {
      cout << "Entrance obj error!" << endl;
      throw PingusError("EntranceObj: Unknown entrance type: " + type);
    }
  init();
}

EntranceObj::~EntranceObj()
{
}

EditorObj*
EntranceObj::duplicate()
{
  return new EntranceObj(*this);
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
	 << "  x_pos = " << pos.x_pos << ";\n"
	 << "  y_pos = " << pos.y_pos << ";\n"
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
  save_position_xml(xml, pos);
  (*xml) << "  <type>" << type << "</type>\n"
	 << "  <direction>" << dir_str << "</direction>\n"
	 << "  <release-rate>" << release_rate << "</release-rate>\n"
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

  sprintf(str, "Entrance: %s Rate: %d Direction: %s",
	  type.c_str(), release_rate, dir_str.c_str());

  return std::string(str);
}

ExitObj::ExitObj(ExitData data)
{
  pos  = data.pos;
  desc = data.desc;
  surf = PingusResource::load_surface(desc);
  init();
}

ExitObj::~ExitObj()
{
}

EditorObj*   
ExitObj::duplicate()
{
  return new ExitObj(*this);
}

void
ExitObj::save(ofstream* plf, ofstream* psm)
{
  (*plf) << "exit {\n"
	 << "  image = (resource:" << desc.datafile << ")\"" << desc.res_name << "\";\n"
	 << "  x_pos = " << pos.x_pos << ";\n"
	 << "  y_pos = " << pos.y_pos << ";\n"
	 << "  z_pos = " << pos.z_pos << ";\n"
	 << "}\n"
	 << endl;
}

void
ExitObj::save_xml(std::ofstream* xml)
{
  (*xml) << "<exit>\n";
  save_position_xml(xml, pos);
  save_desc_xml(xml, desc);
  (*xml) << "</exit>\n"
	 << std::endl;
}

std::string 
ExitObj::status_line()
{
  char str[256];

  sprintf(str, "Exit - %s - X:%3d Y:%3d Z:%3d",
	  desc.res_name.c_str(),
	  pos.x_pos, pos.y_pos, pos.z_pos);

  return std::string(str);
}

TrapObj::TrapObj(TrapData data)
{
  pos   = data.pos;
  type  = data.type;
  frame = 0;

  if (type == "guillotine") {
    surf = PingusResource::load_surface("Traps/guillotineidle", "traps");
  } else if (type == "fake_exit") {
    surf = PingusResource::load_surface("Traps/fake_exit", "traps");
  } else if (type == "laser_exit") {
    frame = 5;
    pos.z_pos = -100;
    surf = PingusResource::load_surface("Traps/laser_exit", "traps");
  } else if (type == "spike") {
    frame = 5;
    surf = PingusResource::load_surface("Traps/spike", "traps");
    pos.z_pos = 100;
  } else if (type == "hammer") {
    surf = PingusResource::load_surface("Traps/hammer", "traps");    
  } else if (type == "smasher") {
    surf = PingusResource::load_surface("Traps/smasher", "traps");
    pos.z_pos = 100;
  } else if (type == "bumper") {
    surf = PingusResource::load_surface("Traps/bumper", "traps");
    pos.z_pos = -100;
  } else if (type == "teleport") {
    surf = PingusResource::load_surface("Traps/teleporter", "traps");
    pos.z_pos = 100;
  } else {
    throw PingusError(type + ": trap is not implemented in editor");
  }
  init();
}

TrapObj::~TrapObj()
{
}

EditorObj*   
TrapObj::duplicate()
{
  return new TrapObj(*this);
}

void
TrapObj::draw_offset(int x_offset, int y_offset)
{
  if (surf) {
    surf->put_screen(pos.x_pos + x_offset + x_of,
		     pos.y_pos + y_offset + y_of,
		     frame);
  } else {
    EditorObj::draw_offset(x_offset, y_offset);
  }
}

void
TrapObj::save(ofstream* plf, ofstream* psm)
{
  (*plf) << "trap {\n"
	 << "  name = \"" << type << "\";\n"
	 << "  x_pos = " << pos.x_pos << ";\n"
	 << "  y_pos = " << pos.y_pos << ";\n"
	 << "  z_pos = " << pos.z_pos << ";\n"
	 << "}\n"
	 << std::endl;
}

void
TrapObj::save_xml(std::ofstream* xml)
{
  (*xml) << "<trap>\n"
	 << "  <type>" << type << "</type>\n";
  save_position_xml(xml, pos);
  (*xml) << "</trap>\n"
	 << std::endl;
}

LiquidObj::LiquidObj(const LiquidObj& data)
{
  pos = data.pos;
  width = data.width;
  desc  = data.desc;
  speed = data.speed;
  surf  = data.surf;
  counter = data.counter;
  //init();
  height = surf->get_height();
}

LiquidObj::LiquidObj(LiquidData data)
{
  pos = data.pos;
  width = data.width;
  desc  = data.desc;
  speed = data.speed;
  surf = PingusResource::load_surface(desc);
  counter.set_size(surf->get_num_frames());
  counter.set_speed(50);
  init();
}

LiquidObj::~LiquidObj()
{
}

EditorObj* 
LiquidObj::duplicate()
{
  return new LiquidObj(*this);
}

void
LiquidObj::draw_offset(int x_offset, int y_offset)
{
  int x1 = pos.x_pos + x_offset;
  int x2 = pos.x_pos + width + x_offset;
  int y1 = pos.y_pos + y_offset;
  int y2 = pos.y_pos + y_offset + surf->get_height();

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

  for(int x = pos.x_pos; x <= pos.x_pos + width; x += surf->get_width())
    surf->put_screen(x + x_offset, pos.y_pos + y_offset, int(++counter));

  CL_Display::pop_clip_rect();
}

void
LiquidObj::draw_mark_offset(int x_offset, int y_offset) 
{
  Display::draw_rect(pos.x_pos + x_offset,
		     pos.y_pos + y_offset,
		     pos.x_pos + width + x_offset,
		     pos.y_pos + surf->get_height() + y_offset,
		     mark_color.r, 
		     mark_color.g,
		     mark_color.b,
		     mark_color.a);
}

bool
LiquidObj::mouse_over(int x_offset, int y_offset)
{
  int height = surf->get_height();
  int mouse_x = CL_Mouse::get_x();
  int mouse_y = CL_Mouse::get_y();  

  if (   mouse_x > pos.x_pos + x_offset 
      && mouse_x < pos.x_pos + width + x_offset
      && mouse_y > pos.y_pos + y_offset 
      && mouse_y < pos.y_pos + height + y_offset)
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
	 << "  x_pos = " << pos.x_pos << ";\n"
	 << "  y_pos = " << pos.y_pos << ";\n"
	 << "  z_pos = " << pos.z_pos << ";\n"
	 << "  width = " << width << ";\n"
    	 << "  speed = " << speed << ";\n"
	 << "}\n" 
	 << endl;
}

void
LiquidObj::save_xml(std::ofstream* xml)
{
  (*xml) << "<liquid>";
  save_desc_xml(xml, desc);
  save_position_xml(xml, pos);
  (*xml) << "  <width>" << width << "</width>\n"
	 << "  <speed>" << speed << "</speed>\n"
	 << "</liquid>\n" << std::endl;
}

std::string  
LiquidObj::status_line()
{
  char str[256];

  sprintf(str, "%4d:%4d:%3d:%2d", pos.x_pos, pos.y_pos, pos.z_pos, speed);

  return std::string(str);
}

/* EOF */

