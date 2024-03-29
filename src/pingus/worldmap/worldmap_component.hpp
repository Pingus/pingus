// Pingus - A free Lemmings clone
// Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_WORLDMAP_COMPONENT_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_WORLDMAP_COMPONENT_HPP

#include <memory>

#include "engine/gui/gui_manager.hpp"

class SceneContext;

namespace pingus::worldmap {

class WorldmapScreen;
class Worldmap;

class WorldmapComponent : public pingus::gui::Component
{
public:
  WorldmapComponent(WorldmapScreen* worldmap_screen);
  ~WorldmapComponent() override;

  void on_primary_button_press (int x, int y) override;
  void on_secondary_button_press (int x, int y) override;
  void on_pointer_move(int x, int y) override;

  void on_fast_forward_press();
  void on_fast_forward_release();

  void draw (DrawingContext& gc) override;
  void update (float delta) override;

  bool is_at (int, int) override { return true; }

private:
  std::unique_ptr<SceneContext> scene_context;
  WorldmapScreen* worldmap_screen;
  bool m_fast_forward;

private:
  WorldmapComponent (WorldmapComponent const&);
  WorldmapComponent& operator= (WorldmapComponent const&);
};

} // namespace pingus::worldmap

#endif

/* EOF */
