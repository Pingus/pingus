// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/resource_manager.hpp"

#include <algorithm>

#include "engine/display/sprite_description.hpp"
#include "pingus/globals.hpp"
#include "util/log.hpp"
#include "util/string_util.hpp"
#include "util/system.hpp"

ResourceManager::ResourceManager() :
  m_cache(),
  m_resources()
{
}

ResourceManager::~ResourceManager()
{
}

std::vector<std::string>
ResourceManager::get_section(const std::string& name)
{
  std::vector<std::string> lst;

  for (auto i = m_resources.begin(); i != m_resources.end(); ++i)
  {
    if (StringUtil::has_prefix(*i, name))
    {
      lst.push_back(*i);
    }
  }

  return lst;
}

void
ResourceManager::add_resources_from_directory(const Pathname& path)
{
  assert(path.get_type() == Pathname::DATA_PATH);

  auto files = path.opendir_recursive();
  for(auto it = files.begin(); it != files.end(); ++it)
  {
    if (it->has_extension(".sprite") ||
        it->has_extension(".png") ||
        it->has_extension(".jpg"))
    {
      // FIXME: ugly hack to remove "images/" prefix, need better
      // way to cut stuff away
      std::string resdesc = System::cut_file_extension(it->get_raw_path().substr(7));
      m_resources.insert(resdesc);
    }
  }
}

std::shared_ptr<SpriteDescription>
ResourceManager::get_sprite_description_from_file(const std::string& resname)
{
  // try to load a .sprite file
  std::string filename = "images/" + resname + ".sprite";
  Pathname path(filename, Pathname::DATA_PATH);
  if (path.exist())
  {
    SpriteDescriptionPtr desc = SpriteDescription::from_file(path);

    // resolve relative filenames
    if (!desc->filename.absolute())
    {
      // path normalization is important here to allow overlay paths,
      // as a path of the form "pingus/blocker/../blocker.png" would
      // fail when "pingus/blocker/" is missing, even so
      // "pingus/blocker.png" exists
      desc->filename = Pathname(System::normalize_path(Pathname::join(System::dirname(filename),
                                                                      desc->filename.get_raw_path())),
                                Pathname::DATA_PATH);
    }

    return desc;
  }

  // try to load a .png file
  filename = "images/" + resname + ".png";
  path = Pathname(filename, Pathname::DATA_PATH);
  if (path.exist())
  {
    SpriteDescriptionPtr desc = std::make_shared<SpriteDescription>();
    desc->filename = path;
    return desc;
  }

  // try to load a .jpg file
  filename = "images/" + resname + ".jpg";
  path = Pathname(filename, Pathname::DATA_PATH);
  if (path.exist())
  {
    SpriteDescriptionPtr desc = std::make_shared<SpriteDescription>();
    desc->filename = path;
    return desc;
  }

  // give up
  return SpriteDescriptionPtr();
}

SpriteDescription*
ResourceManager::get_sprite_description(const std::string& name)
{
  auto i = m_cache.find(name);
  if (i != m_cache.end())
  {
    return i->second.get();
  }
  else
  {
    SpriteDescriptionPtr desc = get_sprite_description_from_file(name);
    if (desc)
    {
      m_cache[name] = desc;
      return desc.get();
    }
    else
    {
      return 0;
    }
  }
}

/* EOF */
