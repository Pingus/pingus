//  $Id: PLF.hh,v 1.1 2000/02/04 23:45:18 mbn Exp $
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
  string levelname;
  string psm_filename;
  string description;
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

  vector<button_data>   buttons;
  vector<entrance_data> entrances;
  vector<exit_data>     exits;
  vector<hotspot_data>  hotspot;
  vector<liquid_data>   liquid;
  vector<trap_data>     traps;

  enum plf_groups { GLOBAL, BACKGROUND, GROUND, MUSIC, EXIT, LIQUID,
		    ENTRANCE, TRAP, HOTSPOT, BUTTONS, NONE } current_group;

  // Private functions
  void set_group_start(string);
  void set_group_end(void);
  void set_value(string valueid,
		 string cast,
		 string value);
  int    str_to_int(const string& str);
  double str_to_double(const string& str);
public:
  PLF(string);
  ~PLF();

  background_data get_bg(void);
  ResDescriptor get_fg(void);
  ResDescriptor get_mapfile(void);
  void set_psm_filename(string name);
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
  string                 get_description(void);
  string                 get_levelname(void);
  vector<hotspot_data>   get_hotspot(void);
  vector<entrance_data>  get_entrance(void);
  vector<exit_data>      get_exit(void);
  vector<trap_data>      get_traps(void);
  vector<button_data>    get_buttons(void);
  vector<liquid_data>    get_liquids(void);
};

#endif

/* EOF */
