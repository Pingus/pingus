//  $Id: PLF.cc,v 1.8 2000/04/10 21:33:06 grumbel Exp $
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

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>

#include "PLF.hh"
#include "algo.hh"
#include "globals.hh"
#include "PingusError.hh"

int    str_to_int(const std::string& str);
float str_to_float(const std::string& str);

// Create a PLF object and start parsing the given file.
PLF::PLF(std::string filename)
{
  x_pos = y_pos = 0;
  width = 0; height = 0;
  pingus = 100;
  max_time = 9000;
  number_to_save = 0;
  pingus = 100;
  difficulty = 5;
  music = ResDescriptor("file", "pingus-4.it");

  col = ResDescriptor("auto", "-Auto ColMap-");
  if (verbose > 1) std::cout << "Starting Parsing" << std::endl;
  init(filename);
}

// Destroy all data
PLF::~PLF()
{
}

void
PLF::set_value(std::string valueid,
	       std::string cast,
	       std::string value)
{
  switch(current_group) {

  case PLF::GLOBAL:
    if (valueid == "start_x_pos") {
      x_pos = str_to_int(value);
    } else if (valueid == "start_y_pos") {
      y_pos = str_to_int(value);
    } else if (valueid == "number_of_pingus") {
      pingus = str_to_int(value);
    } else if (valueid == "number_to_save") {
      number_to_save = str_to_int(value);
    } else if (valueid == "levelname") {
      levelname = value;
    } else if (valueid == "description") {
      description = value;
    } else if (valueid == "time") {
      max_time = str_to_int(value);
    } else if (valueid == "difficulty") {
      difficulty = str_to_int(value);
    } else if (valueid == "author") {
      author = value;
    } else {
      throw PingusError("Global: Unknown ValueID: " + valueid);
    }
    break;

  case PLF::BACKGROUND:
    if (valueid == "image") 
      {
	bg.desc = ResDescriptor(cast, value);
      } 
    else if (valueid == "scroll_x") 
      {
	bg.scroll_x  = str_to_float(value);
      } 
    else if (valueid == "scroll_y") 
      {
	bg.scroll_y = str_to_float(value);
      } 
    else if (valueid == "para_x")
      {
	bg.para_x = str_to_float(value);
      }
    else if (valueid == "para_y")
      {
	bg.para_y = str_to_float(value);
      }
    else if (valueid == "stretch_x")
      {
	bg.stretch_x = str_to_bool(value);
      }
    else if (valueid == "stretch_y")
      {
	bg.stretch_y = str_to_bool(value);
      }
    else if (valueid == "dim") 
      {
	bg.dim = str_to_float(value);
      } 
    else if (valueid == "red") 
      {
	bg.red = str_to_float(value);	
      } 
    else if (valueid == "green") 
      {
	bg.green = str_to_float(value);	
      } 
    else if (valueid == "blue") 
      {
	bg.blue = str_to_float(value);	
      } 
    else 
      {
	throw PingusError("Background: Unknown ValueID: " + valueid);
      }
    break;

  case PLF::GROUND:
    if (valueid == "maptype") {
      if (value == "BMP") {
	maptype = BMP;
      } else if (value == "SPOT") {
	maptype = SPOT;
      } else if (value == "RANDOM") {
	maptype = RANDOM;
      } else {
	maptype = UNDEF;
      }
    } else if (valueid == "data") {
      fg = ResDescriptor(cast, value);
    } else if (valueid == "colmap") {
      col = ResDescriptor(cast, value);
    } else if (valueid == "width") {
      width = str_to_int(value);
    } else if (valueid == "height") {    
      height = str_to_int(value);
    } else {
      throw PingusError("Ground: Unknown ValueID: " + valueid);
    }
    break;

  case PLF::MUSIC:
    if (valueid == "data")
      music = ResDescriptor(cast, value);
    else
      throw PingusError("Music: Unknown ValueID: " + valueid);
    break;

  case PLF::EXIT:
    if (valueid == "x_pos") {
      exit_s.x_pos = str_to_int(value);
      if (verbose > 2) std::cout << "Exit.x_pos: " << exit_s.x_pos << ":'" << value << "'" << std::endl;
    } else if (valueid == "y_pos") {
      exit_s.y_pos = str_to_int(value);
    } else if (valueid == "z_pos") {
      exit_s.z_pos = str_to_int(value);
      if (verbose > 2) std::cout << "Exit.y_pos: " << exit_s.y_pos << std::endl;
    } else if (valueid == "image") {
      exit_s.desc = ResDescriptor(cast, value);
      if (verbose > 2) std::cout << "Exit.desc: " << exit_s.desc.res_name << std::endl;
    } else {
      throw PingusError("Exit: Unknown ValueID: " + valueid);
    }
    break;

  case PLF::ENTRANCE:
    if (valueid == "x_pos") {
      entrance_s.x_pos = str_to_int(value);
    } else if (valueid == "y_pos") {
      entrance_s.y_pos = str_to_int(value);
    } else if (valueid == "z_pos") {
      entrance_s.z_pos = str_to_int(value);
    } else if (valueid == "image") {
      entrance_s.desc.res_name = value;
    } else if (valueid == "type") {
      entrance_s.type = value;
    } else if (valueid == "direction") {
      if (value == "left") {
	entrance_s.direction = entrance_data::LEFT;
      } else if (value == "right") {
	entrance_s.direction = entrance_data::RIGHT;
      } else if (value == "misc") {
	entrance_s.direction = entrance_data::MISC;
      } else {
	throw PingusError("Entrance: Unknown Value for direction: " + valueid);
      }
    } else if (valueid == "release_rate") {
      entrance_s.release_rate = str_to_int(value);
    } else {
      throw PingusError("Entrance: Unknown ValueID: " + valueid);
    }
    break;

  case PLF::TRAP:
    if (valueid == "name") {
      trap_s.name = value;
    } else if (valueid == "x_target") {
      trap_s.x_target = str_to_int(value);
    } else if (valueid == "y_target") {
      trap_s.y_target = str_to_int(value);
    } else if (valueid == "x_pos") {
      trap_s.x_pos = str_to_int(value);
    } else if (valueid == "y_pos") {
      trap_s.y_pos = str_to_int(value);
    } else if (valueid == "z_pos") {
      trap_s.z_pos = str_to_int(value);
    } else {
      throw PingusError("PLF:Trap: Unknown value id: " + valueid);      
    }
    break;
    
  case PLF::HOTSPOT:
    if (valueid == "x_pos") {
      hotspot_s.x_pos = str_to_int(value);
    } else if (valueid == "y_pos") {
      hotspot_s.y_pos = str_to_int(value);
    } else if (valueid == "z_pos") {
      hotspot_s.z_pos = str_to_int(value);
    } else if (valueid == "speed") {
      hotspot_s.speed = str_to_int(value);
    } else if (valueid == "image") {
      hotspot_s.desc = ResDescriptor(cast, value);
    } else if (valueid == "para") {
      hotspot_s.para = str_to_float(value);
    } else {
      throw PingusError("PLF:Hotspot: Unknown value id " + valueid);
    }
    break;

  case PLF::BUTTONS:
    {
      button_data temp;
      temp.name = valueid;
      sscanf(value.c_str(), "%d", &temp.number_of);
      buttons.push_back(temp);
    }
    break;

  case PLF::LIQUID:
    if (valueid == "x_pos") {
      liquid_s.x_pos = str_to_int(value);
    } else if (valueid == "y_pos") {
      liquid_s.y_pos = str_to_int(value);
    } else if (valueid == "z_pos") {
      liquid_s.z_pos = str_to_int(value);
    } else if (valueid == "width") {
      liquid_s.width = str_to_int(value);
    } else if (valueid == "image") {
      liquid_s.desc = ResDescriptor(cast, value);
    } else if (valueid == "speed") {
      liquid_s.speed = str_to_int(value);
    } else {
      throw PingusError("PLF:Liquid: Unknown value id: " + valueid);
    }
    break;
    
  default:
    throw PingusError("PLF:set_value(): Internal Parser error!");
  }
}

