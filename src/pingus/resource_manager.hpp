//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_PINGUS_RESOURCE_MANAGER_HPP
#define HEADER_PINGUS_PINGUS_RESOURCE_MANAGER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

class SpriteDescription;
class FileReader;

class ResourceManager
{
public:
  typedef std::map<std::string, std::shared_ptr<SpriteDescription> > Resources;
  typedef std::map<std::string, std::string> Aliases;

private:
  Resources resources; 
  Aliases aliases;

public:
  ResourceManager();
  ~ResourceManager();

  void add_resources(const std::string& filename);

  /** Returns a pointer to the requested SpriteDescription or 0 if it's not found */
  SpriteDescription* get_sprite_description(const std::string& name) const;

  std::vector<std::string> get_section(const std::string& name);

  const Resources& get_resources() const { return resources; }
  const Aliases&   get_aliases() const { return aliases; }

private:
  void parse(const std::string& section, FileReader&);
  void parse_section(const std::string& section, FileReader&);

  ResourceManager (const ResourceManager&);
  ResourceManager& operator= (const ResourceManager&);
};

#endif

/* EOF */
