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

#ifndef HEADER_PINGUS_LEVEL_IMPL_HXX
#define HEADER_PINGUS_LEVEL_IMPL_HXX

#include <ClanLib/Core/System/sharedptr.h>
#include <ClanLib/Core/Math/size.h>
#include <ClanLib/Display/color.h>
#include <string>
#include <vector>
#include <map>



class PingusLevelImpl
{
public:
       
  // Set defaults in constructor
  PingusLevelImpl()
  : ambient_light(CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f)),
    music("none")
  {
    // Do nothing
  }
                    
  std::string resname;

  std::string levelname;
  std::string description;

  CL_Colorf   ambient_light;
  CL_Size     size;

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
