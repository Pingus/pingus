//  $Id: Graph.hh,v 1.1 2000/09/19 10:40:38 grumbel Exp $
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

#ifndef GRAPH_HH
#define GRAPH_HH

#include <list>

template<class T> class GraphNode;
template<class T> class GraphIterator;
template<class T> class Graph;

template<class T>
class GraphNode
{
public:
  T data;
  list<GraphNode<T>*> attached_nodes;

  GraphNode () {}
  GraphNode (T obj) 
  {
    data = obj;
  }

  void attach(GraphNode<T>* next_node)
  {
    attached_nodes.push_back (next_node);
  }
};

/* A general class for handling a bidirectional graph */
template<class T>
class Graph
{
private:
  GraphNode<T>* first_node;

public:
  Graph () 
  {
    first_node = 0;
  }

  /// Create a new graph with a first element 
  Graph (T obj) 
  {
    first_node = new GraphNode<T>;
    first_node->data = obj;
  }
  
  ~Graph () 
  {
  }

  /// Returns an iterator which points to the first node
  GraphIterator<T> begin()
  {
    assert (first_node);
    return GraphIterator<T>(this, first_node);
  }

  /// Returns true if the graph doesn't contain any nodes
  bool empty() const
  {
    if (first_node) 
      return false;
    else
      return true;
  }
};

template<class T>
class GraphIterator
{
private:
  /// A pointer to the current node
  GraphNode<T>* current_node;
  Graph<T>*     graph;
  
public:

  GraphIterator(Graph<T>* arg_graph, GraphNode<T>* node)
  {
    current_node = node;
    graph = arg_graph;
  }

  GraphIterator(const GraphIterator<T> &obj)
  {
    current_node = obj.current_node;
  }

  /// Returns a list of nodes that are attached to this graph
  list<GraphIterator<T> > next_nodes()
  {
    list<GraphIterator<T> > next_nodes_list;
    
    // Expensive operation
    for (list<GraphNode<T>*>::iterator i = current_node->attached_nodes.begin();
	 i != current_node->attached_nodes.end();
	 i++)
      next_nodes_list.push_back(GraphIterator (graph, *i));
      
    return next_nodes_list;
  }

  /// Return the value of the current node
  T operator*()
  {
    assert (current_node);
    return current_node->data;
  }

  /// Set the value of the current node to obj
  const T& operator=(T obj)
  {
    current_node->data = obj;
    return obj;
  }

  /** Attach another node to the current node and return a Iterator to
     the new node */
  GraphIterator attach(T obj) 
  {
    GraphNode<T>* node = new GraphNode<T>(obj);
    current_node->attach (node);
    return GraphIterator (graph, node);
  }

  /// Returns true if there are no nodes attached to this node
  bool end()
  {
    if (next_nodes.size() == 0)
      return true;
    else
      return false;
  }
};

#endif

/* EOF */
