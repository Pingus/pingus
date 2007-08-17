//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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
#include "../display/scene_context.hpp"
#include "../world.hpp"
#include "../timer.hpp"
#include "../resource.hpp"
#include "../globals.hpp"
#include "../blitter.hpp"
#include "../gui/display.hpp"
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

  Timer timer("Background creation");

  bg_surface = Resource::load_sprite(desc);
  bg_surface.fill(color);

  // Scaling Code
  if (stretch_x && stretch_y)
    {
      bg_surface.scale(world->get_width(), world->get_height());
    }
  else if (stretch_x && !stretch_y)
    {
      if (keep_aspect)
        {
          float aspect = bg_surface.get_height()/float(bg_surface.get_width());
          bg_surface.scale(world->get_width(), int(world->get_width()*aspect));
        }
      else
        {
          bg_surface.scale(world->get_width(), bg_surface.get_height());
        }
    }
  else if (!stretch_x && stretch_y)
    {
      if (keep_aspect)
        {
          float aspect = float(bg_surface.get_width())/bg_surface.get_height();
          bg_surface.scale(int(world->get_height() * aspect), world->get_height());
        }
      else
        {
          bg_surface.scale(bg_surface.get_width(), world->get_height());
        }
    }

  bg_surface.optimize();

  timer.stop();
}

float
SurfaceBackground::get_z_pos () const
{
  return pos.z;
}

void
SurfaceBackground::update()
{
  if (!bg_surface)
    return;

  if (scroll_x) 
    {
      scroll_ox += scroll_x;

      if (scroll_ox > bg_surface.get_width())
        scroll_ox -= bg_surface.get_width();
      else if (-scroll_ox > bg_surface.get_width())
        scroll_ox += bg_surface.get_width();
    }

  if (scroll_y) 
    {
      scroll_oy += scroll_y;

      if (scroll_oy > bg_surface.get_height())
        scroll_oy -= bg_surface.get_height();
      else if (-scroll_oy > bg_surface.get_height())
        scroll_oy += bg_surface.get_height();
    }
}

void
SurfaceBackground::draw (SceneContext& gc)
{
  if (!bg_surface)
    return;

  if (fast_mode)
    {
      Display::clear();
    }
  else
    {
      if (render_preview)
        {
#if 0 // FIXME:
          for(int y = 0; y < gc.get_height();  y += bg_surface.get_height())
            for(int x = 0; x < gc.get_width(); x += bg_surface.get_width())
              gc.color().draw(bg_surface, Vector3f(x, y));
#endif
        }
      else
        {
          int x_of = gc.color().get_clip_rect().left;
          int y_of = gc.color().get_clip_rect().top;

          int start_x;
          int start_y;

          start_x = static_cast<int>((x_of * para_x) + scroll_ox);
          start_y = static_cast<int>((y_of * para_y) + scroll_oy);

          while (start_x > 0)
            start_x -= bg_surface.get_width();

          while (start_y > 0)
            start_y -= bg_surface.get_height();

          for(int y = start_y;
              y < world->get_height();
              y += bg_surface.get_height())
            {
              for(int x = start_x;
                  x < world->get_width();
                  x += bg_surface.get_width())
                {
                  gc.color().draw(bg_surface, Vector3f(static_cast<float>(x),
                                                       static_cast<float>(y), pos.z));
                }
            }
        }
    }
}

} // namespace WorldObjs

/* EOF */
