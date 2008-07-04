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

#include "../display/display.hpp"
#include "../display/scene_context.hpp"
#include "worldmap_screen.hpp"
#include "worldmap.hpp"
#include "worldmap_component.hpp"

namespace WorldMapNS {

WorldMapComponent::WorldMapComponent(WorldMapScreen* worldmap_screen)
  : worldmap_screen(worldmap_screen)
{
  scene_context = new SceneContext();
}

WorldMapComponent::~WorldMapComponent()
{
  delete scene_context;
}

void
WorldMapComponent::draw (DrawingContext& gc)
{
  WorldMap* worldmap = worldmap_screen->worldmap;

  Rect cliprect = worldmap_screen->get_trans_rect();

  scene_context->clear();
  scene_context->push_modelview();
  scene_context->translate((float)cliprect.left, (float)cliprect.top);

  scene_context->set_cliprect(cliprect);

  //scene_context->color().draw_fillrect(-100, -100, 2000, 2000, Color(255,0,0,0), -10000);
  worldmap->draw(scene_context->color());

  gc.draw(new SceneContextDrawingRequest(scene_context, Vector3f(0,0,-1000)));

  scene_context->pop_modelview();

  // Draw border
  if (cliprect != Rect(Vector2i(0,0), Size(Display::get_width(), Display::get_height())))
    {
      Color border_color(50, 65, 75);
      // top
      gc.draw_fillrect(0, 0, Display::get_width(), cliprect.top,
                       border_color);
      // bottom
      gc.draw_fillrect(0, cliprect.bottom, Display::get_width(), Display::get_height(),
                       border_color);
      // left
      gc.draw_fillrect(0, cliprect.top, cliprect.left, cliprect.bottom,
                       border_color);
      // right
      gc.draw_fillrect(cliprect.right, cliprect.top, Display::get_width(), cliprect.bottom,
                       border_color);
    }
}

void
WorldMapComponent::update (float delta)
{
  worldmap_screen->worldmap->update(delta);
}


void
WorldMapComponent::on_primary_button_press (int x, int y)
{
  Rect cliprect = worldmap_screen->get_trans_rect();
  worldmap_screen->worldmap->on_primary_button_press(x - cliprect.left,
                                                     y - cliprect.top);
}


void
WorldMapComponent::on_pointer_move (int x, int y)
{
  Rect cliprect = worldmap_screen->get_trans_rect();
  worldmap_screen->worldmap->on_pointer_move(x - cliprect.left,
                                             y - cliprect.top);
}

void
WorldMapComponent::on_secondary_button_press (int x, int y)
{
  Rect cliprect = worldmap_screen->get_trans_rect();
  worldmap_screen->worldmap->on_secondary_button_press(x - cliprect.left,
                                                       y - cliprect.top);
}

} // namespace WorldMapNS

/* EOF */
