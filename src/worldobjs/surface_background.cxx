//  $Id: surface_background.cxx,v 1.14 2003/10/21 11:01:52 grumbel Exp $
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
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/display.h>
#include "../display/scene_context.hxx"
#include "../world.hxx"
#include "../timer.hxx"
#include "../resource.hxx"
#include "../globals.hxx"
#include "../blitter.hxx"
#include "surface_background.hxx"

namespace Pingus {
namespace WorldObjs {

SurfaceBackground::SurfaceBackground(const FileReader& reader)
  : para_x(0.5),
    para_y(0.5),
    pos(Vector(0, 0, -150)),
    scroll_x(0.0),
    scroll_y(0.0),
    color(CL_Colorf(0,0,0,0)),
    stretch_x(false),
    stretch_y(false),
    keep_aspect(false),
    scroll_ox(0),
    scroll_oy(0)
{
  pos.z = -150;

  reader.read_vector("position", pos);

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

  if (color.alpha > 1.0)
    std::cout << "Background: Warning dim larger than 1.0 are no longer supported" << std::endl;

  CL_PixelBuffer canvas = Resource::load_pixelbuffer(desc);

  // Scaling Code
  if (stretch_x && stretch_y)
    {
      canvas = Blitter::scale_surface_to_canvas(canvas, world->get_width(), world->get_height());
    }
  else if (stretch_x && !stretch_y)
    {
      if (keep_aspect)
        {
          float aspect = canvas.get_height()/float(canvas.get_width());
          canvas = Blitter::scale_surface_to_canvas(canvas,
                                                    world->get_width(),
                                                    int(world->get_width()*aspect));
        }
      else
        {
          canvas = Blitter::scale_surface_to_canvas(canvas, canvas.get_width(), world->get_height());
        }
    }
  else if (!stretch_x && stretch_y)
    {
      if (keep_aspect)
        {
          float aspect = float(canvas.get_width())/canvas.get_height();
          canvas = Blitter::scale_surface_to_canvas(canvas,
                                                    int(world->get_height() * aspect),
                                                    world->get_height());
        }
      else
        {
          canvas = Blitter::scale_surface_to_canvas(canvas, canvas.get_width(), world->get_height());
        }
    }

  /* FIXME: fill_rect doesn't work with RGB images
     FIXME: seems to work fine with indexed images
     FIXME: not tested with RGBA images
     FIXME: the bug might be in create_canvas() and not in fill_rect()
  */

  if (color.alpha != 0.0 && color != CL_Colorf(0, 0, 0, 1.0f))
    { // Workaround for a bug which caused all levels to have the
      // wrong background color
      Blitter::fill_rect(canvas, CL_Rect(0, 0, canvas.get_width(), canvas.get_height()),
                         CL_Color(color));
    }

  //bg_surface = CAImageManipulation::changeHSV(bg_surface, 150, 100, 0);

  if (0) // CLANLIB_0_6
    {
      //counter.set_size(bg_surface.get_frame_count());
      //counter.set_speed(1.0);
    }

  bg_surface = CL_Surface(canvas);

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
  counter++;

  scroll_ox += scroll_x;
  scroll_oy += scroll_y;

  if (scroll_ox > bg_surface.get_width())
    {
      scroll_ox -= bg_surface.get_width();
    }
  else if (-scroll_ox > bg_surface.get_width())
    {
      scroll_ox += bg_surface.get_width();
    }

  if (scroll_oy > bg_surface.get_height())
    {
      scroll_oy -= bg_surface.get_height();
    }
  else if (-scroll_oy > bg_surface.get_height())
    {
      scroll_oy += bg_surface.get_height();
    }
}

void
SurfaceBackground::draw (SceneContext& gc)
{
  if (fast_mode)
    {
      CL_Display::clear();
    }
  else
    {
      if (render_preview)
        {
#if 0 // FIXME:
          for(int y = 0; y < gc.get_height();  y += bg_surface.get_height())
            for(int x = 0; x < gc.get_width(); x += bg_surface.get_width())
              gc.color().draw(bg_surface, Vector(x, y));
#endif
        }
      else
        {
          int x_offset = 0; // FIXME: gc.get_x_offset()
          int y_offset = 0; // FIXME: gc.get_y_offset()
          int x_of = static_cast<int>(x_offset + (gc.color().get_width ()/2));
          int y_of = static_cast<int>(y_offset + (gc.color().get_height ()/2));

          int start_x;
          int start_y;

          start_x = static_cast<int>((x_of * para_x) + scroll_ox);
          start_y = static_cast<int>((y_of * para_y) + scroll_oy);

          if (start_x >= 0)
            start_x = start_x - bg_surface.get_width();

          if (start_y >= 0)
            start_y -= bg_surface.get_height();
          else if (start_y < 0 - static_cast<int>(bg_surface.get_height()))
            start_y += bg_surface.get_height();

          for(int y = start_y;
              y < CL_Display::get_height();
              y += bg_surface.get_height())
            {
              for(int x = start_x;
                  x < CL_Display::get_width();
                  x += bg_surface.get_width())
                {
#ifdef CLANLIB_0_6
                  bg_surface.put_screen(x, y, counter); // FIXME: should use gc
#else
                  bg_surface.draw(x, y); // FIXME: should use gc
#endif
                }
            }
        }
    }
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
