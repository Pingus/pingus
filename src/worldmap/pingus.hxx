//  $Id: pingus.hxx,v 1.12 2002/09/27 11:26:49 torangan Exp $
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

#include "../pingus.hxx"
#include <math.h>
#include <queue>
#include "node.hxx"

namespace boost {
  template <class T> class shared_ptr;
}

namespace WorldMapNS {

/** This is the representation of the horde of Pingus which will walk
    on the worldmap */
class Pingus
{
private:
#if TODO
  /** The node on which the pingu currently stands, 0 if the pingu is
      currently on the move to another node */
  Node* current_node;
  
  /** The node from which the pingu as started its walk to the
      target_node, value is undefined when the pingu is currently
      standing on a node */
  Node* source_node;
 
  /** The node to which the pingu is currently walking, value is
      undefined when the pingu is currently standing on a node */
  Node* target_node;

  /** The node path to walk. The edge between two nodes is itself
      represented as a array of positions */
  std::vector<Node*> node_path;

  /** The path which represents an edge between two nodes */
  std::vector<CL_Vector> edge_path;
  
  /** The length of the edge_path in pixels */
  float edge_path_length;

  /** The position in the edge_path, 0 means the pingu is on the
      source_node, edge_path_length means that the pingu has reached
      the target node. Position between edge_path nodes is
      interpolated. */
  float edge_path_position;

  /** Current position of the pingu, only for caching purpose */
  CL_Vector pos;

  void draw (GraphicContext& gc);

  void update_walk (float delta)
  {
    // Update the position
    edge_path_position += velocity * delta;

    if (edge_path_position > edge_path_length) // target reached
      {
	if (node_path.empty ()) // final target reached
	  {
	    current_node = target_node;
	  }
	else // edge is traveled, now go to the next node
	  {
	    source_node = target_node;
	    target_node = node_path.top ();
	    node_path.pop ();

	    edge_path_position = 0.0f;
	    edge_path = get_edge (source_node, target_node)->data;
	    edge_path_lenght   = calc_edge_path_length ();
	  }
      }

    // Recalc pingu position on the screen
    pos = calc_pos ();
  }
  
  /** @return true if the node is reachable, false otherwise */
  bool walk_to_node (Node* target)
  {
    if (current_node) // pingu stands still
      {
	node_path = worldmap->find_path (current_node, target);
      }
    else // pingu between two nodes
      {
	node_path1 = worldmap->find_path (source_node, target);
	node_path2 = worldmap->find_path (target_node, target);
	
	// Select the shorter path
	if (length (node_path1) < length (node_path2))
	  { // walk to source node, which means to reverse the pingu
	    node_path = node_path1;

	    // Reverse the pingu
	    swap(target_node, source_node);
	    std::reverse(edge_path.begin (), edge_path.end ());
	    edge_path_position = edge_path_lenght - edge_path_position;
	  }
	else
	  { // walk to target_node
	    node_path = node_path2;
	  }	
      }
  }
  
  /** calculate the position of the pingu */
  CL_Vector calc_pos ()
  {
    if (current_node) // pingu stands still
      {
	return current_node->get_pos ();
      }
    else // between two nodes
      {
	iterator current = edge_path.begin ();
	iterator next    = edge_path.begin () + 1;

	float comp_length = 0.0f;
	while (next != end)
	  {
	    float length = line_length (current, next);

	    if (comp_length + length > edge_path_position) 
	      {
		float perc = (edge_path_position - comp_length) // length to walk from current node
		  / length;

		return interpol (current, next, perc);
	      }

	    ++current;
	    ++next;
	  }
	assert (!"This shouldn't happen, traveled bejoint target node");
	return target_node->get_pos ();
      }
  }

  void update (float delta)
  {
    sprite.update (delta);

    if (current_node)
      {
	// do stuff when pingu is on a node
      }
    else // pingu walking
      {
	walk (delta);
      }
  }
  
  /** @return the node on which the pingu is currently standing, 0 is
      returned if the pingu is currently between two nodes */
  Node* get_node () {
    return current_node;
  }
#endif 

  Sprite sprite;
  CL_Vector pos;
  std::queue<Node*> targets;
  bool is_left;
  Node* current_node;

public:
  Pingus ();
  ~Pingus ();

  void draw (const CL_Vector& offset);
  void update (float delta);

  /** Let the pingu walk to the given node
      @param node The node to walk to */
  void walk_to (Node* node);

  /** @return If the pingu is currently between two nodes return
      true */
  bool is_walking ();

  /** Set the pingu to the position of a node */
  void set_position (boost::shared_ptr<Node> node);

  /** @return Current position of the pingu on the screen */
  CL_Vector get_pos () { return pos; }

  /** @return The node where the pingu is current on 

  FIXME: What should happen if the pingu is between two nodes? */
  Node* get_node ();
  
private:
  Pingus (const Pingus&);
  Pingus& operator= (const Pingus&);
};

} // namespace WorldMapNS

#endif

/* EOF */
