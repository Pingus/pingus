//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_PREFAB_FILE_HPP
#define HEADER_PINGUS_PINGUS_PREFAB_FILE_HPP

#include <vector>

#include "util/file_reader.hpp"
#include "util/pathname.hpp"

class PrefabFile
{
public:
  static PrefabFile from_resource(const std::string& name);
  static PrefabFile from_path(const Pathname& file);

private:
  std::string m_name;
  std::vector<FileReader> m_objects;
  FileReader m_overrides;

private:
  PrefabFile(const std::string& filename, const std::vector<FileReader>& objects,
             const FileReader& overrides);

public:
  const std::vector<FileReader>& get_objects() const;
  FileReader get_overrides() const;
};

#endif

/* EOF */
