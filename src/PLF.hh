//  $Id: PLF.hh,v 1.15 2000/08/05 18:52:22 grumbel Exp $
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
#include <map>

#include "BackgroundData.hh"
#include "WeatherData.hh"
#include "ActionData.hh"
#include "EntranceData.hh"
#include "ExitData.hh"
#include "HotspotData.hh"
#include "LiquidData.hh"
#include "TrapData.hh"
#include "PinguMap.hh"
#include "WeatherData.hh"

/** The Pingus Level File
    
    The PLF class holds all object information, which are needed to
    build a level, like the levelname, description, number of pingus,
    etc. */
class PLF 
{
protected:
  ///
  std::map<std::string, std::string> levelname;

  /// The name of the file from were the current level was loaded
  std::string filename;

  ///
  std::string psm_filename;
  ///
  std::map<std::string, std::string> description;
  ///
  std::string author;
  ///
  ResDescriptor foreground;
  ///
  BackgroundData background;
  ///
  ResDescriptor music;
  ///
  ResDescriptor col;
  //
  //MapType  maptype;
  ///
  int  bg_speed;
  ///
  int  start_x_pos;
  ///
  int  start_y_pos;
  ///
  int  width;
  ///
  int  height;
  ///
  int  max_time;
  ///
  int release_rate;
  ///
  int  number_of_pingus;
  ///
  int  number_to_save;
  ///
  int  difficulty;
  
  ///
  std::vector<ActionData>   actions;
  ///
  std::vector<EntranceData> entrances;
  ///
  std::vector<ExitData>     exits;
  ///
  std::vector<HotspotData>  hotspots;
  ///
  std::vector<LiquidData>   liquids;
  ///
  std::vector<TrapData>     traps;
  /// 
  std::vector<SurfaceData>  groundpieces;
  ///
  std::vector<WeatherData>  weathers;
public:
  ///
  PLF();
  ///
  virtual ~PLF();

  ///
  BackgroundData get_background(void);
  ///
  ResDescriptor get_foreground(void);
  ///
  ResDescriptor get_mapfile(void);
  ///
  void set_psm_filename(std::string name);
  
  /** Returns the ResDescriptor where the music for this level can be
      found. The ResDescriptor points normaly to a file. */
  ResDescriptor get_music(void);

  /// Returns the start position for the camera
  int         get_startx(void);

  /// Returns the start position for the camera
  int         get_starty(void);

  /// Returns the width of the level
  int         get_width(void);

  /// Returns the height of the level
  int         get_height(void);

  /** Returns the number of Pingus, which are going to be released in
      this level. */
  int         get_pingus(void);
  
  /// Returns the time you have to complete a level
  int         get_time(void);

  /// Returns the difficulty of the current level
  int         get_difficulty();

  ///
  //MapType     map_type(void);

  /// Returns the number of pingus you need to save in this level.
  int         get_number_to_save();

  /** Returns the description of the level, which fit to the current
      language */
  map<std::string, std::string> get_description();

  /** Returns the name of the current level, {\em not} the level file name. */
  map<std::string, std::string> get_levelname();

  /// Returns the name of the plf file.
  std::string get_filename();

  /// Returns the name of the author, who creates this level
  std::string get_author();

  /// 
  std::vector<HotspotData>   get_hotspot(void);
  ///
  std::vector<EntranceData>  get_entrance(void);
  ///
  std::vector<ExitData>      get_exit(void);
  ///
  std::vector<TrapData>      get_traps(void);
  ///
  std::vector<ActionData>    get_actions(void);
  ///
  std::vector<LiquidData>    get_liquids(void);
  ///
  std::vector<SurfaceData>   get_groundpieces(void);
  ///
  std::vector<WeatherData>   get_weather(void);
  //
  //std::vector<EmptyData>            get_groups(void);
};

#endif

/* EOF */
