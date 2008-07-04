//  Pingus - A free Lemmings clone
//  Copyright (C) 2000,2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_WORLDMAP_MANAGER_HPP
#define HEADER_PINGUS_WORLDMAP_MANAGER_HPP

#include <string>
#include "../sprite.hpp"
#include "../gui/gui_manager.hpp"
#include "../screen/gui_screen.hpp"

class SceneContext;

namespace WorldMapNS {

typedef int NodeId;
class WorldMap;
class WorldMapComponent;

/** The WorldMapScreen manages the worldmaps and the translation
    between two worldmaps, it also holds the GUI elements that are
    accessible in the WorldMap Screen */
class WorldMapScreen : public GUIScreen
{
private:
  Sprite levelname_bg;

  bool is_init;
  bool exit_worldmap;

  WorldMap* worldmap;
  WorldMap* new_worldmap;

public:
  WorldMapScreen ();
  ~WorldMapScreen ();

  void load (const std::string& filename);

  /** Check if WorldMap manager still needs to run and exit if if
      not */
  void update (float);
  void draw_foreground(DrawingContext& gc);

  /** @defgroup WorldMapScreenBindings Controller bindings of the WorldMapScreen
      @{*/
  /** Calculate the node that was clicked and set the pingu to walk
      to that node. If a node is double-cliked, the pingu should go
      faster. */
  void on_primary_button_press (int x, int y);

  /** Exit the WorldMapScreen and return to the previous screen */
  void on_escape_press ();
  /** @}*/

  WorldMap* get_worldmap() { return worldmap; }

  /** Change the current map to the given map

      @param filename the filename of the new map, filename must be
      @param filename relative to the worldmap directory
      @param filename Example: "volcano.pingus" */
  void change_map (const std::string& filename, NodeId node);

  Rect get_trans_rect() const;

private:
  /** Startup Hook of the Screen */
  void on_startup ();

  WorldMapScreen (const WorldMapScreen&);
  WorldMapScreen& operator= (const WorldMapScreen&);
};

} // namespace WorldMapNS

#endif

/* EOF */
