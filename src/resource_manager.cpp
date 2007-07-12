/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <iostream>
#include "lisp/lisp.hpp"
#include "lisp/parser.hpp"
#include "sprite_description.hpp"
#include "sexpr_file_reader.hpp"
#include "resource_manager.hpp"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
  std::map<std::string, SpriteDescription*>::iterator i;
  for (i = resources.begin(); i != resources.end(); ++i) {
    delete (*i).second;
  }
}

void
ResourceManager::add_resources(const std::string& filename)
{
  std::cout << "ResourceManager: " << filename << std::endl;
  lisp::Lisp* sexpr = lisp::Parser::parse(filename);
  if (sexpr)
    {
      SExprFileReader reader(sexpr = sexpr->get_list_elem(0));

      if (reader.get_name() == "pingus-resources")
        {
          std::vector<FileReader> sections = reader.get_sections();
          for(std::vector<FileReader>::iterator i = sections.begin(); i != sections.end(); ++i)
            {
              //std::cout << "Section: " << i->get_name() << std::endl;
              parse("", *i);
            }
        }
      else
        {
          std::cout << "Couldn't find section 'pingus-resources' section in file " << filename
                    << "\ngot " << reader.get_name()
                    << std::endl;
        }
    }
  else
    {
      std::cout << "ResourceManager: File not found " << filename << std::endl;
    }
}

void
ResourceManager::parse(const std::string& section, FileReader& reader)
{
  if (reader.get_name() == "section")
    {
      parse_section(section, reader);
    }
  else if (reader.get_name() == "sprite")
    {
      std::string name;
      reader.read_string("name", name);
      if (!section.empty())
        name = section + "/" + name;
     
      resources[name] = new SpriteDescription(reader);
    }
  else if (reader.get_name() == "alias")
    {
      std::string name;
      std::string link;
      reader.read_string("name", name);
      reader.read_string("link", link);
      //std::cout << "alias: " << name << " -> " << link << std::endl;
    }
  else if (reader.get_name() == "name")
    {
      // ignore (ugly)
    }
  else
    {
      std::cout << "ResourceManager: unknown token: '" << reader.get_name() << "'" << std::endl;
    }
}

void
ResourceManager::parse_section(const std::string& section, FileReader& reader)
{
  std::string name;
  reader.read_string("name", name);

  std::vector<FileReader> sections = reader.get_sections();
  for(std::vector<FileReader>::iterator i = sections.begin(); i != sections.end(); ++i)
    {    
      if (section.empty())
        parse(name, *i);
      else
        parse(section + "/" + name, *i);
    }
}

SpriteDescription* 
ResourceManager::get_sprite_description(const std::string& name) const
{
  Resources::const_iterator i = resources.find(name);
  if (i != resources.end())
    {
      return i->second;
    }
  else
    {
      return 0;
    }  
}

/* EOF */
