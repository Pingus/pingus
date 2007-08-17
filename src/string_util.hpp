//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_STRING_UTIL_HPP
#define HEADER_STRING_UTIL_HPP

#include <sstream>

/** */
class StringUtil
{
private:
public:
  static std::string to_lower(const std::string &str);
  static std::string to_upper(const std::string &str);

  template<class T>
  static T to(const std::string& s) 
  {
    T tmp;
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
    return tmp;
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
