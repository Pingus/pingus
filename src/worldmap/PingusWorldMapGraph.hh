//  $Id: PingusWorldMapGraph.hh,v 1.13 2001/07/24 09:10:12 grumbel Exp $
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
#include "../XMLhelper.hh"
#include "../ResDescriptor.hh"
#include "../Position.hh"
//#include "../generic/Graph.hh"

/** An object on the worldmap */
class PingusWorldMapNode
{
public:
  int id;
  bool accessible;
  CL_Vector pos;
  std::list<int> links;
  
  PingusWorldMapNode () {}
  virtual ~PingusWorldMapNode () {}

  virtual void on_click () =0;
  virtual void mark (bool value) {}
  virtual void draw (CL_Vector offset) {}
  virtual std::map<std::string, std::string> get_string () =0;
};

/** A wrap object which brings you to the next worldmap */
class PingusWorldMapTubeNode
  : public PingusWorldMapNode
{
public:
  std::string worldmap_name;
  Sprite tube;
  int link_node;
public:  
  PingusWorldMapTubeNode ();
  void on_click ();
  void draw (CL_Vector offset);
  std::map<std::string, std::string> get_string ();
};

/** The entrance to a level */
class PingusWorldMapLevelNode
  : public PingusWorldMapNode
{
private:
  Sprite green_dot;
  Sprite red_dot;
  Sprite dot_border;
  Sprite green_flag;

  boost::shared_ptr<PLF> plf;

public:
  std::string levelname;
  bool finished;
  boost::shared_ptr<PLF> get_plf ();

  PingusWorldMapLevelNode () 
    : green_dot ("worldmap/dot_green", "core"),
      red_dot ("worldmap/dot_red", "core"),
      dot_border ("Game/dot_border", "game"),
      green_flag ("worldmap/flaggreen", "core")
  {
    accessible = false;
    finished = false;

    green_flag.set_align (-24, -36);
    green_dot.set_align_center ();
    red_dot.set_align_center ();
    dot_border.set_align_center ();
  }

  void on_click ();
  void mark (bool value);
  void draw (CL_Vector offset);
  std::map<std::string, std::string> get_string ();
};

class PingusWorldMapGraph
{
private:
  //Graph<PingusWorldMapNode>* graph;
  ResDescriptor bg_desc;
  std::string music;
  xmlDocPtr doc;
  
public:
  std::list<boost::shared_ptr<PingusWorldMapNode> >   nodes;
  typedef std::list<boost::shared_ptr<PingusWorldMapNode> >::iterator iterator;

  PingusWorldMapGraph ();
  ~PingusWorldMapGraph ();
 
  ResDescriptor              get_background ();
  //Graph<PingusWorldMapNode>* get_graph ();
  std::string get_music ();
  
  void draw (const CL_Vector&);

  /// Some functions to parse the data out of an xml file
  //@{ 
  void parse_file (std::string filename);
private:
  void parse_node_list (xmlNodePtr);
  void parse_node (xmlNodePtr);
  void parse_tube (xmlNodePtr);
  void parse_music (xmlNodePtr);
  void parse_background (xmlNodePtr);
  //@}
};

#endif

/* EOF */
