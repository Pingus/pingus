//  $Id: manager.cxx,v 1.26 2003/03/25 00:37:44 grumbel Exp $
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

#include <iostream>
#include "../gui/screen_manager.hxx"
#include "../gui/surface_button.hxx"
#include "../path_manager.hxx"
#include "../res_descriptor.hxx"
#include "worldmap.hxx"
#include "manager.hxx"

namespace WorldMapNS {

WorldMapManager* WorldMapManager::instance_ = 0;

class WorldMapManagerCloseButton 
  : public GUI::SurfaceButton
{
public:
  WorldMapManagerCloseButton();
  void on_click();
};

class WorldMapManagerEnterButton
  : public GUI::SurfaceButton
{
public:
  WorldMapManagerEnterButton();
  void on_click();
};

WorldMapManagerCloseButton::WorldMapManagerCloseButton()
  : GUI::SurfaceButton(5, 5,
                       ResDescriptor("menu/close_normal", "core"),
                       ResDescriptor("menu/close_pressed", "core"),
                       ResDescriptor("menu/close_highlight", "core"))
{
}

void
WorldMapManagerCloseButton::on_click()
{
  ScreenManager::instance ()->pop_screen ();
}

WorldMapManagerEnterButton::WorldMapManagerEnterButton()
  : GUI::SurfaceButton(500, 10,
                       ResDescriptor("menu/enterlevel", "core"),
                       ResDescriptor("menu/enterlevel", "core"),
                       ResDescriptor("menu/enterlevel", "core"))
{
}

void
WorldMapManagerEnterButton::on_click()
{
  WorldMapManager::instance()->get_worldmap()->enter_level();
   }

WorldMapManager::WorldMapManager ()
  : worldmap(0),
    new_worldmap(0)
{
  is_init = false;
  exit_worldmap = false;
 
  // FIXME: The default startup map should be configurable by some file
  worldmap = new WorldMap (path_manager.complete("worldmaps/tutorial.xml"));

  //worldmap->on_startup ();

  // FIXME: a bit ugly because of the proteced member, but should work
  // FIXME: well enough. GUIScreen could also use multi-inheritage,
  // FIXME: but that could lead to member function name conflicts
  worldmap_component = new WorldMapComponent();
  gui_manager->add (worldmap_component);
  gui_manager->add(new WorldMapManagerCloseButton());
  gui_manager->add(new WorldMapManagerEnterButton());
}

void
WorldMapManager::on_startup ()
{
  exit_worldmap = false;
}

WorldMapManager::~WorldMapManager ()
{
  delete worldmap;
  delete new_worldmap;
}

void
WorldMapManager::on_escape_press ()
{
  //std::cout << "WorldMapManager::on_escape_press ()..." << std::endl;
  exit_worldmap = true;
}

void
WorldMapManager::update (float)
{
  // Exit the word
  if (exit_worldmap)
    ScreenManager::instance ()->pop_screen ();

  // Check if new worldmap is set and if so, change it
  if (new_worldmap)
    {
      worldmap     = new_worldmap;
      new_worldmap = 0;
    }
}

void
WorldMapManager::WorldMapComponent::draw (GraphicContext& gc)
{
  WorldMapManager::instance()->worldmap->draw(gc);
}

void
WorldMapManager::WorldMapComponent::update (float delta)
{
  WorldMapManager::instance()->worldmap->update();
  UNUSED_ARG(delta);
}

void 
WorldMapManager::WorldMapComponent::on_primary_button_press (int x, int y)
{
  //std::cout << "Buton press" << std::endl;
  /** Fixme: insert Co. translation here */
  WorldMapManager::instance ()->worldmap->on_primary_button_press (x, y);
}


void 
WorldMapManager::WorldMapComponent::on_pointer_move (int x, int y)
{
  WorldMapManager::instance ()->worldmap->on_pointer_move (x, y);
}

void 
WorldMapManager::WorldMapComponent::on_secondary_button_press (int x, int y)
{
  //std::cout << "Buton press" << std::endl;
  /** Fixme: insert Co. translation here */
  WorldMapManager::instance ()->worldmap->on_secondary_button_press (x, y);
}

void 
WorldMapManager::change_map (const std::string& filename, NodeId node)
{
  // Create the new worldmap and make it the current one
  new_worldmap = new WorldMap (path_manager.complete("worldmaps/" + filename));
  new_worldmap->set_pingus (node);
}

WorldMapManager*
WorldMapManager::instance ()
{
  if (instance_)
    return instance_;
  else
    return instance_ = new WorldMapManager ();
}

} // namespace WorldMapNS

/* EOF */
