//  $Id: PingusWorldMapNodeData.cc,v 1.1 2002/06/04 21:23:42 grumbel Exp $
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

#include "../StringConverter.hh"
#include "PingusWorldMapNode.hh"
#include "PingusWorldMapNodeData.hh"

PingusWorldMapNode*
PingusWorldMapNodeData::create ()
{
  /* FIXME: This looks clearly like a design bug. This class should
     FIXME: probally be split of even more */
  assert (!"PingusWorldMapNode* create (): This should never be reached!");
  return 0; 
}

PingusWorldMapNode*
PingusWorldMapTubeNodeData::create ()
{
  return new PingusWorldMapTubeNode (*this);
}

PingusWorldMapNode*
PingusWorldMapLevelNodeData::create ()
{
  return new PingusWorldMapLevelNode (*this);
}

PingusWorldMapNode*
PingusWorldMapEmptyNodeData::create ()
{
  return 0;//FIXME:new PingusWorldMapEmptyNode (*this);
}

PingusWorldMapNodeData*
PingusWorldMapNodeData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  PingusWorldMapNodeData* node = new PingusWorldMapNodeData ();
  
  char* id = (char*)xmlGetProp(cur, (xmlChar*)"id");
  if (id)
    node->id = StringConverter::to_int (id);
  else
    std::cout << "PingusWorldMapGraph::parse_node: no node id given" << std::endl;

  /* Accesibility should probally be placed on the links?!
  char* accessible = (char*)xmlGetProp(cur, (xmlChar*)"accessible");
  if (accessible)
    node->accessible = StringConverter::to_int (accessible);
  */

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

  return node;
}

PingusWorldMapTubeNodeData*
PingusWorldMapTubeNodeData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  PingusWorldMapTubeNodeData* node = new PingusWorldMapTubeNodeData ();

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
	  PingusWorldMapNodeData* node_data = PingusWorldMapNodeData::create (doc, cur);
	  node->assign(*node_data);
	  delete node_data;
	}
      else if (strcmp((char*)cur->name, "worldmap") == 0)
	{
	  char* link_node = (char*)xmlGetProp(cur, (xmlChar*)"linknode");
	  if (link_node)
	    from_string (link_node, node->link_node);
	  else
	    std::cout << "PingusWorldMapGraph::parse_tube: no node 'linknode' given" << std::endl;

	  node->worldmap = XMLhelper::parse_string (doc, cur);
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
	  std::cout << "PingusWorldMapGraph: unhandled" << std::endl;
	}

      cur = cur->next;
    }
  return node;
}


PingusWorldMapLevelNodeData*
PingusWorldMapLevelNodeData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  PingusWorldMapLevelNodeData* node = new PingusWorldMapLevelNodeData ();

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
	  PingusWorldMapNodeData* node_data = PingusWorldMapNodeData::create (doc, cur);
	  node->assign(*node_data);
	  delete node_data;
	}
      else if (strcmp((char*)cur->name, "level") == 0)
	{
	  char* level = (char*)xmlGetProp(cur, (xmlChar*)"name");
	  if (level)
	    node->levelname = std::string("levels/") + level;
	  else
	    std::cout << "PingusWorldMapGraph::parse_node: no levelname given" << std::endl;
	}
      else
	{
	  std::cout << "PingusWorldMapLevelNodeData::create: unhandled" << std::endl;
	}

      cur = cur->next;
    }
  
  return node;  
}

PingusWorldMapNodeData* 
PingusWorldMapEmptyNodeData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  return new PingusWorldMapEmptyNodeData ();
}

/* EOF */
