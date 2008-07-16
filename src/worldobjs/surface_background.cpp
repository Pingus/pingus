//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "../display/scene_context.hpp"
#include "../world.hpp"
#include "../resource.hpp"
#include "../globals.hpp"
#include "../blitter.hpp"
#include "../display/display.hpp"
#include "surface_background.hpp"

namespace WorldObjs {

SurfaceBackground::SurfaceBackground(const FileReader& reader)
  : para_x(0.5),
    para_y(0.5),
    scroll_x(0.0),
    scroll_y(0.0),
    color(0,0,0,0),
    stretch_x(false),
    stretch_y(false),
    keep_aspect(false),
    scroll_ox(0),
    scroll_oy(0)
{
  if (!reader.read_vector("position", pos))
    pos = Vector3f(0.f, 0.f, -150.f);

  ResDescriptor desc;

  reader.read_desc("surface", desc);
  reader.read_color("color", color);

  reader.read_float("para-x", para_x);
  reader.read_float("para-y", para_y);

  reader.read_float("scroll-x", scroll_x);
  reader.read_float("scroll-y", scroll_y);

  reader.read_bool("stretch-x", stretch_x);
  reader.read_bool("stretch-y", stretch_y);

  reader.read_bool("keep-aspect", keep_aspect);

  Surface surface = Resource::load_surface(desc);

  surface.fill(color);

  // Scaling Code
  if (stretch_x && stretch_y)
    {
      surface = surface.scale(world->get_width(), world->get_height());
    }
  else if (stretch_x && !stretch_y)
    {
      if (keep_aspect)
        {
          float aspect = surface.get_height()/float(surface.get_width());
          surface = surface.scale(world->get_width(), int(world->get_width()*aspect));
        }
      else
        {
          surface = surface.scale(world->get_width(), surface.get_height());
        }
    }
  else if (!stretch_x && stretch_y)
    {
      if (keep_aspect)
        {
          float aspect = float(surface.get_width())/surface.get_height();
          surface = surface.scale(int(world->get_height() * aspect), world->get_height());
        }
      else
        {
          surface = surface.scale(surface.get_width(), world->get_height());
        }
    }

  bg_sprite = Sprite(surface);
}

float
SurfaceBackground::get_z_pos () const
{
  return pos.z;
}

void
SurfaceBackground::update()
{
  if (!bg_sprite)
    return;

  if (scroll_x) 
    {
      scroll_ox += scroll_x;

      if (scroll_ox > bg_sprite.get_width())
        scroll_ox -= bg_sprite.get_width();
      else if (-scroll_ox > bg_sprite.get_width())
        scroll_ox += bg_sprite.get_width();
    }

  if (scroll_y) 
    {
      scroll_oy += scroll_y;

      if (scroll_oy > bg_sprite.get_height())
        scroll_oy -= bg_sprite.get_height();
      else if (-scroll_oy > bg_sprite.get_height())
        scroll_oy += bg_sprite.get_height();
    }
}

void
SurfaceBackground::draw (SceneContext& gc)
{
  if (!bg_sprite)
    return;

  
  Vector3f offset = gc.color().world_to_screen(Vector3f(0,0));

  offset.x -= gc.color().get_rect().left;
  offset.y -= gc.color().get_rect().top;

  int start_x = static_cast<int>((offset.x * para_x) + scroll_ox);
  int start_y = static_cast<int>((offset.y * para_y) + scroll_oy);

  if (start_x > 0)
    start_x = (start_x % bg_sprite.get_width()) - bg_sprite.get_width();

  if (start_y > 0)
    start_y = (start_y % bg_sprite.get_height()) - bg_sprite.get_height();

  for(int y = start_y;
      y < world->get_height();
      y += bg_sprite.get_height())
    {
      for(int x = start_x;
          x < world->get_width();
          x += bg_sprite.get_width())
        {
          gc.color().draw(bg_sprite, Vector3f(x - offset.x, 
                                              y - offset.y, pos.z));
        }
    }
}

} // namespace WorldObjs

/* EOF */
