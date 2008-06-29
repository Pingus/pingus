//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_PLT_XML_HPP
#define HEADER_PINGUS_PLT_XML_HPP

#include <vector>
#include <string>


class PLTXML
{
private:
  std::vector<std::string> level_list;
  std::string world_name;
  std::string description;

  void parse_file();
  
public:
  PLTXML ();
  ~PLTXML ();

  void parse (std::string filename);

  const std::string& get_name ();
  const std::string& get_description ();
  std::vector<std::string> get_levels ();

private:
  PLTXML (const PLTXML&);
  PLTXML& operator= (const PLTXML&);
};


#endif

/* EOF */
