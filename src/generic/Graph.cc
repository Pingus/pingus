//  $Id: Graph.cc,v 1.3 2001/04/30 17:52:00 grumbel Exp $
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

#include <iostream>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>
#include "Graph.hh"

void
print_connections (GraphNode<CL_Vector>& obj)
{
  std::cout << "Data: (" << obj.data << ")";
  for (std::list<GraphNode<CL_Vector>* >::iterator i = obj.next_nodes.begin ();
       i != obj.next_nodes.end(); ++i)
    std::cout << "(" << (*i)->data << ") ";
  std::cout << std::endl;
}

void
draw_graph (Graph<CL_Vector>& graph)
{
  for (std::list<GraphNode<CL_Vector> >::iterator i = graph.nodes.begin ();
       i != graph.nodes.end(); ++i)
    {
      std::cout << "Drawing:..." << std::endl;
      for (std::list<GraphNode<CL_Vector>* >::iterator j = (*i).next_nodes.begin ();      
	   j != (*i).next_nodes.end(); ++j)
	{
	  std::cout << "Drawing: " << i->data << std::endl;
	  CL_Display::draw_line (int(i->data.x), int(i->data.y),
				 int((*j)->data.x), int((*j)->data.y),
				 1.0, 1.0, 1.0);
	}
    }
}

class GenericMain : public CL_ClanApplication
{
  char * get_title()
  {
    return "Graph Test";
  }

  int main (int argc, char* argv[])
{
  CL_SetupCore::init ();
  CL_SetupDisplay::init();

  CL_Display::set_videomode(640, 480, 16, 
			    false, 
			    false); // allow resize

  Graph<CL_Vector> graph;

  GraphNode<CL_Vector> a (CL_Vector (10, 10));
  GraphNode<CL_Vector> b (CL_Vector (630, 10));
  GraphNode<CL_Vector> c (CL_Vector (10, 400));
  GraphNode<CL_Vector> d (CL_Vector (630, 400));
  GraphNode<CL_Vector> e (CL_Vector (320, 150));

  a.biconnect (&b);
  b.biconnect (&c);
  c.biconnect (&d);
  e.biconnect (&a);

  a.biconnect (&d);

  graph.add (a);
  graph.add (b);
  graph.add (c);
  graph.add (d);
  graph.add (e);


  print_connections (a);
  print_connections (b);
  print_connections (c);
  print_connections (d);
  print_connections (e);

  CL_System::keep_alive ();
  while (!CL_Keyboard::get_keycode (CL_KEY_ESCAPE))
    {
      CL_Display::clear_display ();
      draw_graph (graph);
      CL_Display::flip_display ();
      CL_System::sleep (20);
      CL_System::keep_alive ();
    }

  return 0;
}
} app;

/* EOF */