void
PLF::set_group_start(std::string groupname)
{
  if (groupname == "global") {
    current_group = PLF::GLOBAL;
  } else if (groupname == "background") {
    current_group = PLF::BACKGROUND;
  } else if (groupname == "ground") {
    current_group = PLF::GROUND;
  } else if (groupname == "music") {
    current_group = PLF::MUSIC;
  } else if (groupname == "exit") {
    current_group = PLF::EXIT;
  } else if (groupname == "liquid") {
    current_group = PLF::LIQUID;
  } else if (groupname == "entrance") {
    current_group = PLF::ENTRANCE;
  } else if (groupname == "trap") {
    current_group = PLF::TRAP;
  } else if (groupname == "hotspot") {
    current_group = PLF::HOTSPOT;
  } else if (groupname == "buttons") {
    current_group = PLF::BUTTONS;
  } else {
    throw PingusError("Parse error: Unknow groupname: '" + groupname + "'");
  }  
}

void
PLF::set_group_end(void)
{
  // flush collected data
  switch(current_group) {
  case PLF::EXIT:
    exits.push_back(exit_s);
    exit_s.clean();
    break;

  case PLF::ENTRANCE:
    entrances.push_back(entrance_s);
    entrance_s.clean();
    break;

  case PLF::TRAP:
    traps.push_back(trap_s);
    trap_s.clean();
    break;

  case PLF::HOTSPOT:
    hotspot.push_back(hotspot_s);
    hotspot_s.clean();
    break;

  case PLF::LIQUID:
    liquid.push_back(liquid_s);
    liquid_s.clean();
    break;

  default:
    if (verbose > 2)
      std::cout << "PLF:set_group_end(): Nothing to flush" << std::endl;
    break;
  }

}

