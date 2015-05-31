// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_RESOURCE_RESOURCE_MANAGER_HPP
#define HEADER_PINGUS_ENGINE_RESOURCE_RESOURCE_MANAGER_HPP

#include <map>

#include "engine/resource/resource.hpp"

namespace resource {

template<typename Key,
         typename Data,
         typename Loader>
class ResourceManager
{
private:
  std::map<Key, Resource<Data> > m_resources;

public:
  ResourceManager() :
    m_resources()
  {
  }

  ~ResourceManager()
  {
  }

  Resource<Data> get(const Key& key)
  {
    auto it = m_resources.find(key);
    if (it != m_resources.end())
    {
      return it->second;
    }
    else
    {
      Resource<Data> res = Resource<Data>(Loader(key));
      m_resources[key] = res;
      return res;
    }
  }

  void reload()
  {
    for(auto it = m_resources.begin(); it != m_resources.end(); ++it)
    {
      if (*it)
      {
        (*it)->load();
      }
    }
  }

private:
  ResourceManager(const ResourceManager&);
  ResourceManager& operator=(const ResourceManager&);
};

} // namespace resource

#endif

/* EOF */
