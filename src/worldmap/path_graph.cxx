//  $Id: path_graph.cxx,v 1.3 2002/10/13 13:34:40 grumbel Exp $
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

#include <iostream>
#include "../xml_helper.hxx"
#include "../pingus_error.hxx"
#include "dot.hxx"
#include "dot_factory.hxx"
#include "worldmap.hxx"
#include "path_graph.hxx"

namespace WorldMapNS {

PathGraph::PathGraph(WorldMap* arg_worldmap, xmlDocPtr doc, xmlNodePtr cur)
  : worldmap(arg_worldmap)
{
  std::cout << "PathGraph::PathGraph(WorldMap* arg_worldmap, xmlDocPtr doc, xmlNodePtr cur)" << std::endl;

  // cur is at <graph>...
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);
  
  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, "nodes"))
        {
          parse_nodes(doc, cur);
        }
      else if (XMLhelper::equal_str(cur->name, "edges"))
        {
          parse_edges(doc, cur);
        }
      else
        {
          std::cout << "PathGraph: Can't parse: " << cur->name << std::endl;
        }

      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }
}

void
PathGraph::parse_nodes(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);

  std::cout << "PathGraph::parse_nodes(xmlDocPtr doc, xmlNodePtr cur)" << std::endl;
  
  while (cur)
    {
      std::cout << "parsing nodes" << std::endl;

      Dot* dot = DotFactory::create(doc, cur);
      if (dot)
        {
          // add the dot to the pathfinding
          NodeId id = graph.add_node(dot);
          node_lookup[dot->get_name()] = id;

          // add the dot to the list of drawables
          worldmap->add_drawable(dot);
        }
      else
        {
          std::cout << "PathGraph: Couldn't create node" << std::endl;
        }

      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }  
}

void
PathGraph::parse_edges(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);

  std::cout << "PathGraph::parse_edges(xmlDocPtr doc, xmlNodePtr cur)" << std::endl;

  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, "edge"))
        {
          std::string name;
          std::string source;
          std::string destination;

          if (!XMLhelper::get_prop(cur, "name", name))
            {
              PingusError::raise("PathGraph: Missing Edge Name");
            }

          if (!XMLhelper::get_prop(cur, "source", source))
            {
              PingusError::raise("PathGraph: Missing Source Name");
            }

          if (!XMLhelper::get_prop(cur, "destination", destination))
            {
              PingusError::raise("PathGraph: Missing Destination Name");
            }

          // FIXME: add path-data parsing here
          Path* path = new Path();
          xmlNodePtr child_cur = cur;
          while (child_cur)
            {
              if (XMLhelper::equal_str(child_cur->name, "position"))
                {
                  Vector pos = XMLhelper::parse_vector(doc, child_cur);
                  path->push_back(pos);
                }
              else
                {
                  std::cout << "12929929" << std::endl;
                }
              
              child_cur = child_cur->next;
              child_cur = XMLhelper::skip_blank(child_cur);
            }    
          
          // FIXME: No error checking, 
          graph.add_bi_edge(path, // FIXME: Memory leak!
                            node_lookup[source], node_lookup[destination], 
                            0 /* costs */);
        }
      else
        {
          PingusError::raise("PathGraph: unhandled: ");
        }

      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }    
}

std::vector<Vector>
PathGraph::get_path(NodeId start, NodeId end)
{
  // insert pathfinding magic here...
  return std::vector<Vector>();
}

} // namespace WorldMapNS

/* EOF */
