//  $Id: manager.cxx,v 1.15 2002/10/12 23:34:43 grumbel Exp $
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
#include "../screen_manager.hxx"
#include "../path_manager.hxx"
#include "worldmap.hxx"
#include "manager.hxx"

namespace WorldMapNS {

WorldMapManager* WorldMapManager::instance_ = 0;

WorldMapManager::WorldMapManager ()
{
  is_init = false;
  exit_worldmap = false;

  // FIXME: The default startup map should be configurable by some file
  worldmap = boost::shared_ptr<WorldMap>
    (new WorldMap (path_manager.complete("worldmaps/newvolcano.xml")));

  //worldmap->on_startup ();

  // FIXME: a bit ugly because of the proteced member, but should work
  // FIXME: well enough. GUIScreen could also use multi-inheritage,
  // FIXME: but that could lead to member function name conflicts
  gui_manager->add (&worldmap_component);
}

void
WorldMapManager::on_startup ()
{
  exit_worldmap = false;
}

WorldMapManager::~WorldMapManager ()
{
}

void
WorldMapManager::on_escape_press ()
{
  std::cout << "WorldMapManager::on_escape_press ()..." << std::endl;
  exit_worldmap = true;
}

void
WorldMapManager::update (float)
{
  // Exit the word
  if (exit_worldmap)
    ScreenManager::instance ()->pop_screen ();

  // Check if new worldmap is set and if so, change it
  if (new_worldmap.get ())
    {
      worldmap = new_worldmap;
      new_worldmap = boost::shared_ptr<WorldMap>();
    }
}

void
WorldMapManager::WorldMapComponent::draw (GraphicContext& gc)
{
  WorldMapManager::instance ()->worldmap->draw (gc);
}

void
WorldMapManager::WorldMapComponent::update (float delta)
{
  WorldMapManager::instance ()->worldmap->update ();
}

void 
WorldMapManager::WorldMapComponent::on_primary_button_press (int x, int y)
{
  std::cout << "Buton press" << std::endl;
  /** Fixme: insert Co. translation here */
  WorldMapManager::instance ()->worldmap->on_primary_button_press (x, y);
}

void 
WorldMapManager::change_map (const std::string& filename, NodeId node)
{
  // Create the new worldmap and make it the current one
  new_worldmap = boost::shared_ptr<WorldMap>
    (new WorldMap (path_manager.complete("worldmaps/" + filename)));
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
