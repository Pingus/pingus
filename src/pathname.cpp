/*  $Id$
**
**  Pingus - A free Lemmings clone
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <assert.h>
#include <iostream>
#include "system.hpp"
#include "path_manager.hpp"
#include "pathname.hpp"

Pathname Pathname::tmpfile(const std::string& prefix)
{
  assert(!"Unimplemented");
}

Pathname::Pathname()
 : type(INVALID)
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
        return path_manager.complete(pathname);

      default:
        assert(!"Never reached");
    }
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
}

/* EOF */
