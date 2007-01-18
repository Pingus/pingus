//  $Id: xml_pdf.cxx,v 1.10 2003/10/18 23:17:27 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <algorithm>
#include <iostream>
#include <vector>
#include <ClanLib/core.h>
#include "pingus_error.hpp"
#include "path_manager.hpp"
#include "xml_pingus_level.hpp"
////#include "xml_file_reader.hpp"
#include "xml_pdf.hpp"


XMLPDF::XMLPDF(const std::string& filename)
{
  CL_InputSourceProvider_File provider(".");
  CL_DomDocument doc(provider.open_source(filename), true);

  CL_DomElement root = doc.get_document_element();

  if (root.get_tag_name() != "pingus-demo")
    {
      PingusError::raise("Error: " + filename + ": not a <pingus-demo> file");
    }
  else
    {
      XMLFileReader reader(root);

      reader.read_string ("level",  levelname);

      FileReader events_reader;
      reader.read_section("events", events_reader);
      const std::vector<FileReader>& objects = events_reader.get_sections();
      for(std::vector<FileReader>::const_iterator i = objects.begin();
          i != objects.end(); ++i)
        {
          events.push_back(ServerEvent(*i));
        }
    }

  std::reverse(events.begin(), events.end());

  if (levelname.empty())
    PingusError::raise("XMLPDF: No level given");

  plf = XMLPingusLevel(path_manager.complete("levels/" + levelname + ".pingus"),
                       path_manager.complete("levels/" + levelname + ".pingus"));

  std::cout << "XXXXXXXXX Read Demo file: " << std::endl;
  write_xml(std::cout);
}

XMLPDF::~XMLPDF()
{
}

void
XMLPDF::write_xml(std::ostream& xml)
{
  xml << "<pingus-demo>\n"
      << "  <level>" << levelname << "</level>\n"
      << "  <events>\n";

  for(std::vector<ServerEvent>::iterator i = events.begin();
      i != events.end();
      ++i)
    i->write_xml(xml);

  xml << "  </events>\n";
}

/** @return a pointer to the level structure */
PingusLevel
XMLPDF::get_plf()
{
  return plf;
}

/** @return the levelname */
std::string
XMLPDF::get_levelname()
{
  return levelname;
}

/** Returns a reference to the events of this demo */
std::vector<ServerEvent>
XMLPDF::get_events()
{
  return events;
}


/* EOF */
