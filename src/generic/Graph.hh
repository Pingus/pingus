//  $Id: Graph.hh,v 1.5 2001/06/11 08:45:22 grumbel Exp $
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

class GraphNode
{
private:
  std::list<boost::dummy_ptr<GraphNode> > next_nodes;
  
public:

  GraphNode ()
  {}

  /// The distance between this node and another node
  virtual float distance (boost::dummy_ptr<GraphNode> other_node) =0;

  //virtual float h_distance (boost::dummy_ptr<GraphNode> other_node) =0;
  
  void connect (GraphNode<T>* next_node)
  {
    for (NodeIter i = next_nodes.begin (); i != next_nodes.end (); ++i)
      {
	// We are already connected to that node, so return
	if (*i == next_node)
	  return;
      }
    next_nodes.push_back (next_node);
  }
  
  /** Connect a node with another node in both direction, when you
      want a bidirectional graph, you only need this one, and not
      connect () */
  void biconnect (GraphNode<T>* next_node)
  {
    next_node->connect (this);
    connect (next_node);
  }
};

/** A general class for handling a bidirectional graph */
template<class T>
class Graph
{
public:
  std::list<boost::shared_ptr<GraphNode> > nodes;
  typedef std::list<boost::shared_ptr<GraphNode> >::iterator NodeIter;
  

  Graph ()
  {
  }

  ~Graph () 
  {
  }

  void add (const GraphNode<T>& node)
  {
    nodes.push_back (node);
  }

  void add (const T& data)
  {
    nodes.push_back (GraphNode<T>(data));
  }
  
  GraphNode<T>* find (const T& data)
  {
    for (NodeIter i = nodes.begin (); nodes.end (); ++i)
      {
	if (data == i->data)
	  return &(*i);
      }
    return 0;
  }

  /// Returns true if the graph doesn't contain any nodes
  bool empty() const
  {
    return nodes.empty ();
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
    current_node->attach (current_node, node);
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
