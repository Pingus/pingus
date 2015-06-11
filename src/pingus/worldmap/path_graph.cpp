// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdexcept>

#include "pingus/globals.hpp"
#include "pingus/worldmap/dot.hpp"
#include "pingus/worldmap/dot_factory.hpp"
#include "pingus/worldmap/path_drawable.hpp"
#include "pingus/worldmap/worldmap.hpp"
#include "util/log.hpp"
#include "util/raise_exception.hpp"
#include "util/mem.hpp"

namespace WorldmapNS {

PathGraph::PathGraph(Worldmap* arg_worldmap, const ReaderMapping& reader) :
  worldmap(arg_worldmap),
  graph(),
  dots(),
  pathfinder_cache(),
  node_lookup(),
  edge_lookup()
{
  ReaderCollection nodes_collection;
  reader.read_collection("nodes", nodes_collection);
  parse_nodes(nodes_collection);

  ReaderCollection edges_collection;
  reader.read_collection("edges", edges_collection);
  parse_edges(edges_collection);

  init_cache();
}

void delete_Path(Edge<Path*> x)
{
  delete x.data;
}

PathGraph::~PathGraph()
{
  graph.for_each_edge(delete_Path);
  for(PFinderCache::iterator i = pathfinder_cache.begin();
      i != pathfinder_cache.end(); ++i)
    delete *i;
}

void
PathGraph::parse_nodes(const ReaderCollection& collection)
{
  std::vector<ReaderObject> childs = collection.get_objects();
  for(auto i = childs.begin(); i != childs.end(); ++i)
  {
    std::unique_ptr<Dot> dot = DotFactory::create(*i);
    if (dot)
    {
      // add the dot to the pathfinding
      NodeId id = graph.add_node(dot.get());

      //log_info("Adding to lookup table: " << dot->get_name());
      node_lookup[dot->get_name()] = id;

      // FIXME: should be use this for freeing the stuff?
      dots.push_back(dot.get());

      // add the dot to the list of drawables
      if (worldmap)
        worldmap->add_drawable(std::move(dot));

    }
    else
    {
      log_info("PathGraph: Couldn't create node");
    }
  }
}

void
PathGraph::parse_edges(const ReaderCollection& collection)
{
  std::vector<ReaderObject> childs = collection.get_objects();

  for(auto i = childs.begin(); i != childs.end(); ++i)
  {
    if (i->get_name() == "edge")
    {
      ReaderMapping reader = i->get_mapping();

      std::string name;
      std::string source;
      std::string destination;

      reader.read_string("name",   name);
      reader.read_string("source", source);
      reader.read_string("destination", destination);

      auto path = util::make_unique<Path>();
      auto path2 = util::make_unique<Path>();

      std::vector<Vector3f> positions;
      if (reader.read_vectors("positions", positions))
      {
        for(auto const& p : positions)
        {
          path->push_back(p);
        }
      }

      Path full_path;
      full_path.push_back(graph.resolve_node(lookup_node(source)).data->get_pos());
      full_path.insert(*path);
      full_path.push_back(graph.resolve_node(lookup_node(destination)).data->get_pos());

      // FIXME: merge this together with the Pingus::distance() stuff in a seperate Path class
      float cost = full_path.length();

      if (worldmap && globals::developer_mode)
        worldmap->add_drawable(util::make_unique<PathDrawable>(full_path));

      path2->reverse_insert(*path);

      // FIXME: No error checking,
      EdgeId id1 = graph.add_edge(path.release(), // FIXME: Memory leak!
                                  lookup_node(destination), lookup_node(source),
                                  cost /* costs */);

      //EdgeId id2 =
      graph.add_edge(path2.release(), // FIXME: Memory leak!
                     lookup_node(source), lookup_node(destination),
                     cost /* costs */);

      //log_info("Cost: " << cost);

      // FIXME: edge lookup is flawed, since we have different edges in both directions

      edge_lookup[name] = id1;
    }
    else
    {
      raise_exception(std::runtime_error, "PathGraph: unhandled: ");
    }
  }
}

PathfinderResult
PathGraph::get_path(NodeId start_id, NodeId end_id)
{
  Pathfinder<Dot*,Path*>*& pfinder = pathfinder_cache[static_cast<size_t>(start_id)];

  if (!pfinder)
  {
    pfinder = new Pathfinder<Dot*, Path*>(graph, start_id);
    pathfinder_cache[static_cast<size_t>(start_id)] = pfinder;
  }

  return pfinder->get_result(end_id);
}

EdgeId
PathGraph::lookup_edge(const std::string& name)
{
  std::map<std::string, EdgeId>::iterator i = edge_lookup.find(name);
  if (i == edge_lookup.end())
  {
    log_info("Couldn't find EdgeId for: %1%", name);
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
    log_info("Couldn't find NodeId for: %1%", name);
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
  log_info("PathGraph: Couldn't find id: %1%", id);
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
  log_info("PathGraph: Couldn't find id: %1%", id);
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

    if (Math::sqrt(x*x + y*y) < 30.0f)
      return *i;
  }
  return 0;
}

NodeId
PathGraph::get_id(Dot* dot)
{
  for(std::vector<Dot*>::iterator i = dots.begin(); i != dots.end(); ++i)
    if (dot == *i)
      return static_cast<int>(i - dots.begin());
  return NoNode;
}

void
PathGraph::init_cache()
{
  // Init the pathfinder cache
  pathfinder_cache.resize(static_cast<size_t>(graph.max_node_handler_value()));
  for(PFinderCache::iterator i = pathfinder_cache.begin();
      i != pathfinder_cache.end(); ++i)
    *i = 0;
}

} // namespace WorldmapNS

/* EOF */
