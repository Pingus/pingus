//  $Id: surface_background.cxx,v 1.5 2002/10/07 23:04:06 grumbel Exp $
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
#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include <ClanLib/Display/Display/display.h>
#include "../graphic_context.hxx"
#include "../world.hxx"
#include "../timer.hxx"
#include "../pingus_resource.hxx"
#include "../globals.hxx"
#include "../blitter.hxx"
#include "../worldobjsdata/surface_background_data.hxx"
#include "surface_background.hxx"

namespace WorldObjs {

SurfaceBackground::SurfaceBackground (const WorldObjsData::SurfaceBackgroundData& data_)
  : scroll_ox(0),
    scroll_oy(0),
    data(new WorldObjsData::SurfaceBackgroundData(data_))

{
  Timer timer("Background creation");

  if (data->color.alpha > 1.0) 
    std::cout << "Background: Warning dim larger than 1.0 are no longer supported" << std::endl;
  
  // Testing animatied backgrounds...
  /*std::cout << "Res: " << data->desc.res_name << std::endl
    << "file: " << data->desc.datafile << std::endl;*/

  if (background_manipulation_enabled)
    {
      std::cout << "------ SurfaceBackground:: Manipulating background ------" << std::endl;
      CL_Surface source_surface = PingusResource::load_surface(data->desc);
      
      CL_Canvas* canvas;

      // Scaling Code
      if (data->stretch_x && data->stretch_y)
	canvas = Blitter::scale_surface_to_canvas(source_surface, world->get_width(), world->get_height());
      else if (data->stretch_x && !data->stretch_y)
	canvas = Blitter::scale_surface_to_canvas(source_surface, world->get_width(), source_surface.get_height());
      else if (!data->stretch_x && data->stretch_y)
	canvas = Blitter::scale_surface_to_canvas(source_surface, source_surface.get_width(), world->get_height());
      else
	canvas = Blitter::create_canvas(source_surface);

      /* FIXME: fill_rect doesn't work with RGB images
	 FIXME: seems to work fine with indexed images
	 FIXME: not tested with RGBA images
	 FIXME: the bug might be in create_canvas() and not in fill_rect()
      */
    
      if (data->color.alpha != 0.0 && data->color != Color(0, 0, 0, 1.0f))
	{ // Workaround for a bug which caused all levels to have the
	  // wrong background color
	  canvas->fill_rect(0, 0, 
			    canvas->get_width(), canvas->get_height(),
			    data->color.red, data->color.green, data->color.blue, 
			    data->color.alpha);
	}
      
      bg_surface = CL_Surface(canvas, true);
      std::cout << "Surface: " << bg_surface.get_width() << " " << bg_surface.get_height() << std::endl;
    }
  else
    {
      bg_surface = PingusResource::load_surface(data->desc);
    }

  //bg_surface = CAImageManipulation::changeHSV(bg_surface, 150, 100, 0);
  counter.set_size(bg_surface.get_num_frames());
  counter.set_speed(1.0);

  if (verbose > 1)
    {
      std::cout << "Background: data->stretch_x: " << data->stretch_x << std::endl;
      std::cout << "Background: data->stretch_y: " << data->stretch_y << std::endl;
    }
  
  timer.stop();
}

SurfaceBackground::~SurfaceBackground ()
{
  delete data;
}

float
SurfaceBackground::get_z_pos () const
{
  return data->pos.z;
}

void
SurfaceBackground::update()
{
  counter++;

  scroll_ox += data->scroll_x;
  scroll_oy += data->scroll_y;
  
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
SurfaceBackground::draw (GraphicContext& gc)
{
  if (fast_mode) 
    {
      CL_Display::clear_display();
    } 
  else 
    {
      int x_of = static_cast<int>(gc.get_x_offset () + (gc.get_width ()/2));
      int y_of = static_cast<int>(gc.get_y_offset () + (gc.get_height ()/2));

      int start_x;
      int start_y;
      
      start_x = static_cast<int>((x_of * data->para_x) + scroll_ox);
      start_y = static_cast<int>((y_of * data->para_y) + scroll_oy);
      
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
	      bg_surface.put_screen(x, y, counter); // FIXME: should use gc
	    }
	}
    }
}

} // namespace WorldObjs

/* EOF */
