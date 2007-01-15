//  $Id: path_graph.hxx,v 1.12 2003/10/20 19:28:55 grumbel Exp $
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

#ifndef HEADER_PATH_GRAPH_HXX
#define HEADER_PATH_GRAPH_HXX

#include <map>
#include <vector>
#include "../math/vector3f.hpp"
#include "path.hxx"
#include "pathfinder.hxx"
#include "graph.hxx"

namespace WorldMapNS {

class Dot;
class WorldMap;
class MetaMap;

/** This class represents the walkable path on the Worldmap */
class PathGraph
{
private:
	// The pathgraph will have one of these, not both.
  WorldMap* worldmap;
	MetaMap* metamap;

public:
  // FIXME: Memory leak? Where do we free stuff data inside the graph?
  // FIXME: shouldn't be public
  Graph<Dot*, Path*> graph;

  std::vector<Dot*> dots;
  typedef std::vector<Pathfinder<Dot*, Path*>* > PFinderCache;
  PFinderCache pathfinder_cache;
private:

  // FIXME: This could/should probally be moved inside the graph (or not?!)
  /** Map to look up node names and get the coresponding id's */
  std::map<std::string, NodeId> node_lookup;

  /** Map to look up edge names and get the corresponding id's */
  std::map<std::string, EdgeId> edge_lookup;

public:
  /** @param worldmap is a pointer to the worldmap that holds this
      PathGraph, it is used so that the PathGraph can insert its
      drawables into the world */
  PathGraph(WorldMap* worldmap, FileReader &reader);
	PathGraph(MetaMap* metamap, FileReader &reader);

  ~PathGraph();

  /** @return a list of positions to walk to reach node \a end, by
      starting from \a start */
  PathfinderResult get_path(NodeId start, NodeId end);

  /** Get a node by it id */
  Dot* get_dot(NodeId id);

  NodeId get_id(Dot*);

  /** Get a node by its position */
  Dot* get_dot(float x, float y);

  EdgeId lookup_edge(const std::string& name);
  NodeId lookup_node(const std::string& name);

  std::string lookup_node(EdgeId id);
  std::string lookup_edge(NodeId id);

private:
  void parse_nodes(FileReader reader);
  void parse_edges(FileReader reader);
	void init_cache();

  PathGraph (const PathGraph&);
  PathGraph& operator= (const PathGraph&);
};

} // namespace WorldMapNS

#endif

/* EOF */
