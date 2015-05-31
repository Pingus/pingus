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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_WORLDMAP_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_WORLDMAP_HPP

#include <memory>
#include <vector>

#include "engine/display/graphic_context_state.hpp"
#include "pingus/worldmap/pingus_worldmap.hpp"

class Font;
class DrawingContext;

namespace WorldmapNS {

typedef int EdgeId;
typedef int NodeId;

class WorldmapStory;
class PathGraph;
class Drawable;
class Pingus;

/** A class for loading, displaying and managing the worldmap. The
    worldmap is basically a multi-layered image and a path
    (bidirectional graph) where a pingu can walk on. Parts of the
    worldmap are hidable and will only get displayed at specific
    events (successfull level completions, etc.). */
class Worldmap
{
private:
  PingusWorldmap worldmap;

  NodeId default_node;
  NodeId final_node;

  Pingus* pingus;

  GraphicContextState gc_state;

  /** The graph that represents the path on the map */
  std::unique_ptr<PathGraph> path_graph;

  /** A collection of drawable things */
  std::vector<std::unique_ptr<Drawable> > drawables;

  int mouse_x;
  int mouse_y;

public:
  /** Load the given*/
  Worldmap(const Pathname& filename);
  ~Worldmap();

  Pingus* get_pingus() { return pingus; }

  void on_startup();

  bool is_final_map();

  void draw (DrawingContext& gc);
  void update (float delta);

  /** Enters the level on which the Pingu is currently standing */
  void enter_level();

  void add_drawable(std::unique_ptr<Drawable> drawable);

  /** @return the shortest path between node1 and node2  */
  std::vector<EdgeId> find_path (NodeId node1, NodeId node2);

  /** x,y are in Worldmap CO, not ScreenCO */
  void on_primary_button_press(int x, int y);
  void on_secondary_button_press(int x, int y);
  void on_pointer_move(int x, int y);

  int get_width()  const;
  int get_height() const;

  PingusWorldmap get_worldmap() const { return worldmap; }
private:
  /** Unlock nodes according to the finished ones */
  void update_locked_nodes();

  /** Sets the starting level on the worldmap.  Either take it from the StatManager
      or use the "default-node" option from the XML file */
  void set_starting_node();

private:
  static Worldmap* current_;
public:
  static Worldmap* current() { return current_; }

private:
  Worldmap(const Worldmap&);
  Worldmap & operator=(const Worldmap&);
};

} // namespace WorldmapNS

#endif

/* EOF */
