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

#ifndef HEADER_PINGUS_UTIL_STRING_UTIL_HPP
#define HEADER_PINGUS_UTIL_STRING_UTIL_HPP

#include <sstream>

class StringUtil
{
private:
public:
  template<class T>
  static T to(const std::string& s, const T& val = T())
  {
    T tmp = val;
    std::istringstream str(s);
    str >> tmp;
    return tmp;
  }

  template<class T>
  static bool from_string(const std::string& s, T& t)
  {
    std::istringstream str(s);
    T tmp;
    str >> tmp;
    if (str.fail())
    {
      return false;
    }
    else
    {
      t = tmp;
      return true;
    }
    return false;
  }

  template<class T>
  static std::string to_string(const T& t)
  {
    std::ostringstream str;
    str << t;
    return str.str();
  }

private:
  StringUtil ();
  StringUtil (const StringUtil&);
  StringUtil& operator= (const StringUtil&);
};

#endif

/* EOF */
