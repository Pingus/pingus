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

#ifndef HEADER_PINGUS_PINGUS_GROUNDTYPE_HPP
#define HEADER_PINGUS_PINGUS_GROUNDTYPE_HPP

#include <string>

/** This class olds the definitions of the different types of ground
    available in pingus, it might also provide converter function from
    GPType to std::string and vice verse. */
class Groundtype
{
public:
  enum GPType {
    GP_NOTHING,
    GP_SOLID,
    GP_TRANSPARENT,
    GP_GROUND,
    GP_BRIDGE,
    GP_WATER,
    GP_LAVA,
    GP_REMOVE,
    GP_OUTOFSCREEN
  };

  static GPType      string_to_type (const std::string& arg_type);
  static std::string type_to_string (GPType arg_type);
};

#endif

/* EOF */
