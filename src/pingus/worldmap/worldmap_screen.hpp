// Pingus - A free Lemmings clone
// Copyright (C) 2000,2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_WORLDMAP_SCREEN_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_WORLDMAP_SCREEN_HPP

#include <memory>
#include <string>

#include "engine/display/sprite.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/screen/gui_screen.hpp"

namespace GUI {
class SurfaceButton;
}

class SceneContext;
namespace WorldmapNS {

typedef int NodeId;
class Worldmap;
class WorldmapComponent;

/** The WorldmapScreen manages the worldmaps and the translation
    between two worldmaps, it also holds the GUI elements that are
    accessible in the Worldmap Screen */
class WorldmapScreen : public GUIScreen
{
private:
  bool is_init;
  bool exit_worldmap;

  std::unique_ptr<Worldmap> worldmap;
  std::unique_ptr<Worldmap> new_worldmap;

  GUI::SurfaceButton* close_button;
  GUI::SurfaceButton* credits_button;
  WorldmapComponent* m_worldmap_component;

public:
  WorldmapScreen ();
  ~WorldmapScreen ();

  void load(const Pathname& filename);

  /** Check if Worldmap manager still needs to run and exit if if
      not */
  void update (float);
  void draw_foreground(DrawingContext& gc);

  /** @defgroup WorldmapScreenBindings Controller bindings of the WorldmapScreen
      @{*/
  /** Calculate the node that was clicked and set the pingu to walk
      to that node. If a node is double-cliked, the pingu should go
      faster. */
  void on_primary_button_press (int x, int y);

  void on_fast_forward_press();
  void on_fast_forward_release();

  /** Exit the WorldmapScreen and return to the previous screen */
  void on_escape_press ();
  /** @}*/

  Worldmap* get_worldmap() { return worldmap.get(); }

  Rect get_trans_rect() const;

  void show_intro_story();
  void show_end_story();

  void resize(const Size& size);
private:
  /** Startup Hook of the Screen */
  void on_startup ();

  WorldmapScreen (const WorldmapScreen&);
  WorldmapScreen& operator= (const WorldmapScreen&);
};

} // namespace WorldmapNS

#endif

/* EOF */
