// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_PINGUS_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_PINGUS_HPP

#include "pingus/state_sprite.hpp"
#include "pingus/worldmap/drawable.hpp"
#include "pingus/worldmap/path_graph.hpp"

namespace WorldmapNS {

/** This is the representation of the horde of Pingus which will walk
    on the worldmap */
class Pingus : public Drawable
{
private:
  PathGraph* path;
  StateSprite sprite;
  Sprite sprite_standing;
  Sprite arrow;

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

  /** While the target_node holds the next node to which the pingu is
      walking, the final_target_node holds the node on which the pingu
      will stop */
  NodeId final_target_node;

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
  Vector3f pos;

  /** The position the pingu had before his current position, used to
      calculate its current direction */
  Vector3f last_pos;

public:
  Pingus (PathGraph* arg_path);
  ~Pingus ();

  void draw (DrawingContext& gc);
  void update (float delta);

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
  Vector3f get_pos() const { return pos; }

private:
  /** Calculate the direction in which the pingu is heading, return
      value is in degrees => [0,360[, 0=north, 180=south, 270=east, 90=west */
  float get_direction() const;

  /** calculate the position of the pingu */
  Vector3f calc_pos ();

  void  update_walk (float delta);

  void update_edge_path();

  float calc_edge_path_length();

  Vector3f interpolate(const Vector3f& a, const Vector3f& b, float perc);

  Pingus (const Pingus&);
  Pingus& operator= (const Pingus&);
};

} // namespace WorldmapNS

#endif

/* EOF */
