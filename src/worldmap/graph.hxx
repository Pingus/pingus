//  $Id: graph.hxx,v 1.21 2003/04/18 12:48:50 grumbel Exp $
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

#ifndef HEADER_GRAPH_HXX
#define HEADER_GRAPH_HXX

#include <algorithm>
#include <assert.h>
#include <vector>

namespace WorldMapNS {

typedef int NodeId;
typedef int EdgeId;

extern const NodeId NoNode;
extern const EdgeId NoEdge;

template<class NodeType>
class Node 
{
public:
  Node (const NodeType& d) 
    : data (d)
  {}

  Node& operator= (const NodeType& d) 
  {
    data = d;
  }

  NodeType data;
  std::vector<EdgeId> next;
};

template<class EdgeType>
class Edge
{
public:
  NodeId source;
  NodeId destination;
  float cost;

  EdgeType data;

  Edge (const EdgeType& arg_data, const NodeId& s, const NodeId& d, float c)
    : source (s), destination (d), cost (c), data(arg_data)
  {
  }
};

template<class NodeType,
         class EdgeType>
class Graph
{
private:
  std::vector<Node<NodeType> > nodes;
  std::vector<Edge<EdgeType> > edges;
  
public:
  Graph () 
  {
  }

  Graph (const Graph&)
  {
    assert (false);
  }

  Graph& operator= (const Graph&)
  {
    assert (false);
  }

  ~Graph () 
  {
  }

  NodeId add_node (NodeType d)
  { 
    nodes.push_back (Node<NodeType>(d));
    return NodeId (nodes.size ()-1);
  }
  
  EdgeId add_edge (const EdgeType& data, const NodeId& a, const NodeId& b, float cost)
  {
    Edge<EdgeType> new_edge (data, a, b, cost);
    edges.push_back (new_edge);
    resolve_node (a).next.push_back (edges.size ()-1);
    return EdgeId (edges.size ()-1);
  }

  std::pair<EdgeId, EdgeId>
  add_bi_edge (const EdgeType& data, const NodeId& a, const NodeId& b, float cost)
  {
    std::pair<EdgeId, EdgeId> ret;
    ret.first  = add_edge (data, a, b, cost);
    ret.second = add_edge (data, b, a, cost);
    return ret;
  }

  void remove_node (const NodeId& node)
  {
    assert (!"remove_node: not implemented");
  }

  void remove_edge (const NodeId& node1, const NodeId& node2)
  {
    assert (!"remove_edge: not implemented");
  }

  Edge<EdgeType>& resolve_edge (const EdgeId& node)
  {
    // FIXME: No error handling
    return edges[node];
  }

  /** Translates a NodeId into the corresponding Node */
  Node<NodeType>& resolve_node (const NodeId& node)
  {
    // FIXME: No error handling
    return nodes[node];
  }

  Edge<EdgeType>& resolve_edge(const NodeId& source, const NodeId& destination)
  {
    // FIXME: this could be done faster with an adjacense matrix
    for (typename std::vector<Edge<EdgeType> >::iterator i = edges.begin();
         i != edges.end(); ++i)
      {
        if (i->source == source
            && i->destination == destination)
          return *i;
      }
    std::cout << "couldn't resolve edge: source=" << source << " destination=" << destination << std::endl;
    assert(false);
	return *((Edge<EdgeType>*) 0);
  }
  
  /* FIXME: This might give problems under MSVC, so it could be better to not use it */
  template<class Func>
  void for_each_node (Func func) 
  {
    std::for_each (nodes.begin (), nodes.end (), func);
  }

  template<class Func>
  void for_each_edge (Func func) 
  {
    std::for_each (edges.begin (), edges.end (), func);    
  }

  int nodes_size () {
    return nodes.size ();
  }
  
  int max_node_handler_value () {
    return nodes.size ();
  }
};

} // namespace WorldMapNS

#endif

/* EOF */
