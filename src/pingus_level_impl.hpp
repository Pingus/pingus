//  $Id$
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_LEVEL_IMPL_HPP
#define HEADER_PINGUS_LEVEL_IMPL_HPP

#include "math/size.hpp"
#include "math/color.hpp"
#include <string>
#include <vector>
#include <map>

class PingusLevelImpl
{
public:
       
  // Set defaults in constructor
  PingusLevelImpl()
    : ambient_light(255, 255, 255, 255),
      number_of_pingus(0), number_to_save(0),
      time(0), difficulty(0),
      music("none")
  {
    // Do nothing
  }
                    
  std::string resname;

  std::string levelname;
  std::string description;

  Color  ambient_light;
  Size   size;

  int number_of_pingus;
  int number_to_save;

  std::map<std::string, int> actions;

  int time;
  int difficulty;
  
  std::string author;
  std::string music;

  std::vector<FileReader> objects;
}; 


#endif

/* EOF */
