//  $Id: path_graph.cxx,v 1.1 2002/10/12 23:34:43 grumbel Exp $
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

#include "../xml_helper.hxx"
#include "../pingus_error.hxx"
#include "dot_factory.hxx"
#include "path_graph.hxx"

namespace WorldMapNS {

PathGraph::PathGraph(WorldMap* worldmap, xmlDocPtr doc, xmlNodePtr cur)
{
  // cur is at <graph>...
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);
  
  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, "nodes"))
        {

        }
      else if (XMLhelper::equal_str(cur->name, "edges"))
        {
          
        }
    }
}

void
PathGraph::parse_nodes(xmlDocPtr doc, xmlNodePtr cur)
{
  while (cur)
    {
      DotFactory::create(doc, cur);
    }  
}

void
PathGraph::parse_edges(xmlDocPtr doc, xmlNodePtr cur)
{
  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, "edge"))
        {
          std::string name;
          std::string source;
          std::string destination;

          if (!XMLhelper::get_prop(cur, "id", name))
            {
              PingusError::raise("PathGraph: Missing Edge Name");
            }

          if (!XMLhelper::get_prop(cur, "source", source))
            {
              PingusError::raise("PathGraph: Missing Source Name");
            }

          if (!XMLhelper::get_prop(cur, "destination", destination))
            {
              PingusError::raise("PathGraph: Missing Destination Name");
            }

          // FIXME: add path-data parsing here

          // FIXME: No error checking
          graph.add_bi_edge((Path*)0, node_lookup[source], node_lookup[destination], 
                            0 /* costs */);
        }
      else
        {
          PingusError::raise("PathGraph: unhandled: ");
        }

      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }   
}

std::vector<Vector>
PathGraph::get_path(NodeId start, NodeId end)
{
  // insert pathfinding magic here...
  return std::vector<Vector>();
}

} // namespace WorldMapNS

/* EOF */
