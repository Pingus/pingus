//  $Id: manager.hxx,v 1.24 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDMAP_MANAGER_HXX
#define HEADER_PINGUS_WORLDMAP_MANAGER_HXX

#include <string>
#include <ClanLib/Signals/slot.h>
#include "../gui/gui_manager.hxx"
#include "../gui/gui_screen.hxx"

class CL_InputDevice;
class CL_Key;
class GameDelta;

namespace Pingus {
namespace WorldMapNS {

typedef int NodeId;
class MetaMap;
class WorldMap;

/** The WorldMapManager manages the worldmaps and the translation
    between two worldmaps, it also holds the GUI elements that are
    accessible in the WorldMap Screen */
class WorldMapManager : public GUIScreen
{
  /** FIXME: Workaround class to let the worldmap play with well
      FIXME: with the Screen, should be deleted at a later point. */
  class WorldMapComponent : public GUI::Component
  {
  public:
    WorldMapComponent () { }

    void on_primary_button_press (int x, int y);
    void on_secondary_button_press (int x, int y);
    void on_pointer_move(int x, int y);

    void draw (DrawingContext& gc);
    void update (float delta);

    bool is_at (int, int) { return true; }

  private:
    WorldMapComponent (const WorldMapComponent&);
    WorldMapComponent& operator= (const WorldMapComponent&);

  };

  WorldMapComponent* worldmap_component;

  friend class WorldMapComponent;
private:
  static WorldMapManager* instance_;

  bool is_init;
  bool exit_worldmap;

  WorldMap* worldmap;
  WorldMap* new_worldmap;

	MetaMap* metamap;

  WorldMapManager ();
public:
  ~WorldMapManager ();

  void load (const std::string& filename);

  /** Check if WorldMap manager still needs to run and exit if if
      not */
  void update (float);

  /** @defgroup WorldMapManagerBindings Controller bindings of the WorldMapManager
      @{*/
  /** Calculate the node that was clicked and set the pingu to walk
      to that node. If a node is double-cliked, the pingu should go
      faster. */
  void on_primary_button_press (int x, int y);

  /** Exit the WorldMapManager and return to the previous screen */
  void on_escape_press ();
  /** @}*/

  WorldMap* get_worldmap() { return worldmap; }
	MetaMap* get_metamap() { return metamap; }

  /** Change the current map to the given map

  @param filename the filename of the new map, filename must be
  @param filename relative to the worldmap directory
  @param filename Example: "volcano.pingus" */
  void change_map (const std::string& filename, NodeId node);

  /** Singleton access function */
  static WorldMapManager* instance ();
  static void deinit();
private:
  /** Startup Hook of the Screen */
  void on_startup ();

  WorldMapManager (const WorldMapManager&);
  WorldMapManager& operator= (const WorldMapManager&);
};

} // namespace WorldMapNS
} // namespace Pingus

#endif

/* EOF */
