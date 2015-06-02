// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_PINGUS_LEVEL_IMPL_HPP
#define HEADER_PINGUS_PINGUS_PINGUS_LEVEL_IMPL_HPP

#include "math/color.hpp"
#include "math/size.hpp"
#include <map>
#include <string>
#include <vector>

class PingusLevelImpl
{
public:
  // Set defaults in constructor
  PingusLevelImpl() :
    resname(),
    checksum(),
    levelname(),
    description(),
    ambient_light(255, 255, 255, 255),
    size(),
    number_of_pingus(0),
    number_to_save(0),
    actions(),
    time(0),
    author(),
    music(),
    objects()
  {}

  std::string resname;

  std::string checksum;

  std::string levelname;
  std::string description;

  Color  ambient_light;
  Size   size;

  int number_of_pingus;
  int number_to_save;

  std::map<std::string, int> actions;

  int time;

  std::string author;
  std::string music;

  std::vector<ReaderObject> objects;
};

#endif

/* EOF */
