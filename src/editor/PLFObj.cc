//  $Id: PLFObj.cc,v 1.52 2001/08/12 23:05:22 grumbel Exp $
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
#include "EditorView.hh"
#include "PLFObj.hh"

using namespace std;

EntranceObj::EntranceObj(const EntranceData& data)
  : SpriteEditorObj (pos),
    EntranceData (data)
{
  std::cout << "EntranceObj::EntranceObj(const EntranceData& data): " 
	    << type << std::endl;

  if (type == "generic")
    {
      sprite = Sprite("Entrances/generic", "entrances");
      sprite.set_align(-(sprite.get_width()/2), -sprite.get_height());
    } 
  else if (type == "woodthing") 
    {
      std::cout << "WOODTHING" << std::endl;
      sprite = Sprite("Entrances/woodthing_mov", "entrances");
      sprite.set_align(0  - sprite.get_width()/2,
		       32 - sprite.get_height());
    }
  else if (type == "cloud")
    {
      sprite = Sprite("Entrances/cloud", "entrances");
      sprite.set_align(-115, -75);
    } 
  else 
    {
      cout << "Entrance obj error!" << endl;
      throw PingusError("EntranceObj: Unknown entrance type: " + type);
    }
}

boost::shared_ptr<EditorObj>
EntranceObj::duplicate()
{
  std::cout << "EntranceObj::duplicate()" << std::endl;
  boost::shared_ptr<EditorObj> entrance (new EntranceObj(*this));

  return entrance;
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
  : SpriteEditorObj (data.desc.res_name, data.desc.datafile, pos),
    ExitData (data)
{
  if (!use_old_pos_handling)
    {
      pos.x -= sprite.get_width ()/2;
      pos.y -= sprite.get_height ();
    }
}

boost::shared_ptr<EditorObj>   
ExitObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new ExitObj(*this));
}

std::string 
ExitObj::status_line()
{
  char str[256] = {};
  
  sprintf(str, "Exit - %s - X:%4.2f Y:%4.2f Z:%4.2f OwnerId: %d",
	  desc.res_name.c_str(),
	  pos.x, pos.y, pos.z, owner_id);

  return std::string("unset");
}

TrapObj::TrapObj(const TrapData& data)
  : SpriteEditorObj ("", "", pos),
    TrapData (data)
{/*
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
  height = surf.get_height ();*/
}

boost::shared_ptr<EditorObj>
TrapObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new TrapObj(*this));
}

void
TrapObj::draw (boost::dummy_ptr<EditorView> view)
{
  /*  if (surf) {
    view->draw(surf, *position, frame);
  } else {
    EditorObj::draw(view);
    }*/
}

std::string  
TrapObj::status_line()
{
  char str[1024];
  sprintf (str, "TrapObj: %4.2fx%4.2fx%4.2f", pos.x, pos.y, pos.z);
  return str;
}

LiquidObj::LiquidObj(const LiquidData& data)
  : SpriteEditorObj (data.desc.res_name, data.desc.datafile, pos),
    LiquidData (data)
{
  if (old_width_handling)
    {
      width = (width + sprite.get_width ()) / sprite.get_width ();
      old_width_handling = false;
    }
}

boost::shared_ptr<EditorObj>
LiquidObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new LiquidObj(*this));
}

void
LiquidObj::draw (boost::dummy_ptr<EditorView> view)
{
  for(int i = 0; i < width; i++)
    view->draw (sprite, pos + CL_Vector (i * sprite.get_width (), 0));
}

void
LiquidObj::draw_mark (boost::dummy_ptr<EditorView> view) 
{
  view->draw_rect(int(pos.x),
		  int(pos.y),
		  int(pos.x + sprite.get_width () * width),
		  int(pos.y + sprite.get_height()),
		  1.0, 1.0, 1.0, 0.0);
}

std::string  
LiquidObj::status_line()
{
  char str[256];
  
  sprintf(str, "%4.2f:%4.2f:%3.2f:%2d", pos.x, pos.y, pos.z, speed);

  return std::string(str);
}

void 
LiquidObj::make_larger ()
{
  LiquidData::width += 1;
}

void 
LiquidObj::make_smaller ()
{
  if (LiquidData::width > 1)
    LiquidData::width -= 1;
}

/* EOF */
