//  $Id: Graph.cc,v 1.2 2001/04/27 20:44:38 grumbel Exp $
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

#include "Graph.hh"

#ifdef DO_GRAPH_TEST

#include <iostream>

void display_subgraph(GraphIterator<int> graph);

int main ()
{
  Graph<CL_Vector> graph;

  graph.add (CL_Vector (10, 10));

  Graph<int> graph (5);
  GraphIterator<int> iter = graph.begin ();

  std::cout << "Graph constructed " << *(graph.begin()) << std::endl;

  iter = 2;

  GraphIterator<int> iter2 = iter.attach (6);

  std::cout << "Graph constructed " << *(graph.begin()) << std::endl;

  std::cout << "attached " << *iter2 << std::endl;

  GraphIterator<int> iter3 = iter2.attach (3);
  iter3.attach (32).attach (56);
  iter3.attach (101).attach (123);
  std::cout << "attached " << *iter3 << std::endl;

  GraphIterator<int> iter4 = iter2.attach (2);
  std::cout << "attached " << *iter4 << std::endl;


  display_subgraph(graph.begin());
  //  std::cout << "Graph constructed " << g << std::endl;
}

void display_subgraph(GraphIterator<int> graph)
{
  GraphIterator<int> iter = graph;
  std::cout << "Value: " << *iter << " Attached: ";

  list<GraphIterator<int> > next_nodes = iter.next_nodes ();
  
    for (list<GraphIterator<int> >::iterator i = next_nodes.begin();
	 i != next_nodes.end();
	 i++)
      std::cout << **i << " ";
    std::cout << std::endl;

    for (list<GraphIterator<int> >::iterator i = next_nodes.begin();
	 i != next_nodes.end();
	 i++)
      display_subgraph(*i);
}

#endif /* DO_GRAPH_TEST */

/* EOF */
