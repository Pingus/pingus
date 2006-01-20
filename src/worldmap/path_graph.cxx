//  $Id: path_graph.cxx,v 1.23 2003/10/20 19:28:55 grumbel Exp $
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
#include <math.h>
#include "../file_reader.hxx"
#include "../pingus_error.hxx"
#include "dot.hxx"
#include "dot_factory.hxx"
#include "worldmap.hxx"
#include "metamap.hxx"
#include "pathfinder.hxx"
#include "path_graph.hxx"
#include "path_drawable.hxx"
#include "../globals.hxx"

namespace Pingus {
namespace WorldMapNS {

PathGraph::PathGraph(WorldMap* arg_worldmap, FileReader &reader)
  : worldmap(arg_worldmap),
	metamap(0)
{
  parse_nodes(reader.read_section("nodes"));
  parse_edges(reader.read_section("edges"));
  
	init_cache();
}

PathGraph::PathGraph(MetaMap* arg_metamap, FileReader &reader)
  : worldmap(0),
	metamap(arg_metamap) 
	
{
  parse_nodes(reader.read_section("nodes"));
  parse_edges(reader.read_section("edges"));
  
	init_cache();
}

void delete_Path(Edge<Path*> x)
{
	delete x.data;
}

PathGraph::~PathGraph()
{
	graph.for_each_edge(delete_Path);
}

void
PathGraph::parse_nodes(FileReader reader)
{
  const std::vector<FileReader>& childs = reader.get_sections();

  for(std::vector<FileReader>::const_iterator i = childs.begin(); 
      i != childs.end(); ++i)
    {
      Dot* dot = DotFactory::create(*i);
      if (dot)
        {
          // add the dot to the pathfinding
          NodeId id = graph.add_node(dot);

          //std::cout << "Adding to lookup table: " << dot->get_name() << std::endl;
          node_lookup[dot->get_name()] = id;

          // add the dot to the list of drawables
					if (worldmap)
						worldmap->add_drawable(dot);
					else
						metamap->add_drawable(dot);

          // FIXME: should be use this for freeing the stuff?
          dots.push_back(dot);
        }
      else
        {
          std::cout << "PathGraph: Couldn't create node" << std::endl;
        }
    }
}

void
PathGraph::parse_edges(FileReader reader)
{
  const std::vector<FileReader>& childs = reader.get_sections();

  for(std::vector<FileReader>::const_iterator i = childs.begin(); 
      i != childs.end(); ++i)
    {
      if (i->get_name() == "edge")
        {
          std::string name;
          std::string source;
          std::string destination;

          i->read_string("name",   name);
          i->read_string("source", source);
          i->read_string("destination", destination);
          
          // FIXME: add path-data parsing here
          Path* path = new Path();
          
          const std::vector<FileReader>& childs2 = reader.read_section("positions").get_sections();
          
          for(std::vector<FileReader>::const_iterator j = childs2.begin(); 
              j != childs2.end(); ++j)
            {
              if (j->get_name() == "position")
                {
                  Vector pos;
                  j->read_float("x", pos.x);
                  j->read_float("y", pos.y);
                  j->read_float("z", pos.z);
                  path->push_back(pos);
                }
            }

          Path full_path;
          full_path.push_back(graph.resolve_node(lookup_node(source)).data->get_pos());
          full_path.insert(*path);
          full_path.push_back(graph.resolve_node(lookup_node(destination)).data->get_pos());

          // FIXME: merge this together with the Pingus::distance() stuff in a seperate Path class
          float cost = full_path.length();

          // FIXME: Memory leak
          if (worldmap && pingus_debug_flags & PINGUS_DEBUG_WORLDMAP)
            worldmap->add_drawable(new PathDrawable(full_path));

          // FIXME: No error checking,
          EdgeId id1 = graph.add_edge(path, // FIXME: Memory leak!
                                      lookup_node(destination), lookup_node(source),
                                      cost /* costs */);

          Path* path2 = new Path();
          path2->reverse_insert(*path);
          //EdgeId id2 =
          graph.add_edge(path2, // FIXME: Memory leak!
                         lookup_node(source), lookup_node(destination),
                         cost /* costs */);

          //std::cout << "Cost: " << cost << std::endl;

          // FIXME: edge lookup is flawed, since we have different edges in both directions

          edge_lookup[name] = id1;
        }
      else
        {
          PingusError::raise("PathGraph: unhandled: ");
        }
    }
}

PathfinderResult
PathGraph::get_path(NodeId start_id, NodeId end_id)
{
  Pathfinder<Dot*,Path*>*& pfinder = pathfinder_cache[start_id];

  if (!pfinder)
    {
      pfinder = new Pathfinder<Dot*, Path*>(graph, start_id);
    }

  return pfinder->get_result(end_id);
}

EdgeId
PathGraph::lookup_edge(const std::string& name)
{
  std::map<std::string, EdgeId>::iterator i = edge_lookup.find(name);
  if (i == edge_lookup.end())
    {
      std::cout << "Couldn't find EdgeId for: " << name << std::endl;
      return NoEdge;
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
      return NoNode;
    }
  else
    {
      return i->second;
    }
}

std::string
PathGraph::lookup_node(EdgeId id)
{
  for (std::map<std::string, NodeId>::iterator i = node_lookup.begin();
       i != node_lookup.end(); ++i)
    {
      if (i->second == id)
        {
          return i->first;
        }
    }
  std::cout << "PathGraph: Couldn't find id: " << id << std::endl;
  return "error_node";
}

std::string
PathGraph::lookup_edge(NodeId id)
{
  for (std::map<std::string, EdgeId>::iterator i = node_lookup.begin();
       i != node_lookup.end(); ++i)
    {
      if (i->second == id)
        {
          return i->first;
        }
    }
  std::cout << "PathGraph: Couldn't find id: " << id << std::endl;
  return "error_node";
}

Dot*
PathGraph::get_dot(NodeId id)
{
  return graph.resolve_node(id).data;
}

Dot*
PathGraph::get_dot(float x_pos, float y_pos)
{
  for(std::vector<Dot*>::iterator i = dots.begin(); i != dots.end(); ++i)
    {
      float x = x_pos - (*i)->get_pos().x;
      float y = y_pos - (*i)->get_pos().y;

      if (sqrt(x*x + y*y) < 30.0f)
        return *i;
    }
  return 0;
}

NodeId
PathGraph::get_id(Dot* dot)
{
  for(std::vector<Dot*>::iterator i = dots.begin(); i != dots.end(); ++i)
    if (dot == *i)
      return (int)(i - dots.begin());
  return NoNode;
}

void
PathGraph::init_cache()
{
	// Init the pathfinder cache
	pathfinder_cache.resize(graph.max_node_handler_value());
	for(PFinderCache::iterator i = pathfinder_cache.begin();
		i != pathfinder_cache.end(); ++i)
		*i = 0;
}

} // namespace WorldMapNS
} // namespace Pingus

/* EOF */
