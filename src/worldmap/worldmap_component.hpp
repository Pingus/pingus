//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_WORLDMAP_COMPONENT_HPP
#define HEADER_WORLDMAP_COMPONENT_HPP

#include <string>
#include <memory>
#include "../gui/gui_manager.hpp"

class SceneContext;

namespace WorldmapNS {

class WorldmapScreen;
class Worldmap;

class WorldmapComponent : public GUI::Component
{
private:
  std::auto_ptr<SceneContext> scene_context;
  WorldmapScreen* worldmap_screen;
  
public:
  WorldmapComponent(WorldMapScreen* worldmap_screen);
  ~WorldmapComponent();

  void on_primary_button_press (int x, int y);
  void on_secondary_button_press (int x, int y);
  void on_pointer_move(int x, int y);

  void draw (DrawingContext& gc);
  void update (float delta);

  bool is_at (int, int) { return true; }

private:
  WorldmapComponent (const WorldMapComponent&);
  WorldmapComponent& operator= (const WorldMapComponent&);
};

} // namespace WorldmapNS

#endif

/* EOF */
