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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_PATHFINDER_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_PATHFINDER_HPP

#include <queue>

#include "pingus/worldmap/graph.hpp"

namespace WorldmapNS {

struct PathfinderResult
{
  std::vector<NodeId> path;
  float cost;

  PathfinderResult() :
    path(),
    cost()
  {}
};

template<class T, class C>
class Pathfinder
{
public:
  class NodeStat
  {
  public:
    enum { CLOSED, UNKNOWN, OPEN} status;

    NodeId parent;
    float  cost;
    NodeId handle;

    NodeStat ()
      : status (UNKNOWN),
        parent(-1),
        cost(0),
        handle (-1)
    {
    }
  };

private:
  struct CostComp
  {
    Pathfinder<T, C>& pathfinder;

    CostComp(Pathfinder<T, C>& p) : pathfinder(p) {}

    bool operator()(NodeId a, NodeId b)
    {
      //log_info("Coast: " << pathfinder.stat_graph[a].cost << " " << pathfinder.stat_graph[b].cost);
      return pathfinder.stat_graph[static_cast<size_t>(a)].cost > pathfinder.stat_graph[static_cast<size_t>(b)].cost;
    }
  };

  friend struct CostComp;

  Graph<T, C>& graph;
  NodeId start;
  std::priority_queue<NodeId, std::vector<NodeId>, CostComp> open_nodes;
  std::vector<NodeStat>   stat_graph;

public:
  /** Find pathes to all other nodes, by starting from the node \a s */
  Pathfinder (Graph<T, C>& g, NodeId s) :
    graph(g),
    start(s),
    open_nodes(CostComp(*this)),
    stat_graph()
  {
    stat_graph.resize(static_cast<size_t>(graph.max_node_handler_value()));
    push_to_open(start);

    while (!open_nodes.empty())
    {
      NodeId current = open_nodes.top ();
      open_nodes.pop ();

      Node<T>& node = graph.resolve_node (current);
      for (std::vector<EdgeId>::iterator e = node.next.begin ();
           e != node.next.end ();
           ++e)
      {
        NodeId child_node = graph.resolve_edge(*e).destination;
        NodeStat& stat = stat_graph[static_cast<size_t>(child_node)];
        float new_cost = stat_graph[static_cast<size_t>(current)].cost + graph.resolve_edge(*e).cost;

        if (stat.status == NodeStat::OPEN
            && stat.cost <= new_cost)
        {
          // do nothing, already now a better path
        }
        else
        {
          stat_graph[static_cast<size_t>(child_node)].parent = current;
          stat_graph[static_cast<size_t>(child_node)].cost   = new_cost;

          if (!is_open(child_node))
            push_to_open(child_node);
        }
      }
    }
    //log_info("---DONE---");
  }

  /** The nodes to walk to reach end is returned in reverse order! so
      you have to handle the vector like a stack with .back() == .top() */
  std::vector<NodeId> get_path(NodeId end)
  {
    std::vector<NodeId> path;
    NodeId handle = end;

    do
    {
      path.push_back(handle);

      if (handle == start)
      {
        return path;
      }
      else if (handle == -1)
      {
        // no path found
        return  std::vector<NodeId>();
      }
      else
      {
        handle = stat_graph[static_cast<size_t>(handle)].parent;
      }
    }
    while (1);
  }

  /** @return the cost of the path to node */
  float get_cost(NodeId node)
  {
    return stat_graph[static_cast<size_t>(node)].cost;
  }

  PathfinderResult get_result(NodeId node)
  {
    PathfinderResult res;
    res.path = get_path(node);
    res.cost = get_cost(node);
    return res;
  }

  void push_to_open (NodeId handle)
  {
    open_nodes.push (handle);
    stat_graph[static_cast<size_t>(handle)].status = NodeStat::OPEN;
  }

  bool is_open (NodeId handle)
  {
    return stat_graph[static_cast<size_t>(handle)].status == NodeStat::OPEN;
  }

private:
  Pathfinder (const Pathfinder&);
  Pathfinder operator= (const Pathfinder&);
};

} // namespace WorldmapNS

#endif

/* EOF */
