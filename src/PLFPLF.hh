//  $Id: PLFPLF.hh,v 1.5 2001/08/12 18:36:40 grumbel Exp $
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

#ifndef PLFPLF_HH
#define PLFPLF_HH

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "PLF.hh"
#include "PLFParser.hh"
#include "PinguMap.hh"
#include "ActionData.hh"
#include "EntranceData.hh"
#include "ExitData.hh"
#include "HotspotData.hh"
#include "LiquidData.hh"
#include "TrapData.hh"

class SurfaceBackgroundData;

/** The Pingus Level File
    
    The PLF class holds all object information, which are needed to
    build a level, like the levelname, description, number of pingus,
    etc. */
class PLFPLF : public PLFParser, public PLF
{
private:
  boost::shared_ptr<SurfaceBackgroundData> sur_background;

  EntranceData entrance_s;
  ExitData     exit_s;
  HotspotData  hotspot_s;
  TrapData     trap_s;
  LiquidData   liquid_s;

  enum plf_groups { GLOBAL, BACKGROUND, GROUND, MUSIC, EXIT, LIQUID,
		    ENTRANCE, TRAP, HOTSPOT, BUTTONS, NONE };
  plf_groups current_group;

  /// Private functions
  void set_group_start(std::string);
  void set_group_end(void);
  void set_value(std::string valueid,
		 std::string cast,
		 std::string value);
  int   str_to_int(const std::string& str);
  float str_to_float(const std::string& str);
  bool  str_to_bool(const std::string& str);
public:
  PLFPLF(std::string);
  virtual ~PLFPLF();
};

#endif

/* EOF */
