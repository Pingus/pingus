//  $Id: plt_xml.hxx,v 1.9 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_PLT_XML_HXX
#define HEADER_PINGUS_PLT_XML_HXX

#include <map>
#include "xml_helper.hxx"

namespace Pingus {

class PLTXML
{
private:
  xmlDocPtr doc;
  std::vector<std::string> level_list;
  std::map<std::string, std::string> world_name;
  std::map<std::string, std::string> description;

  void parse_file();
  void parse_background(xmlNodePtr cur);
  void parse_description(xmlNodePtr cur);
  void parse_world_name(xmlNodePtr cur);
  void parse_level_list(xmlNodePtr cur);

public:
  PLTXML ();
  ~PLTXML ();

  void parse (std::string filename);

  std::map<std::string,std::string> get_name ();
  std::map<std::string,std::string> get_description ();
  std::string get_surface () { return ""; }
  std::vector<std::string> get_levels ();

private:
  PLTXML (const PLTXML&);
  PLTXML& operator= (const PLTXML&);
};

} // namespace Pingus

#endif

/* EOF */
