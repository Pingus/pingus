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

#include "pingus/prefab_file.hpp"

#include <stdexcept>

#include "util/reader.hpp"
#include "util/raise_exception.hpp"
#include "util/system.hpp"

namespace pingus {

PrefabFile
PrefabFile::from_path(Pathname const& filename)
{
  auto doc = prio::ReaderDocument::from_file(filename.get_sys_path());
  if (doc.get_name() != "pingus-prefab") {
    raise_exception(std::runtime_error, "Error: " << filename.str() << ": not a 'pingus-prefab' file");
  } else {
    return PrefabFile(std::move(doc));
  }
}

PrefabFile
PrefabFile::from_resource(std::string const& name)
{
  Pathname filename(name + ".prefab", Pathname::DATA_PATH);
  return from_path(filename);
}

PrefabFile::PrefabFile(ReaderDocument doc) :
  m_doc(std::move(doc)),
  m_objects(),
  m_overrides()
{
  ReaderMapping reader = m_doc.get_mapping();
  reader.read("overrides", m_overrides);
  reader.read("objects", m_objects);
}

ReaderCollection const&
PrefabFile::get_objects() const
{
  return m_objects;
}

ReaderMapping const&
PrefabFile::get_overrides() const
{
  return m_overrides;
}

} // namespace pingus

/* EOF */
