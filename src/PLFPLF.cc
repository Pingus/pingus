//  $Id: PLFPLF.cc,v 1.12 2001/08/12 18:36:40 grumbel Exp $
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
#include "PSMParser.hh"
#include "PLFPLF.hh"
#include "backgrounds/SurfaceBackgroundData.hh"

using namespace std;

int    str_to_int(const string& str);
float str_to_float(const string& str);

// Create a PLF object and start parsing the given file.
PLFPLF::PLFPLF(string arg_filename)
{
  //std::cout << "----- Parsing .plf file" << std::endl;

  filename = arg_filename;

  start_x_pos = start_y_pos = 0;
  width = 0; height = 0;
  number_of_pingus = 100;
  max_time = 9000;
  number_to_save = 0;
  difficulty = 5;

  music = ResDescriptor("file", "pingus-4.it");

  col = ResDescriptor("auto", "-Auto ColMap-");
  if (verbose > 1) cout << "Starting Parsing" << endl;
  init(filename);

  PSMParser psm_parser;

  psm_parser.parse(filename.substr(0, filename.size() - 4) + ".psm");
  groundpieces = psm_parser.get_surfaces();
}

// Destroy all data
PLFPLF::~PLFPLF()
{
}

void
PLFPLF::set_value(string valueid,
	       string cast,
	       string value)
{
  switch(current_group) {

  case PLFPLF::GLOBAL:
    if (valueid == "start_x_pos") {
      start_x_pos = str_to_int(value);
    } else if (valueid == "start_y_pos") {
      start_y_pos = str_to_int(value);
    } else if (valueid == "number_of_pingus") {
      number_of_pingus = str_to_int(value);
    } else if (valueid == "number_to_save") {
      number_to_save = str_to_int(value);
    } else if (valueid == "levelname") {
      levelname[default_language] = value;
    } else if (valueid == "description") {
      description[default_language] = value;
    } else if (valueid == "time") {
      max_time = str_to_int(value);
    } else if (valueid == "difficulty") {
      difficulty = str_to_int(value);
    } else if (valueid == "author") {
      author = value;
    } else {
      std::cout << "Global: Unknown ValueID: " << valueid << std::endl;
    }
    break;

  case PLFPLF::BACKGROUND:
    {
      assert(sur_background.get());
      /*      SurfaceBackgroundData* sur_background;

      // FIXME: Memory leak, but not important, this file will be
      // FIXME: deleted soon 
      sur_background = new SurfaceBackgroundData();
      backgrounds.push_back(sur_background);
      */
      if (valueid == "image") 
	{
	  sur_background->desc = ResDescriptor(cast, value);
	} 
      else if (valueid == "scroll_x") 
	{
	  sur_background->scroll_x  = str_to_float(value);
	} 
      else if (valueid == "scroll_y") 
	{
	  sur_background->scroll_y = str_to_float(value);
	} 
      else if (valueid == "para_x")
	{
	  sur_background->para_x = str_to_float(value);
	}
      else if (valueid == "para_y")
	{
	  sur_background->para_y = str_to_float(value);
	}
      else if (valueid == "stretch_x")
	{
	  sur_background->stretch_x = str_to_bool(value);
	}
      else if (valueid == "stretch_y")
	{
	  sur_background->stretch_y = str_to_bool(value);
	}
      else if (valueid == "dim") 
	{
	  sur_background->color.alpha = str_to_float(value);
	} 
      else if (valueid == "alpha") 
	{
	  sur_background->color.alpha = str_to_float(value);
	} 
      else if (valueid == "red") 
	{
	  sur_background->color.red = str_to_float(value);	
	} 
      else if (valueid == "green") 
	{
	  sur_background->color.green = str_to_float(value);	
	} 
      else if (valueid == "blue") 
	{
	  sur_background->color.blue = str_to_float(value);	
	} 
      else 
	{
	  std::cout << "Background: Unknown ValueID: " << valueid << std::endl;
	}
    }
    break;

  case PLFPLF::GROUND:
    /*
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
      } else*/ if (valueid == "data") {
	foreground = ResDescriptor(cast, value);
      } else if (valueid == "colmap") {
	col = ResDescriptor(cast, value);
      } else if (valueid == "width") {
	width = str_to_int(value);
      } else if (valueid == "height") {    
	height = str_to_int(value);
      } else {
	std::cout << "Ground: Unknown ValueID: " << valueid << std::endl;
      }
  break;

  case PLFPLF::MUSIC:
    if (valueid == "data")
      music = ResDescriptor(cast, value);
    else
      std::cout << "Music: Unknown ValueID: " << valueid << std::endl;
    break;

  case PLFPLF::EXIT:
    if (valueid == "x_pos") {
      exit_s.pos.x = str_to_int(value);
      if (verbose > 2) cout << "Exit.x: " << exit_s.pos.x << ":'" << value << "'" << endl;
    } else if (valueid == "y_pos") {
      exit_s.pos.y = str_to_int(value);
    } else if (valueid == "z_pos") {
      exit_s.pos.z = str_to_int(value);
      if (verbose > 2) cout << "Exit.y: " << exit_s.pos.y << endl;
    } else if (valueid == "image") {
      exit_s.desc = ResDescriptor(cast, value);
      if (verbose > 2) cout << "Exit.desc: " << exit_s.desc.res_name << endl;
    } else {
      std::cout << "Exit: Unknown ValueID: " << valueid << std::endl;
    }
    break;

  case PLFPLF::ENTRANCE:
    if (valueid == "x_pos") {
      entrance_s.pos.x = str_to_int(value);
    } else if (valueid == "y_pos") {
      entrance_s.pos.y = str_to_int(value);
    } else if (valueid == "z_pos") {
      entrance_s.pos.z = str_to_int(value);
    } else if (valueid == "image") {
      entrance_s.desc.res_name = value;
    } else if (valueid == "type") {
      entrance_s.type = value;
    } else if (valueid == "direction") {
      if (value == "left") {
	entrance_s.direction = EntranceData::LEFT;
      } else if (value == "right") {
	entrance_s.direction = EntranceData::RIGHT;
      } else if (value == "misc") {
	entrance_s.direction = EntranceData::MISC;
      } else {
	std::cout << "Entrance: Unknown Value for direction: " << valueid << std::endl;
      }
    } else if (valueid == "release_rate") {
      entrance_s.release_rate = str_to_int(value);
    } else {
      std::cout << "Entrance: Unknown ValueID: " << valueid << std::endl;
    }
    break;

  case PLFPLF::TRAP:
    if (valueid == "name") {
      trap_s.type = value;
      /*    } else if (valueid == "x_target") {
	    trap_s.x_target = str_to_int(value);
	    } else if (valueid == "y_target") {
	    trap_s.y_target = str_to_int(value);*/
    } else if (valueid == "x_pos") {
      trap_s.pos.x = str_to_int(value);
    } else if (valueid == "y_pos") {
      trap_s.pos.y = str_to_int(value);
    } else if (valueid == "z_pos") {
      trap_s.pos.z = str_to_int(value);
    } else {
      std::cout << "PLF:Trap: Unknown value id: " << valueid << std::endl;      
    }
    break;
    
  case PLFPLF::HOTSPOT:
    if (valueid == "x_pos") {
      hotspot_s.pos.x = str_to_int(value);
    } else if (valueid == "y_pos") {
      hotspot_s.pos.y = str_to_int(value);
    } else if (valueid == "z_pos") {
      hotspot_s.pos.z = str_to_int(value);
    } else if (valueid == "speed") {
      hotspot_s.speed = str_to_int(value);
    } else if (valueid == "image") {
      hotspot_s.desc = ResDescriptor(cast, value);
    } else if (valueid == "para") {
      hotspot_s.para = str_to_float(value);
    } else {
      std::cout << "PLF:Hotspot: Unknown value id " << valueid << std::endl;
    }
    break;

  case PLFPLF::BUTTONS:
    {
      ActionData temp;
      temp.name = valueid;
      sscanf(value.c_str(), "%d", &temp.number_of);
      actions.push_back(temp);
    }
    break;

  case PLFPLF::LIQUID:
    if (valueid == "x_pos") {
      liquid_s.pos.x = str_to_int(value);
    } else if (valueid == "y_pos") {
      liquid_s.pos.y = str_to_int(value);
    } else if (valueid == "z_pos") {
      liquid_s.pos.z = str_to_int(value);
    } else if (valueid == "width") {
      liquid_s.width = str_to_int(value);
    } else if (valueid == "image") {
      liquid_s.desc = ResDescriptor(cast, value);
    } else if (valueid == "speed") {
      liquid_s.speed = str_to_int(value);
    } else {
      std::cout << "PLF:Liquid: Unknown value id: " << valueid << std::endl;
    }
    break;
    
  default:
    throw PingusError("PLF:set_value(): Internal Parser error!");
  }
}