background_data
PLF::get_bg()
{
  return bg;
}

ResDescriptor
PLF::get_fg()
{
  if (!psm_filename.empty()) {
    return ResDescriptor("file", psm_filename);
  } else {
    ResDescriptor ret_val = fg;
    
    switch(fg.type) {
    case  ResDescriptor::FILE:
      ret_val.res_name = find_file(pingus_datadir, "levels/" + fg.res_name);
      break;
    case  ResDescriptor::RESOURCE:
      if (verbose > 1) std::cout << "Not supported" << std::endl;
      break;
    default:
      assert(false);
      break;
    }
    return ret_val;
  }
}

MapType
PLF::map_type()
{
  return maptype;
}

ResDescriptor
PLF::get_mapfile()
{
  switch(col.type) {
  case  ResDescriptor::FILE:
    col.res_name = find_file(pingus_datadir, "levels/" + col.res_name);
    break;
  case  ResDescriptor::RESOURCE:
    break;
  default:
    break;
  }
  return col;
}

int
PLF::get_startx()
{
  return x_pos;
}

int
PLF::get_starty()
{
  return y_pos;
}

int
PLF::get_width(void)
{
  return width;
}

int
PLF::get_height(void)
{
  return height;
}

vector<hotspot_data>
PLF::get_hotspot()
{
  return hotspot;
}

vector<exit_data>
PLF::get_exit()
{
  return exits;
}

vector<entrance_data>
PLF::get_entrance()
{
  return entrances;
}

vector<trap_data> 
PLF::get_traps()
{
  return traps;
}

int
PLF::get_pingus()
{
  return pingus;
}

vector<button_data>
PLF::get_buttons()
{
  return buttons;
}

ResDescriptor
PLF::get_music()
{
  return music;
}

string
PLF::get_description(void)
{
  return description;
}

string
PLF::get_levelname()
{
  return levelname;
}

vector<liquid_data>
PLF::get_liquids(void)
{
  return liquid;
}

int
PLF::get_time(void)
{
  return max_time;
}

int
PLF::str_to_int(const std::string& str)
{
  int ret_val;

  if (sscanf(str.c_str(), "%d", &ret_val) != 1) {
    throw PingusError("PLF: Couldn't covert std::string to integer: " + str);
  }

  return ret_val;
}

float
PLF::str_to_float(const std::string& str)
{
  float ret_val;

  if (sscanf(str.c_str(), "%f", &ret_val) != 1) {
    throw PingusError("PLF: Couldn't covert std::string to float: " + str);
  }

  return ret_val;
}

bool
PLF::str_to_bool(const std::string& str)
{
  if (str == "true" || str == "1")
    {
      return true;
    }
  else if (str == "false" || str == "0")
    {
      return false;
    }
  else
    {
      throw PingusError("PLF: value: " + str + " is not of type bool.");
    }
}

void
PLF::set_psm_filename(std::string name)
{
  psm_filename = name;
}

int
PLF::get_number_to_save()
{
  return number_to_save;
}

int  
PLF::get_difficulty()
{
  return difficulty;
}

std::string
PLF::get_author()
{
  return author;
}

/* EOF */
