//  $Id: manager.hxx,v 1.2 2002/06/20 11:23:53 grumbel Exp $
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

#ifndef PINGUSWORLDMAPMANAGER_HH
#define PINGUSWORLDMAPMANAGER_HH

#include <string>
#include <ClanLib/Signals/slot.h>
#include "../boost/smart_ptr.hpp"

class CL_InputDevice;
class CL_Key;

namespace Pingus
{
  namespace WorldMap
  {
    class WorldMap;
  }

  /**  */
  class WorldMapManager
  {
  private:
    bool is_init;
    bool exit_worldmap;
    boost::shared_ptr<WorldMap::WorldMap> worldmap;
    boost::shared_ptr<WorldMap::WorldMap> new_worldmap;

    /// Some slots to manage the event handling
    //@{
    CL_Slot on_button_press_slot;
    CL_Slot on_button_release_slot;
    CL_Slot on_mouse_move_slot;
    CL_Slot on_resize_slot;
    //@}

    static WorldMapManager* current_manager;

  public:
    WorldMapManager ();
    ~WorldMapManager ();
  
  private:
    /// Load all required resources if not already done
    void init ();

    void on_button_press (CL_InputDevice *device, const CL_Key &key);
    void on_button_release (CL_InputDevice *device, const CL_Key &key);
    void on_mouse_move(CL_InputDevice *,int mouse_x, int mouse_y);
    void on_resize(int w, int h);

  public:
    /// Display the worldmap and let the user select a level
    void display ();
    void change_map (const std::string& filename, int node);

    static WorldMapManager* get_current () { return current_manager; } 
  };

}

#endif

/* EOF */



