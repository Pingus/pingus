//  $Id: manager.hxx,v 1.7 2002/08/23 15:49:57 torangan Exp $
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
#include "../boost/smart_ptr.hpp"
#include "../gui_screen.hxx"

class CL_InputDevice;
class CL_Key;
class GameDelta;

namespace Pingus
{
  namespace WorldMap
  {
    class WorldMap;
  }

  /**  */
  class WorldMapManager : public GUIScreen
  {
    class WorldMapComponent : public GUI::Component
    {
    public:
      WorldMapComponent () { }
      
      void on_primary_button_press (int x, int y);
 
      void draw ();
      void update (float delta);
      
      bool is_at (int, int) { return true; }
      
    private:
      WorldMapComponent (const WorldMapComponent&);
      WorldMapComponent operator= (const WorldMapComponent&);

    } worldmap_component;

    
    friend class WorldMapComponent;
  private:
    static WorldMapManager* instance_;

    bool is_init;
    bool exit_worldmap;
    boost::shared_ptr<WorldMap::WorldMap> worldmap;
    boost::shared_ptr<WorldMap::WorldMap> new_worldmap;

    WorldMapManager ();
  public:
    ~WorldMapManager ();
  
    void update (float);
    void on_escape_press ();
    void on_startup ();
  private:
    /// Load all required resources if not already done
    void init ();

  public:
    void change_map (const std::string& filename, int node);
    static WorldMapManager* instance ();
    
  private:
    WorldMapManager (const WorldMapManager&);
    WorldMapManager operator= (const WorldMapManager&);
  };

}

#endif

/* EOF */



