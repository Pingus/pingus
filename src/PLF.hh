//  $Id: PLF.hh,v 1.7 2000/04/24 13:15:41 grumbel Exp $
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

#ifndef PLF_HH
#define PLF_HH

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "PLFParser.hh"
#include "PinguMap.hh"
#include "background_data.hh"
#include "button_data.hh"
#include "entrance_data.hh"
#include "exit_data.hh"
#include "hotspot_data.hh"
#include "liquid_data.hh"
#include "trap_data.hh"

class PLF : public PLFParser
{
private:
  std::string levelname;
  std::string psm_filename;
  std::string description;
  std::string author;
  ResDescriptor fg;

  background_data bg;

  ResDescriptor music;
  ResDescriptor col;
  MapType  maptype;
  int  bg_speed;
  int  x_pos;
  int  y_pos;
  int  width;
  int  height;
  int  pingus;
  int  max_time;
  int  number_to_save;
  int  difficulty;
  
  entrance_data entrance_s;
  exit_data     exit_s;
  hotspot_data  hotspot_s;
  liquid_data   liquid_s;
  trap_data     trap_s;

  std::vector<button_data>   buttons;
  std::vector<entrance_data> entrances;
  std::vector<exit_data>     exits;
  std::vector<hotspot_data>  hotspot;
  std::vector<liquid_data>   liquid;
  std::vector<trap_data>     traps;

  enum plf_groups { GLOBAL, BACKGROUND, GROUND, MUSIC, EXIT, LIQUID,
		    ENTRANCE, TRAP, HOTSPOT, BUTTONS, NONE } current_group;

  // Private functions
  void set_group_start(std::string);
  void set_group_end(void);
  void set_value(std::string valueid,
		 std::string cast,
		 std::string value);
  int   str_to_int(const std::string& str);
  float str_to_float(const std::string& str);
  bool  str_to_bool(const std::string& str);
public:
  PLF(std::string);
  ~PLF();

  background_data get_bg(void);
  ResDescriptor get_fg(void);
  ResDescriptor get_mapfile(void);
  void set_psm_filename(std::string name);
  ResDescriptor get_music(void);
  int         get_startx(void);
  int         get_starty(void);
  int         get_width(void);
  int         get_height(void);
  int         get_pingus(void);
  int         get_time(void);
  int         get_difficulty();
  MapType     map_type(void);
  int         get_number_to_save();
  std::string get_description(void);
  std::string get_levelname(void);
  std::string get_author(void);
  std::vector<hotspot_data>   get_hotspot(void);
  std::vector<entrance_data>  get_entrance(void);
  std::vector<exit_data>      get_exit(void);
  std::vector<trap_data>      get_traps(void);
  std::vector<button_data>    get_buttons(void);
  std::vector<liquid_data>    get_liquids(void);
};

#endif

/* EOF */
