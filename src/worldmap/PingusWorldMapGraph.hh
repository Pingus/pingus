//  $Id: PingusWorldMapGraph.hh,v 1.20 2002/06/06 09:34:28 grumbel Exp $
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

#ifndef PINGUSWORLDMAPGRAPH_HH
#define PINGUSWORLDMAPGRAPH_HH

#include <string>

#include "../PLF.hh"
#include "../Sprite.hh"
#include "../ResDescriptor.hh"
#include "../Position.hh"
#include "../XMLhelper.hh"
#include "PingusWorldMapNode.hh"

class PingusWorldMapGraph
{
private:
  //Graph<Node>* graph;
  ResDescriptor bg_desc;
  std::string music;
  xmlDocPtr doc;
  
public:
  std::list<boost::shared_ptr<Pingus::WorldMap::Node> >   nodes;
  typedef std::list<boost::shared_ptr<Pingus::WorldMap::Node> >::iterator iterator;

  PingusWorldMapGraph ();
  ~PingusWorldMapGraph ();
 
  ResDescriptor              get_background ();
  //Graph<Node>* get_graph ();
  std::string get_music ();
  
  void draw (const CL_Vector&);

  /// Some functions to parse the data out of an xml file
  //@{ 
  void parse_file (std::string filename);
private:
  void parse_node_list (xmlNodePtr);
  void parse_music (xmlNodePtr);
  void parse_background (xmlNodePtr);
  //@}
};

#endif

/* EOF */
