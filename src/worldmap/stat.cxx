//  $Id: stat.cxx,v 1.12 2002/10/12 00:49:10 torangan Exp $
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

namespace WorldMapNS {

PingusWorldMapNodeStat::PingusWorldMapNodeStat () 
  : finished(false),
    accessible(false),
    id(-1),
    percentage(25)
{
}

PingusWorldMapNodeStat::PingusWorldMapNodeStat (const PingusWorldMapNodeStat& old)
  : finished(old.finished),
    accessible(old.accessible),
    id(old.id),
    percentage(old.percentage),
    levelfile(old.levelfile),
    checksum(old.checksum)
{
}

PingusWorldMapNodeStat&
PingusWorldMapNodeStat::operator= (const PingusWorldMapNodeStat& old)
{
  if (this == &old)
    return *this;
    
  finished   = old.finished;
  accessible = old.accessible;
  id         = old.id;
  percentage = old.percentage;
  levelfile  = old.levelfile;
  checksum   = old.checksum;
  
  return *this;
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
PingusWorldMapStat::parse_file (std::string filename_)
{
  doc = xmlParseFile(filename.c_str());

  if (! doc)
    PingusError::raise("Couldn't open \"" + filename_ + "\" or syntax error.");

  xmlNodePtr cur = doc->ROOT;

  if (cur && XMLhelper::equal_str(cur->name, "pingus-worldmap-stat")) 
    {
      cur = cur->children;
      while (cur)
	{
	  if (xmlIsBlankNode(cur)) 
	    {
	      cur = cur->next;
	      continue;
	    }

	  if (XMLhelper::equal_str(cur->name, "node"))
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
  if (!XMLhelper::get_prop(cur, "id", node.id))
    std::cout << "PingusWorldMapStat: id missing" << std::endl;

  //std::cout << "Parsing node: " << cur->name << std::endl;
    
  XMLhelper::get_prop(cur, "accessible", node.accessible);
  XMLhelper::get_prop(cur, "finished", node.finished);
  //XMLhelper::get_prop(cur, "checksum", node.checksum);
    
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
PingusWorldMapStat::save (std::list<boost::shared_ptr<Node> >& nodes)
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

} // namespace WorldMapNS

/* EOF */
