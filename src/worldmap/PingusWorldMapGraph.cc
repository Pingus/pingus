//  $Id: PingusWorldMapGraph.cc,v 1.24 2002/01/04 01:19:47 grumbel Exp $
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
#include "../globals.hh"
#include "../PingusError.hh"
#include "../XMLPLF.hh"
#include "../Console.hh"
#include "../XMLhelper.hh"
#include "../StringConverter.hh"
#include "../PingusSound.hh"
#include "../PingusGameSession.hh"
#include "PingusWorldMapManager.hh"
#include "PingusWorldMapGraph.hh"

PingusWorldMapTubeNode::PingusWorldMapTubeNode ()
  : worldmap_name ("pacman.xml"),
    tube ("worldmap/tube", "core")
{
  tube.set_align (-16, -32);
}

void 
PingusWorldMapTubeNode::on_click ()
{
  std::cout << "Not Supported" << std::endl;
  PingusWorldMapManager::get_current ()->change_map (worldmap_name, link_node);
}

void 
PingusWorldMapTubeNode::draw (CL_Vector offset)
{
  tube.put_screen (pos + offset);
}

std::map<std::string, std::string>
PingusWorldMapTubeNode::get_string ()
{
  std::map<std::string, std::string> str;
  str["en"] = "Tube";

  return str;
}

void 
PingusWorldMapLevelNode::on_click ()
{
  if (maintainer_mode) 
    std::cout << "Start a level...: " << levelname << std::endl;
		  
  PingusSound::play_sound("sounds/letsgo.wav");
  PingusGameSession game (levelname);
      
  // Launch the game and wait until it is finished
  game.start ();

  if (game.get_result ().finished ())
    {
      finished = true;	
    }
  else
    {
      console.puts("Please try again!");
    }
}

void 
PingusWorldMapLevelNode::mark (bool value)
{
  finished = value;
}

void 
PingusWorldMapLevelNode::draw (CL_Vector offset)
{
  if (!levelname.empty())
    {
      if (accessible) 
	{
	  green_dot.put_screen (pos + offset);
	  if (finished) {
	    green_flag.put_screen (pos + offset);
	  }
	}
      else
	{
	  red_dot.put_screen (pos + offset);
	}
    }
}

std::map<std::string, std::string>
PingusWorldMapLevelNode::get_string ()
{
  return get_plf ()->get_levelname ();
}

boost::shared_ptr<PLF>
PingusWorldMapLevelNode::get_plf ()
{
  if (plf.get () == 0) 
    {
      //console << "Loading " << levelname << std::endl;
      try {
	if (broken_clanlib_resource_handling)
	  plf = boost::shared_ptr<PLF> (new XMLPLF ("../" + levelname));
	else
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
      else if (strcmp((char*)cur->name, "tube") == 0)
	{
	  parse_tube (cur);
	}
      else if (strcmp((char*)cur->name, "comment") == 0)
	{
	  // ignore
	}
      else
	{
	  printf("PingusWorldMapGraph:parse_node_list: Unhandled: %s\n", (char*)cur->name);	  
	}
      cur = cur->next;
    } 
}

void 
PingusWorldMapGraph::parse_tube (xmlNodePtr cur)
{
  PingusWorldMapTubeNode* node = new PingusWorldMapTubeNode ();

  char* id = (char*)xmlGetProp(cur, (xmlChar*)"id");
  if (id)
    node->id = StringConverter::to_int (id);
  else
    std::cout << "PingusWorldMapGraph::parse_tube: no node 'id' given" << std::endl;

  cur = cur->children;
  
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (strcmp((char*)cur->name, "position") == 0)
	{
	  node->pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (strcmp((char*)cur->name, "worldmap") == 0)
	{
	  char* link_node = (char*)xmlGetProp(cur, (xmlChar*)"linknode");
	  if (link_node)
	    from_string (link_node, node->link_node);
	  else
	    std::cout << "PingusWorldMapGraph::parse_tube: no node 'linknode' given" << std::endl;

	  node->worldmap_name = XMLhelper::parse_string (doc, cur);
	}
      else if (strcmp((char*)cur->name, "link") == 0)
	{
	  char* id = (char*)xmlGetProp(cur, (xmlChar*)"id");
	  if (id)
	    node->links.push_back(StringConverter::to_int (id));
	  else
	    std::cout << "PingusWorldMapGraph::parse_node: no id given" << std::endl;	    
	}

      cur = cur->next;
    }
  
  nodes.push_back (boost::shared_ptr<class PingusWorldMapNode>(node));
}

void
PingusWorldMapGraph::parse_node (xmlNodePtr cur)
{
  PingusWorldMapLevelNode* node = new PingusWorldMapLevelNode ();

  char* id = (char*)xmlGetProp(cur, (xmlChar*)"id");
  if (id)
    node->id = StringConverter::to_int (id);
  else
    std::cout << "PingusWorldMapGraph::parse_node: no node id given" << std::endl;
  
  char* accessible = (char*)xmlGetProp(cur, (xmlChar*)"accessible");
  if (accessible)
    node->accessible = StringConverter::to_int (accessible);
  
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
	    node->levelname = std::string("levels/") + level;
	  else
	    std::cout << "PingusWorldMapGraph::parse_node: no levelname given" << std::endl;
	}
      else if (strcmp((char*)cur->name, "position") == 0)
	{
	  node->pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (strcmp((char*)cur->name, "link") == 0)
	{
	  char* id = (char*)xmlGetProp(cur, (xmlChar*)"id");
	  if (id)
	    node->links.push_back(StringConverter::to_int (id));
	  else
	    std::cout << "PingusWorldMapGraph::parse_node: no id given" << std::endl;	    
	}
      else
	{
	  printf("PingusWorldMapGraph:parse_node: Unhandled: %s\n", (char*)cur->name);	  
	}

      cur = cur->next;
    }  

  nodes.push_back (boost::shared_ptr<class PingusWorldMapNode>(node));
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

  for (iterator i = nodes.begin();
       i != nodes.end();
       ++i)
    {
      for (iterator j = nodes.begin();
	   j != nodes.end();
	   ++j)
	{
	  for (std::list<int>::iterator k = (*i)->links.begin();
	       k != (*i)->links.end();
	       ++k)
	    if ((*j)->id == *k)
	      CL_Display::draw_line ((int)((*j)->pos.x + offset.x), (int)((*j)->pos.y + offset.y),
				     (int)((*i)->pos.x + offset.x), (int)((*i)->pos.y + offset.y),
				     1.0, 1.0, 1.0, 1.0);
	}
    }
}

      /* Fade out, fixme, doesn't work at the moment 
	 CL_SurfaceProvider* provider = new TargetProvider (target);
	 CL_Surface* sur = CL_Surface::create (provider);

	 for (int y = 0; y < CL_Display::get_height(); 
	 y += CL_Display::get_height() / 40)
	 {
	 CL_System::keep_alive ();
	 CL_Display::clear_display ();
	 sur->put_screen (0, y);
	 Display::flip_display ();
	 }
		      
	 delete sur;
	 delete provider;
      */

/* EOF */