void
PLFPLF::set_group_start(string groupname)
{
  if (groupname == "global") {
    current_group = PLFPLF::GLOBAL;
  } else if (groupname == "background") {
    current_group = PLFPLF::BACKGROUND;
    sur_background = boost::shared_ptr<SurfaceBackgroundData>(new SurfaceBackgroundData());
    //backgrounds.push_back(sur_background);
  } else if (groupname == "ground") {
    current_group = PLFPLF::GROUND;
  } else if (groupname == "music") {
    current_group = PLFPLF::MUSIC;
  } else if (groupname == "exit") {
    current_group = PLFPLF::EXIT;
  } else if (groupname == "liquid") {
    current_group = PLFPLF::LIQUID;
  } else if (groupname == "entrance") {
    current_group = PLFPLF::ENTRANCE;
  } else if (groupname == "trap") {
    current_group = PLFPLF::TRAP;
  } else if (groupname == "hotspot") {
    current_group = PLFPLF::HOTSPOT;
  } else if (groupname == "buttons") {
    current_group = PLFPLF::BUTTONS;
  } else {
    throw PingusError("Parse error: Unknown groupname: '" + groupname + "'");
  }  
}

void
PLFPLF::set_group_end(void)
{
  // flush collected data
  switch(current_group) {
  case PLFPLF::EXIT:
    exits.push_back(exit_s);
    exit_s.clean();
    break;

  case PLFPLF::ENTRANCE:
    entrances.push_back(entrance_s);
    entrance_s.clean();
    break;

  case PLFPLF::TRAP:
    traps.push_back(trap_s);
    trap_s.clean();
    break;

  case PLFPLF::HOTSPOT:
    hotspots.push_back(hotspot_s);
    hotspot_s.clean();
    break;

  case PLFPLF::LIQUID:
    worldobjs_data.push_back(boost::shared_ptr<WorldObjData> (new LiquidData(liquid_s)));
    liquid_s.clean();
    break;

  default:
    if (verbose > 2)
      cout << "PLF:set_group_end(): Nothing to flush" << endl;
    break;
  }

}

int
PLFPLF::str_to_int(const string& str)
{
  int ret_val;

  if (sscanf(str.c_str(), "%d", &ret_val) != 1) {
    throw PingusError("PLF: Couldn't convert string to integer: " + str);
  }

  return ret_val;
}

float
PLFPLF::str_to_float(const string& str)
{
  float ret_val;

  if (sscanf(str.c_str(), "%f", &ret_val) != 1) {
    throw PingusError("PLF: Couldn't convert string to float: " + str);
  }

  return ret_val;
}

bool
PLFPLF::str_to_bool(const string& str)
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

/* EOF */
