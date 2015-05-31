// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#include "util/string_util.hpp"

// Code based on clanstring.cpp from ClanLib 0.8

std::string
StringUtil::to_lower(const std::string &str)
{
  std::string lower_impl = str;

  for( std::string::iterator i = lower_impl.begin();
       i != lower_impl.end();
       ++i )
  {
    *i = static_cast<char>(tolower(*i));
  }

  return lower_impl;
}

std::string
StringUtil::to_upper(const std::string& str)
{
  std::string upper_impl = str;

  for( std::string::iterator i = upper_impl.begin();
       i != upper_impl.end();
       ++i )
  {
    *i = static_cast<char>(toupper(*i));
  }

  return upper_impl;
}

/* EOF */
