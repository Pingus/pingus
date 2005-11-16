//  $Id: level_impl.hxx,v 1.00 2005/11/11 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include <ClanLib/Core/System/sharedptr.h>
#include <ClanLib/Core/Math/size.h>
#include <ClanLib/Display/color.h>
#include <string>
#include <vector>
#include <map>
#include "level_objs.hxx"

namespace Pingus {

namespace Editor {

	class LevelObj;

class LevelImpl
{
public:
  // Set defaults in constructor
  LevelImpl()
  : ambient_light(CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f)),
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

  CL_Colorf   ambient_light;
  CL_Size     size;

  int number_of_pingus;
  int number_to_save;

  std::map<std::string, int> actions;

  int time;
  int difficulty;
  
  std::string author;
	std::string comment;
  std::string music;

  std::vector<LevelObj*> objects;

private:
	LevelImpl (const LevelImpl&);
  LevelImpl& operator= (const LevelImpl&);
};	// LevelImpl class

}		// Editor namespace
}		// Pingus namespace

#endif

/* EOF */