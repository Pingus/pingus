//  $Id: pathfinder.hxx,v 1.1 2002/10/12 23:37:23 grumbel Exp $
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

#ifndef HEADER_PATHFINDER_HXX
#define HEADER_PATHFINDER_HXX

#include <queue>
#include "graph.hxx"

/** */
template<class T>
class Pathfinder
{
public:
  class NodeStat
  {
  public:
    enum { CLOSED, UNKNOWN, OPEN} status;

    NodeHandle parent;
    int cost;
    NodeHandle handle;

    NodeStat ()
      : status (UNKNOWN),
	parent(-1),
	cost(0),
	handle (-1)
    {
    }
  };

private:
  Graph<T>& graph;
  NodeHandle start;
  std::priority_queue<NodeHandle> open_nodes;
  std::vector<NodeStat>   stat_graph;
  
public:
  Pathfinder (Graph<T>& g, NodeHandle s)
    : graph (g), start (s)
  {
    stat_graph.resize (graph.max_node_handler_value());
    push_to_open (start);

    while (!open_nodes.empty())
      {
	NodeHandle current = open_nodes.top ();
	open_nodes.pop ();

	std::cout << "Current Node: " << current << " "
		  << stat_graph[current].cost << std::endl;

	Node<T>& node = graph.resolve_node (current);
	for (std::vector<EdgeHandle>::iterator e = node.next.begin ();
	     e != node.next.end ();
	     ++e)
	  {
	    NodeHandle child_node = graph.resolve_edge(*e).next;
	    NodeStat& stat = stat_graph[child_node];
	    int new_cost = stat_graph[current].cost + graph.resolve_edge(*e).cost;
	    
	    if  (stat.status == NodeStat::OPEN 
		 && stat.cost <= new_cost)
	      {
		// do nothing, already now a better path
	      }
	    else
	      {
		stat_graph[child_node].parent = current;
		stat_graph[child_node].cost   = new_cost;

		if (!is_open (child_node))
		  push_to_open (child_node);
	      }
	  }
      }
    std::cout << "---DONE---" << std::endl;
  }

  std::vector<NodeHandle> get_path (NodeHandle end) 
  {
    std::vector<NodeHandle> path;
    NodeHandle handle = end;
    
    do
      {
	path.push_back(handle);
	std::cout << "Handle: " << handle 
		  << " Parent: " << stat_graph[handle].parent << std::endl;

	if (handle == start)
	  {
	    std::reverse (path.begin (), path.end ());
	    return path;
	  }
	else if (handle == -1)
	  {
	    // no path found
	    return  std::vector<NodeHandle>();
	  }
	else
	  {
	    handle = stat_graph[handle].parent;
	  }	
      }
    while (1);
  }

  void push_to_open (NodeHandle handle)
  {
    open_nodes.push (handle);
    stat_graph[handle].status = NodeStat::OPEN;
  }

  bool is_open (NodeHandle handle)
  {
    return stat_graph[handle].status == NodeStat::OPEN;
  }

private:
  Pathfinder (const Pathfinder&);
  Pathfinder operator= (const Pathfinder&);
};

#endif

/* EOF */
