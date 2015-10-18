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
#include "util/log.hpp"
#include "util/raise_exception.hpp"
#include "util/system.hpp"

PrefabFile
PrefabFile::from_path(const Pathname& filename)
{
  ReaderObject reader_object = Reader::parse(filename);

  if (reader_object.get_name() != "pingus-prefab")
  {
    raise_exception(std::runtime_error, "Error: " << filename.str() << ": not a 'pingus-prefab' file");
  }
  else
  {
    ReaderMapping reader = reader_object.get_mapping();

    ReaderMapping overrides;
    reader.read_mapping("overrides", overrides);

    ReaderCollection objects;
    if (!reader.read_collection("objects", objects) || objects.get_objects().empty())
    {
      raise_exception(std::runtime_error, "Error: " << filename.str() << ": empty prefab file");
    }
    else
    {
      // FIXME: Hacky way to get the Prefab name
      PrefabFile prefab(System::cut_file_extension(filename.get_raw_path()),
                        objects.get_objects(), overrides);
      return prefab;
    }
  }
}

PrefabFile
PrefabFile::from_resource(const std::string& name)
{
  Pathname filename(name + ".prefab", Pathname::DATA_PATH);
  return from_path(filename);
}

PrefabFile::PrefabFile(const std::string& name,
                       const std::vector<ReaderObject>& objects,
                       const ReaderMapping& overrides) :
  m_name(name),
  m_objects(objects),
  m_overrides(overrides)
{
}

std::vector<ReaderObject> const&
PrefabFile::get_objects() const
{
  return m_objects;
}

ReaderMapping const&
PrefabFile::get_overrides() const
{
  return m_overrides;
}

/* EOF */
