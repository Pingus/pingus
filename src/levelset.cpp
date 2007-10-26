//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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
#include "pingus_error.hpp"
#include "file_reader.hpp"
#include "levelset.hpp"

Levelset::Levelset(const Pathname& pathname)
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
      FileReader level_reader = reader.read_section("levels");
      std::vector<FileReader> sections = level_reader.get_sections();
      for(std::vector<FileReader>::iterator i = sections.begin(); i != sections.end(); ++i)
        {
          if (i->get_name() == "level")
            {
              std::string filename;
              if (i->read_string("filename", filename))
                {
                  levels.push_back(filename);
                }
              else
                {
                  std::cout << "Levelset: " << pathname.str() << " is missing filename tag" << std::endl;
                }
            }
        }
    }
}

Levelset::~Levelset()
{
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

std::string
Levelset::get_level(int num) const
{
  if (num >= 0 && num < static_cast<int>(levels.size()))
    return levels[num];
  else
    return "";
}

int
Levelset::get_level_count() const
{
  return levels.size();
}

/* EOF */
