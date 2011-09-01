//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "util/pathname.hpp"

#include <assert.h>
#include <iostream>

#include "pingus/path_manager.hpp"
#include "util/system.hpp"

Pathname Pathname::tmpfile(const std::string& prefix)
{
  assert(!"Unimplemented");
  return Pathname();
}

Pathname::Pathname() :
  pathname(),
  type(INVALID)
{
}

Pathname::Pathname(const std::string& pathname_, Type type_)
  : pathname(pathname_), type(type_)
{
}

std::string
Pathname::get_sys_path() const
{
  switch(type)
  {
    case SYSTEM_PATH:
      return pathname;
        
    case DATA_PATH:
      return g_path_manager.complete(pathname);

    case INVALID:
      return "";

    default:
      assert(!"Never reached");
  }
  return "";
}

std::string
Pathname::get_raw_path() const
{
  return pathname; 
}

Pathname::Type
Pathname::get_type() const
{
  return type;
}

bool
Pathname::empty() const
{
  return (type == INVALID);
}

bool
Pathname::exist() const
{
  return System::exist(get_sys_path());
}

uint64_t
Pathname::mtime() const
{
  return System::get_mtime(get_sys_path());
}

std::string
Pathname::str() const
{
  switch(type)
  {
    case Pathname::INVALID:
      return "invalid://" + pathname; 

    case Pathname::DATA_PATH:
      return "datadir://" + pathname;

    case Pathname::SYSTEM_PATH:
      return "system://" + pathname;

    default: 
      assert(!"never reached");
  }
  return std::string();
}

std::ostream& operator<< (std::ostream& os, const Pathname& p)
{
  switch(p.get_type())
  {
    case Pathname::INVALID:
      return os << "invalid://" << p.get_raw_path();

    case Pathname::DATA_PATH:
      return os << "datadir://" << p.get_raw_path();

    case Pathname::SYSTEM_PATH:
      return os << "system://" << p.get_raw_path();

    default: 
      assert(!"never reached");
  }
  return os;
}

/* EOF */
