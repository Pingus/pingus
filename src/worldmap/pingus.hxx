//  $Id: pingus.hxx,v 1.20 2002/11/26 21:30:37 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDMAP_PINGUS_HXX
#define HEADER_PINGUS_WORLDMAP_PINGUS_HXX

#include <math.h>
#include <queue>

#include "../sprite.hxx"
#include "../vector.hxx"
#include "../pingus.hxx"
#include "drawable.hxx"
#include "path_graph.hxx"

namespace boost {
  template <class T> class shared_ptr;
}

namespace WorldMapNS {

/** This is the representation of the horde of Pingus which will walk
    on the worldmap */
class Pingus : public Drawable
{
private:
  PathGraph* path;
  Sprite sprite;

  /** The node on which the pingu currently stands, NoNode if the pingu is
      currently on the move to another node */
  NodeId current_node;
  
  /** The node from which the pingu as started its walk to the
      target_node, value is undefined when the pingu is currently
      standing on a node */
  NodeId source_node;
 
  /** The node to which the pingu is currently walking, value is
      undefined when the pingu is currently standing on a node. This
      is not the final target node (aka node_path.back()), but instead
      the end node of an edge */
  NodeId target_node;

  /** The node path to walk. The edge between two nodes is itself
      represented as a array of positions */
  std::vector<NodeId> node_path;

  /** The path which represents an edge between two nodes, it includes
      both source and destinations position, so it is complete */
  Path edge_path;
  
  /** The position in the edge_path, 0 means the pingu is on the
      source_node, edge_path_length means that the pingu has reached
      the target node. Position between edge_path nodes is
      interpolated. */
  float edge_path_position;

  /** Current position of the pingu, only for caching purpose */
  Vector pos;

  /** The position the pingu had before his current position, used to
      calculate its current direction */
  Vector last_pos;

public:
  /** */
  Pingus (PathGraph* arg_path);
  ~Pingus ();

  void draw (GraphicContext& gc);
  void update ();

  /** @return true if the node is reachable, false otherwise */
  bool walk_to_node (NodeId target);
  
  bool is_walking();
  
  /** @return the node on which the pingu is currently standing, 0 is
      returned if the pingu is currently between two nodes */
  NodeId get_node () {
    return current_node;
  }

  /** Set the pingu to the position of a given node */
  void set_position (NodeId node); 

  /** return the current position in world coordinates */
  Vector get_pos() const { return pos; }

  /** Used for z-ordering for drawing */
  float get_z_pos() const;
  
private:
  /** Calculate the direction in which the pingu is heading, return
      value is in degrees => [0,360[, 0=north, 180=south, 270=east, 90=west */
  float get_direction() const;

  /** calculate the position of the pingu */
  Vector calc_pos ();

  void  update_walk (float delta);

  void update_edge_path();

  float calc_edge_path_length();
  
  /** */
  Vector interpolate(const Vector& a, const Vector& b, float perc);

  Pingus (const Pingus&);
  Pingus& operator= (const Pingus&);
};

} // namespace WorldMapNS

#endif

/* EOF */
