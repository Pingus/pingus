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

#ifndef HEADER_PATHNAME_HPP
#define HEADER_PATHNAME_HPP

#include <iosfwd>
#include <string>

/** Simple class to allow a distinction of paths that rever to the
 *  filesystem and paths that refer to the datadir, it also hides
 *  path_manager from the rest of the code. */
class Pathname
{
public: 
  /** Generate a temporary filename suitable for temporary files */
  static Pathname tmpfile(const std::string& prefix = "");

  enum Type { 
    // The given pathname refers to the native file system
    SYSTEM_PATH, 

    // The given pathname refers to a file in the datadir
    DATA_PATH,

    INVALID
  };
  
private:
  std::string pathname;
  Type type;

public:
  Pathname();  
  explicit Pathname(const std::string& pathname, Type type = DATA_PATH);

  /** Return the pathname in a form suitable for the native file
      system (i.e. can be passed to ifstream and friends */
  std::string get_sys_path() const;
  
  /** Return the pathname as is */
  std::string get_raw_path() const;

  Type get_type() const;

  /** Converts the Pathname into human readable form, usefull for
      debugging output and nothing else */
  std::string str() const;

  bool empty() const;
  
  bool exist() const;

  uint64_t mtime() const;
};

std::ostream& operator<< (std::ostream& os, const Pathname& p);

#endif

/* EOF */
