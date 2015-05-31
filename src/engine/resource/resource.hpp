// Pingus - A free Lemmings clone
// Copyright (C) 2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_RESOURCE_RESOURCE_HPP
#define HEADER_PINGUS_ENGINE_RESOURCE_RESOURCE_HPP

#include <functional>
#include <memory>

namespace resource {

template<typename Data>
class ResourceImpl
{
public:
  Data data;
  std::function<Data()> loader;

  ResourceImpl(const std::function<Data()>& loader_) :
    data(),
    loader(loader_)
  {}
};

template<typename Data>
class Resource
{
public:
  Resource() :
    m_impl()
  {
  }

  Resource(const std::function<Data()>& loader) :
    m_impl(new ResourceImpl<Data>(loader))
  {
  }

  Data get() const
  {
    assert(m_impl);

    if (!m_impl->data)
    {
      load();
    }

    return m_impl->data;
  }

  void load()
  {
    if (m_impl)
    {
      m_impl->data = m_impl->loader();
    }
  }

  void unload()
  {
    if (m_impl)
    {
      m_impl->data = Data();
    }
  }

  explicit operator bool(){
    return m_impl && m_impl->data;
  }

private:
  std::shared_ptr<ResourceImpl<Data> > m_impl;
};

} // namespace resource

#endif

/* EOF */
