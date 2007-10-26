//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_LEVELSET_HPP
#define HEADER_LEVELSET_HPP

#include <string>
#include <vector>
#include "pathname.hpp"

/** */
class Levelset
{
private:
  std::string title;
  std::string description;
  std::vector<std::string> levels;

public:
  Levelset(const Pathname& pathname);
  ~Levelset();

  std::string get_title() const;
  std::string get_description() const;
  std::string get_level(int num) const;
  int get_level_count() const;

private:
  Levelset (const Levelset&);
  Levelset& operator= (const Levelset&);
};

#endif

/* EOF */
