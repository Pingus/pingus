//  $Id: PingusWorldMapGraph.cc,v 1.15 2001/05/22 16:10:12 grumbel Exp $
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

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "../my_gettext.hh"
#include "../PingusError.hh"
#include "../XMLPLF.hh"
#include "../Console.hh"
#include "../XMLhelper.hh"
#include "../StringConverter.hh"
#include "PingusWorldMapGraph.hh"

boost::shared_ptr<PLF>
PingusWorldMapNode::get_plf ()
{
  if (plf.get () == 0) 
    {
      console << "Loading levelname" << std::endl;
      try {
	plf = boost::shared_ptr<PLF> (new XMLPLF (levelname));
      } catch (PingusError e) {
	std::cout << "PingusWorldMapGraph: Caught PingusError (" << e.get_message () << ")" << std::endl;
	std::cout << "PingusWorldMapGraph: Failed to load '" << levelname << "', fallback to level1.xml" << std::endl;
	plf = boost::shared_ptr<PLF> (new XMLPLF ("levels/level1.xml"));
      }
    }
  
  return plf;
}

PingusWorldMapGraph::PingusWorldMapGraph ()
{
  music = "pingus-1.it";
  //graph = 0;
}

PingusWorldMapGraph::~PingusWorldMapGraph ()
{
  //llif (graph) delete graph;
}
  
void
PingusWorldMapGraph::parse_file (std::string filename)
{
  doc = xmlParseFile(filename.c_str());

  if (!doc) {
    throw PingusError (_("PingusWorldMapGraph: File not found: ") + filename);
  }    

  xmlNodePtr cur = doc->ROOT;

  if (cur != NULL && strcmp((const char*)cur->name, "pingus-worldmap") == 0)
    {
      cur = cur->children;
      
      while (cur != NULL)
	{
	  if (xmlIsBlankNode(cur)) 
	    {
	      cur = cur->next;
	      continue;
	    }

	  if (strcmp ((char*)cur->name, "node-list") == 0)
	    {
	      parse_node_list (cur);
	    }
	  else if (strcmp ((char*)cur->name, "surface") == 0)
	    {
	      parse_background (cur);
	    }
	  else if (strcmp ((char*)cur->name, "music") == 0)
	    {
	      parse_music (cur);
	    }
	  else
	    {
	      printf("PingusWorldMapGraph: Unhandled: %s\n", (char*)cur->name);
	    }
	  cur = cur->next;
	}      
    }
  
  xmlFreeDoc(doc);
}

void
PingusWorldMapGraph::parse_node_list (xmlNodePtr cur)
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
	  printf("PingusWorldMapGraph:parse_node_list: Unhandled: %s\n", (char*)cur->name);	  
	}
      cur = cur->next;
    } 
}

void
PingusWorldMapGraph::parse_node (xmlNodePtr cur)
{
  PingusWorldMapNode node;

  char* id = (char*)xmlGetProp(cur, (xmlChar*)"id");
  if (id)
    node.id = StringConverter::to_int (id);
  else
    std::cout << "PingusWorldMapGraph::parse_node: no node id given" << std::endl;

  char* accessible = (char*)xmlGetProp(cur, (xmlChar*)"accessible");
  if (accessible)
    node.accessible = StringConverter::to_int (accessible);
  
  cur = cur->children;

  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (strcmp((char*)cur->name, "level") == 0)
	{
	  char* level = (char*)xmlGetProp(cur, (xmlChar*)"name");
	  if (level)
	    node.levelname = std::string("levels/") + level;
	  else
	    std::cout << "PingusWorldMapGraph::parse_node: no levelname given" << std::endl;
	}
      else if (strcmp((char*)cur->name, "position") == 0)
	{
	  node.pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (strcmp((char*)cur->name, "link") == 0)
	{
	  char* id = (char*)xmlGetProp(cur, (xmlChar*)"id");
	  if (id)
	    node.links.push_back(StringConverter::to_int (id));
	  else
	    std::cout << "PingusWorldMapGraph::parse_node: no id given" << std::endl;	    
	}
      else
	{
	  printf("PingusWorldMapGraph:parse_node: Unhandled: %s\n", (char*)cur->name);	  
	}

      cur = cur->next;
    }  

  nodes.push_back (node);
  // FIXME: Add the node to the graph
}

void
PingusWorldMapGraph::parse_background (xmlNodePtr cur)
{
  bg_desc = XMLhelper::parse_surface(doc, cur);
}

void
PingusWorldMapGraph::parse_music (xmlNodePtr cur)
{
  char* file = (char*)xmlGetProp(cur, (xmlChar*)"file");

  if (file)
    music = file;
  else
    {
      std::cout << "PingusWorldMapGraph: No music file given" << std::endl;
    }
}

ResDescriptor 
PingusWorldMapGraph::get_background ()
{
  return bg_desc;
}

std::string 
PingusWorldMapGraph::get_music ()
{
  return music;
}

/*Graph<PingusWorldMapNode>* 
PingusWorldMapGraph::get_graph ()
{
  return graph;
}*/

void
PingusWorldMapGraph::draw (const CL_Vector& offset)
{
  //float x_scale = CL_Display::get_width () / 800.0;
  //float y_scale = CL_Display::get_height () / 600.0;

  for (std::list<PingusWorldMapNode>::iterator i = nodes.begin();
       i != nodes.end();
       ++i)
    {
      for (std::list<PingusWorldMapNode>::iterator j = nodes.begin();
	   j != nodes.end();
	   ++j)
	{
	  for (std::list<int>::iterator k = i->links.begin();
	       k != i->links.end();
	       ++k)
	    if (j->id == *k)
	      CL_Display::draw_line (j->pos.x + offset.x, j->pos.y + offset.y,
				     i->pos.x + offset.x, i->pos.y + offset.y,
				     1.0, 1.0, 1.0, 1.0);
	}
    }
}

/* EOF */
