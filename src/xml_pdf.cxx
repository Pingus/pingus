//  $Id: xml_pdf.cxx,v 1.9 2003/08/19 19:56:55 torangan Exp $
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
#include "pingus_error.hxx"
#include "plf.hxx"
#include "path_manager.hxx"
#include "xml_pdf.hxx"

XMLPDF::XMLPDF(const std::string& filename)
  : plf(0)
{
  xmlDocPtr doc = xmlParseFile(filename.c_str());
  if (doc == NULL)
    PingusError::raise("XMLPDF: Couldn't open \"" + filename + "\"");

  xmlNodePtr cur = doc->ROOT;

  if (XMLhelper::equal_str(cur->name, "pingus-demo"))
    {
      cur = cur->children;
      cur = XMLhelper::skip_blank(cur);

      while(cur)
	{
	  std::cout << "Token: " << cur->name << std::endl;

	  if (XMLhelper::equal_str(cur->name, "level"))
	    {
	      if (!XMLhelper::node_list_get_string(doc, cur->children, 1, levelname))
		{
		  std::cout << "XMLPDF: parse error at levelname" << std::endl;
		}
	      std::cout << "Level: " << levelname << std::endl;
	    }
	  else if (XMLhelper::equal_str(cur->name, "events"))
	    {
	      xmlNodePtr child_cur = cur->children;
	      child_cur = XMLhelper::skip_blank(child_cur);

	      while(child_cur)
		{
		  events.push_back(ServerEvent(doc, child_cur));
		  child_cur = child_cur->next;
		  child_cur = XMLhelper::skip_blank(child_cur);
		}
	    }
	  else
	    {
	      std::cout << "XMLPDF: Unhandled tag: " << cur->name << std::endl;
	    }

	  cur = cur->next;
	  cur = XMLhelper::skip_blank(cur);
	}
    }
  else
    {
      PingusError::raise("XMLPDF: Not a pingus-demo file");
    }

  xmlFreeDoc(doc);

  std::reverse(events.begin(), events.end());

  if (levelname.empty())
    PingusError::raise("XMLPDF: No level given");

  plf = PLF::create(path_manager.complete("levels/" + levelname + ".plf"));

  std::cout << "XXXXXXXXX Read Demo file: " << std::endl;
  write_xml(std::cout);
}

XMLPDF::~XMLPDF()
{
  delete plf;
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
PLF*
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
