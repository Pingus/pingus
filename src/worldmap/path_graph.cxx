//  $Id: path_graph.cxx,v 1.8 2002/10/13 23:02:29 grumbel Exp $
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
#include "pathfinder.hxx"
#include "path_graph.hxx"
#include "path_drawable.hxx"

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

          std::cout << "Adding to lookup table: " << dot->get_name() << std::endl;
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
          xmlNodePtr child_cur = XMLhelper::skip_blank(cur->children);
          while (child_cur)
            {
              if (XMLhelper::equal_str(child_cur->name, "position"))
                {
                  Vector pos = XMLhelper::parse_vector(doc, child_cur);
                  path->push_back(pos);
                }
              else
                {
                  std::cout << "12929929: " << child_cur->name  << std::endl;
                }
              
              child_cur = child_cur->next;
              child_cur = XMLhelper::skip_blank(child_cur);
            }

          Path full_path;
          full_path.push_back(graph.resolve_node(lookup_node(source)).data->get_pos());
          full_path.insert(full_path.end(), path->begin(), path->end());
          full_path.push_back(graph.resolve_node(lookup_node(destination)).data->get_pos());
          
          // FIXME: Memory leak
          worldmap->add_drawable(new PathDrawable(full_path));
          
          // FIXME: No error checking, 
          std::pair<EdgeId, EdgeId> id 
            = graph.add_bi_edge(path, // FIXME: Memory leak!
                                lookup_node(source), lookup_node(destination), 
                                0 /* costs */);
          edge_lookup[name] = id.first;
        }
      else
        {
          PingusError::raise("PathGraph: unhandled: ");
        }

      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }    
}

std::vector<NodeId>
PathGraph::get_path(NodeId start_id, NodeId end_id)
{
  //Node<Dot*>& start = graph.resolve_node(start_id);
  //Node<Dot*>& end   = graph.resolve_node(end_id);

  Pathfinder<Dot*, Path*> pathfinder(graph, start_id);
  std::vector<NodeId> path = pathfinder.get_path(end_id);

  assert(path.size() > 0);

  return path;
}

EdgeId
PathGraph::lookup_edge(const std::string& name)
{
  std::map<std::string, EdgeId>::iterator i = edge_lookup.find(name);
  if (i == edge_lookup.end())
    {
      std::cout << "Couldn't find EdgeId for: " << name << std::endl;
      return 0;
    }
  else
    {
      return i->second;
    }
}

NodeId
PathGraph::lookup_node(const std::string& name)
{
  std::map<std::string, NodeId>::iterator i = node_lookup.find(name);
  if (i == node_lookup.end())
    {
      std::cout << "Couldn't find NodeId for: " << name << std::endl;
      return 0;
    }
  else
    {
      return i->second;
    }
}

Dot*
PathGraph::get_dot(NodeId id)
{
  return graph.resolve_node(id).data; 
}

} // namespace WorldMapNS

/* EOF */
