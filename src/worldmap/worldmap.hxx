//  $Id: worldmap.hxx,v 1.14 2002/10/01 19:53:45 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDMAP_WORLDMAP_HXX
#define HEADER_PINGUS_WORLDMAP_WORLDMAP_HXX

#include "stat.hxx"

class GraphicContext;
class CL_Key;
class CL_Font;
class CL_InputDevice;

namespace WorldMapNS {

class Pingus;

/** A class for loading, displaying and managing the worldmap. The
    worldmap is basically a multi-layered image and a path
    (bidirectional graph) where a pingu can walk on. Parts of the
    worldmap are hidable and will only get displayed at specific
    events (successfull level completions, etc.). */
class WorldMap
{
#if TODO
public:
  /** Load the given*/
  WorldMap (const std::string& filename);
  
  void draw (GraphicContext& gc);
  void update (float delta)
  
  /** @return the shortest path between node1 and node2  */
  std::vector<Edge*> find_path (Node* node1, Node* node2);
  
  /** @return the node at the given position. x and y are in
      world-COs, not screen. */
  Node* get_node (int x, int y);
private:
  /** Parses a WorldMap XML file */
  void parse_file(xmlNodePtr cur);
  
  /** Parse the object section of the Worldmap XML file, it contains 
      Sprites, Backgrounds and other things */
  void parse_objects(xmlNodePtr cur);

  /** Parse the graph section of the WorldMap XML file, it contains
      the path where the Pingu can walk on. */
  void parse_graph(xmlNodePtr cur);
  
  /** Parse the propertie section of a WorldMap XML file, it contains
      meta data such as the author or the name of the Worldmap */
  void parse_properties(xmlNodePtr cur);

#endif
private:
  CL_Surface background;

  Sprite green_dot;
  Sprite red_dot;
  Sprite dot_border;
  Sprite green_flag; 

  //Graph<PingusWorldMapNode>* graph;
  Graph graph_data;
  typedef Graph::iterator GraphIter;

  /** FIXME: Should this be part of the worldmap manager? -> probally
      FIXME: not, since pingu needs to be z-ordered */
  Pingus* pingu;

  /** FIXME: What is this? */
  boost::shared_ptr<Node> last_node;
  typedef boost::shared_ptr<Node> NodePtr;
  unsigned int last_node_time;
  
  /** This should be the state saving object, which tells which
      nodes are accessible and which are not. FIXME: might needs a
      reimplementation. */
  boost::shared_ptr<PingusWorldMapStat> stat;
public:
  /** Load a worldmap from a given worldmap description file
      @param filename The fully qualified filename (as fopen()
      @param and friends filename likes it) */
  WorldMap (std::string filename);

  /** Destruct the worldmap */
  ~WorldMap ();

  /** Start up the music and other things that need only to me run
      once on startup of a new WorldMap */
  void on_startup ();

  /** Save the current status to a file */
  void save ();
  
  /** React on button press:
      - calculate which level was clicked
      - calculate the shortest path
      - let the pingu walk */
  void on_primary_button_press (int x, int y);

  /** Disable all event catching */
  void disable_button_events ();
  
  /** Enable all event catching */
  void enable_button_events ();
  
  /** Draw the world worldmap */
  void draw (GraphicContext& gc);
  
  /** Let the woldmap do some stuff, like animating smoke, playing
      sounds or reacting on special events */
  void update (float delta);

  /** Returns a pointer to the node under the given coordinates */
  NodePtr get_node (int x, int y);
  
  /** Callculate the offset which is used for drawing and collision
      detection. The offset will be used for scrolling when the
      background is larger than the screen. 

      FIXME: Do we need this one public? Other classes could
      FIXME: handle the draw offset via the GraphicContext, 
      FIXME: collision handling should be done in world-co's

      @return the currently used draw offset */
  Vector get_offset ();

  /** Set the pingu to the given node with 'id' */
  void set_pingus (int node_id);
      
private:
  WorldMap (const WorldMap&);
  WorldMap& operator= (const WorldMap&);
};

} // namespace WorldMapNS

#endif

/* EOF */
