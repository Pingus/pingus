//  $Id: node_data.cxx,v 1.6 2002/09/27 11:26:49 torangan Exp $
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

#include "../string_converter.hxx"
#include "../xml_helper.hxx"
#include "node.hxx"

namespace WorldMapNS {

NodeData::NodeData (const NodeData& old) : id(old.id),
                                           links(old.links),
					   pos(old.pos)
{
}

NodeData&
NodeData::operator= (const NodeData& old)
{
  if (this == &old)
    return *this;
    
  id    = old.id;
  links = old.links;
  pos   = old.pos;
  
  return *this;
}

Node*
NodeData::create ()
{
  /* FIXME: This looks clearly like a design bug. This class should
     FIXME: probally be split of even more */
  assert (!"Node* create (): This should never be reached!");
  return 0; 
}

Node*
LevelNodeData::create ()
{
  return new LevelNode (*this);
}

LevelNodeData::LevelNodeData (const LevelNodeData& old) : NodeData(old),
                                                          levelname(old.levelname)
{
}

LevelNodeData&
LevelNodeData::operator= (const LevelNodeData& old)
{
  if (this == &old)
    return *this;
    
  NodeData::operator=(old);
  
  levelname = old.levelname;
  
  return *this;
}

LevelNodeData*
LevelNodeData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  LevelNodeData* node = new LevelNodeData ();

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
	  NodeData* node_data = NodeData::create (doc, cur);
	  node->assign(*node_data);
	  delete node_data;
	}
      else if (XMLhelper::equal_str(cur->name, "level"))
	{
	  char* level = XMLhelper::get_prop(cur, "name");
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

TubeNodeData::TubeNodeData (const TubeNodeData& old) : NodeData(old),
                                                       worldmap(old.worldmap),
						       link_node(old.link_node)
{
}

TubeNodeData&
TubeNodeData::operator= (const TubeNodeData& old)
{
  if (this == &old)
    return *this;
    
  NodeData::operator=(old);
  
  worldmap  = old.worldmap;
  link_node = old.link_node;
  
  return *this;
}
      
Node*
TubeNodeData::create ()
{
  return new TubeNode (*this);
}

Node*
EmptyNodeData::create ()
{
  return 0;//FIXME:new EmptyNode (*this);
}

NodeData*
NodeData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  NodeData* node = new NodeData ();
  
  char* id = XMLhelper::get_prop(cur, "id");
  if (id)
    node->id = StringConverter::to_int (id);
  else
    std::cout << "PingusWorldMapGraph::parse_node: no node id given" << std::endl;

  /* Accesibility should probally be placed on the links?!
  char* accessible = XMLhelper::get_prop(cur, "accessible");
  if (accessible)
    node->accessible = StringConverter::to_int (accessible);
  */

  cur = cur->children;

  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (XMLhelper::equal_str(cur->name, "position"))
	{
	  node->pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "link"))
	{
	  char* id = XMLhelper::get_prop(cur, "id");
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

TubeNodeData*
TubeNodeData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  TubeNodeData* node = new TubeNodeData ();

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
	  NodeData* node_data = NodeData::create (doc, cur);
	  node->assign(*node_data);
	  delete node_data;
	}
      else if (XMLhelper::equal_str(cur->name, "worldmap"))
	{
	  char* link_node = XMLhelper::get_prop(cur, "linknode");
	  if (link_node)
	    from_string (link_node, node->link_node);
	  else
	    std::cout << "PingusWorldMapGraph::parse_tube: no node 'linknode' given" << std::endl;

	  node->worldmap = XMLhelper::parse_string (doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "link"))
	{
	  char* id = XMLhelper::get_prop(cur, "id");
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

NodeData* 
EmptyNodeData::create(xmlDocPtr /*doc*/, xmlNodePtr /*cur*/)
{
  return new EmptyNodeData ();
}

} // namespace WorldMapNS

/* EOF */
