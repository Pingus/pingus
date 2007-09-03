//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOR_LEVEL_IMPL_HXX
#define HEADER_PINGUS_EDITOR_LEVEL_IMPL_HXX

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "level_objs.hpp"


namespace Editor {

static bool LevelObjSort(LevelObj *a, LevelObj *b)
{
  return (a->get_pos().z < b->get_pos().z);
}

class LevelImpl
{
public:
  // Set defaults in constructor
  LevelImpl()
    : ambient_light(255, 255, 255),
      music("none")
  {
    // Do nothing
  }

  /** Destructor */
  ~LevelImpl()
  {
    for (unsigned i = 0; i < objects.size(); i++)
      delete objects[i];
    objects.clear();
  }
                    
  std::string resname;

  std::string levelname;
  std::string description;

  Color    ambient_light;
  Size     size;

  int number_of_pingus;
  int number_to_save;

  std::map<std::string, int> actions;

  int time;
  int difficulty;
  
  std::string author;
  std::string comment;
  std::string music;

  std::vector<LevelObj*> objects;

  /** Sort the objects by their z position */
  void sort_objs()
  {
    std::stable_sort(objects.begin(), objects.end(), LevelObjSort);
  }

private:
  LevelImpl (const LevelImpl&);
  LevelImpl& operator= (const LevelImpl&);
};	// LevelImpl class

}		// Editor namespace

#endif

/* EOF */

