//  $Id: stat.cxx,v 1.2 2002/06/13 19:53:22 torangan Exp $
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

#include <fstream>
#include <iostream>
#include "../system.hxx"
#include "../string_converter.hxx"
#include "../pingus_error.hxx"
#include "../xml_helper.hxx"
#include "stat.hxx"

using namespace Pingus::WorldMap;

PingusWorldMapNodeStat::PingusWorldMapNodeStat ()
{
  id = -1;
  finished = false;
  accessible = false;
  percentage = 25;
}

PingusWorldMapStat::PingusWorldMapStat (std::string worldmap_name)
  : is_empty (true)
{
  filename = System::get_statdir () + "stat/" + worldmap_name;

  if (System::exist (filename))
    {
      parse_file (filename);
      is_empty = false;
    }
}

void 
PingusWorldMapStat::parse_file (std::string filename)
{
  doc = xmlParseFile(filename.c_str());

  if (doc == NULL)
    throw PingusError("Couldn't open \"" + filename + "\" or syntax error.");

  xmlNodePtr cur = doc->ROOT;

  if (cur && strcmp((const char*)cur->name, "pingus-worldmap-stat") == 0) 
    {
      cur = cur->children;
      while (cur != NULL)
	{
	  if (xmlIsBlankNode(cur)) 
	    {
	      cur = cur->next;
	      continue;
	    }

	  if (strcmp((char*)cur->name, "node") == 0)
	    {
	      parse_node (cur);
	    }
	  else
	    {
	      std::cout << "PingusWorldMapStat: Unhandled:" << cur->name << std::endl;
	    }
	  
	  cur = cur->next;
	}
    }
  else
    {
      std::cout << "Not a pingus-worldmap-stat: " << cur->name << std::endl;
    }

  xmlFreeDoc(doc);
}

void 
PingusWorldMapStat::parse_node (xmlNodePtr cur)
{
  PingusWorldMapNodeStat node;
  char* id         = (char*)xmlGetProp(cur, (xmlChar*)"id");
  char* accessible = (char*)xmlGetProp(cur, (xmlChar*)"accessible");
  char* finished   = (char*)xmlGetProp(cur, (xmlChar*)"finished");
  char* checksum   = (char*)xmlGetProp(cur, (xmlChar*)"checksum");

  //std::cout << "Parsing node: " << cur->name << std::endl;

  if (id) {
    from_string (id, node.id);
    free (id);
  } else {
    std::cout << "PingusWorldMapStat: id missing" << std::endl;
  }
  
  if (accessible) {
    node.accessible = StringConverter::to_int (accessible);
    free (accessible);    
  }
  
  if (finished) {
    node.finished = StringConverter::to_int (finished);
    free (finished);  
  }
  
  if (checksum) {
    //node.checksum = StringConverter::to_int (checksum);
    free (checksum);
  }

  stats[node.id] = node;
}

PingusWorldMapStat::~PingusWorldMapStat ()
{
}

bool 
PingusWorldMapStat::finished (int id)
{
  return stats[id].finished;
}

bool 
PingusWorldMapStat::accessible (int id)
{
  return stats[id].accessible;
}

void
PingusWorldMapStat::save (std::list<boost::shared_ptr<Pingus::WorldMap::Node> >& nodes)
{
  std::ofstream out (filename.c_str ());
  
  if (!out)
    std::cout << "PingusWorldMapStat: Unexpected IO ERROR" << std::endl;
  else
    {
      out << "<pingus-worldmap-stat>" << std::endl;
      for (Graph::iterator i = nodes.begin ();
	   i != nodes.end (); ++i)
	{
	  LevelNode* node = dynamic_cast<LevelNode*>((*i).get ());

	  if (node)
	    {
	      out << "  <node id=\"" << node->get_id () << "\" "
		//<< "accessible=\"" << node->accessible  << "\" "
		  << "finished=\"" << node->finished  << "\" "
		  << "checksum=\"not-impl\""
		  << "/>" << std::endl;
	    }
	}
      out << "</pingus-worldmap-stat>" << std::endl;
    }
}

/* EOF */
