//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_WORLDMAP_WORLDMAP_HPP
#define HEADER_PINGUS_WORLDMAP_WORLDMAP_HPP

#include <vector>
#include "../sprite.hpp"
#include "../file_reader.hpp"
#include "../display/drawing_context.hpp"
#include "../graphic_context_state.hpp"

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
  /** name of the file to parse */
  std::string filename;

  WorldmapStory *intro_story;
  WorldmapStory *end_story;

  typedef std::vector<Drawable*>   ObjectLst;
  typedef std::vector<Drawable*> DrawableLst;

  int width;
  int height;

  std::string name;
  std::string short_name;
  std::string author;
  std::string email;
  std::string music;

  // Beginning and ending nodes are configurable by the XML file.
  NodeId default_node;
  NodeId final_node;

  Pingus* pingus;

  GraphicContextState gc_state;

  /** The graph that represents the path on the map */
  PathGraph* path_graph;

  /** A collection of drawable things, allocation and deallocation
      takes place elsewhere these are only references to other
      objects */
  DrawableLst drawables;

  /** A collection of drawables loaded from the xml file, this list
      gets deleted at the end */
  ObjectLst objects;

  int mouse_x;
  int mouse_y;

private:
  static Worldmap* current_; 
public:
  static Worldmap* current() { return current_; }

  /** Load the given*/
  Worldmap(const std::string& filename);
  ~Worldmap();

  Pingus* get_pingus() { return pingus; }
  WorldmapStory* get_intro_story() const { return intro_story; }
  WorldmapStory* get_end_story() const { return end_story; }

  void on_startup();

  std::string get_filename() const { return filename; }
  std::string get_shortname() const { return short_name; }

  bool is_final_map();

  void draw (DrawingContext& gc);
  void update (float delta);

  /** Enters the level on which the Pingu is currently standing */
  void enter_level();

  /** Return the current levelname for use in GUI */
  std::string get_levelname();

  /** The the pingu to the given Node */
  void set_pingus(NodeId id);

  /** FIXME: Memory leak?! */
  void add_drawable(Drawable* drawable);
  void remove_drawable(Drawable* drawable);

  /** @return the shortest path between node1 and node2  */
  std::vector<EdgeId> find_path (NodeId node1, NodeId node2);

  /** x,y are in Worldmap CO, not ScreenCO */
  void on_primary_button_press(int x, int y);
  void on_secondary_button_press(int x, int y);
  void on_pointer_move(int x, int y);

  int get_width()  const { return width; }
  int get_height() const { return height; }

private:
  /** Parses a Worldmap XML file */
  void parse_file(FileReader reader);

  /** Parse the object section of the Worldmap XML file, it contains
      Sprites, Backgrounds and other things */
  void parse_objects(FileReader reader);

  /** Parse the graph section of the Worldmap XML file, it contains
      the path where the Pingu can walk on. */
  void parse_graph(FileReader reader);

  /** Parse the propertie section of a Worldmap XML file, it contains
      meta data such as the author or the name of the Worldmap */
  void parse_properties(FileReader reader);

  /** Unlock nodes according to the finished ones */
  void update_locked_nodes();

  /** Sets the starting level on the worldmap.  Either take it from the StatManager
      or use the "default-node" option from the XML file */
  void set_starting_node();
};

} // namespace WorldmapNS

#endif

/* EOF */
