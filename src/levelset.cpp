//  $Id: levelset.cpp 3393 2007-10-30 06:24:52Z jsalmon3 $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "pingus_error.hpp"
#include "file_reader.hpp"
#include "levelset.hpp"
#include "plf_res_mgr.hpp"
#include "savegame_manager.hpp"
#include "resource.hpp"
#include "math.hpp"

Levelset::Levelset(const Pathname& pathname)
  : completion(0)
{
  FileReader reader = FileReader::parse(pathname);
  if (reader.get_name() != "pingus-levelset")
    {
      PingusError::raise("Error: " + pathname.str() + ": not a 'pingus-levelset' file");
    }
  else
    {
      reader.read_string("title",       title);
      reader.read_string("description", description);
      std::string image;
      if (reader.read_string("image", image))
        this->image = Resource::load_sprite(image);

      FileReader level_reader = reader.read_section("levels");
      std::vector<FileReader> sections = level_reader.get_sections();
      for(std::vector<FileReader>::iterator i = sections.begin(); i != sections.end(); ++i)
        {
          if (i->get_name() == "level")
            {
              Level* level = new Level();

              if (i->read_string("filename", level->resname))
                {
                  level->plf        = PLFResMgr::load_plf(level->resname);
                  
                  level->accessible = false;
                  level->finished   = false;
                      
                  levels.push_back(level);
                }
              else
                {
                  std::cout << "Levelset: " << pathname.str() << " is missing filename tag" << std::endl;
                  delete level;
                }
            }
        }
    }

  refresh();
}

Levelset::~Levelset()
{
  for(std::vector<Level*>::iterator i = levels.begin(); i != levels.end(); ++i)
    {
      delete *i;
    }
}

std::string
Levelset::get_title() const
{
  return title;
}

std::string
Levelset::get_description() const
{
  return description;
}

Levelset::Level*
Levelset::get_level(int num) const
{
  if (num >= 0 && num < int(levels.size()))
    return levels[num];
  else
    return 0;
}

int
Levelset::get_level_count() const
{
  return levels.size();
}

int
Levelset::get_completion()  const
{
  return completion;
}

Sprite
Levelset::get_image() const
{
  return image;
}

void
Levelset::refresh()
{
  for(std::vector<Level*>::iterator i = levels.begin(); i != levels.end(); ++i)
    {
      Savegame* savegame = SavegameManager::instance()->get((*i)->resname);

      if (savegame)
        {
          (*i)->accessible = (savegame->get_status() != Savegame::NONE);
          (*i)->finished   = (savegame->get_status() == Savegame::FINISHED);
        }
    }

  if (!levels.empty())
    {
      levels[0]->accessible = true; 
      for(std::vector<Level*>::size_type i = 0; i < levels.size()-1; ++i)
        if (levels[i]->finished)
          levels[i+1]->accessible = true;
    }

  completion = 0;
  for(std::vector<Level*>::iterator i = levels.begin(); i != levels.end(); ++i)
    if ((*i)->finished)
      completion += 1;
  completion = Math::clamp(0, completion * 100 / int(levels.size()), 100);
}

/* EOF */
