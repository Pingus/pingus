//  $Id: SurfaceBackground.cc,v 1.14 2001/08/13 21:35:37 grumbel Exp $
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
#include <ClanLib/core.h>

#include <cassert>

#include "../globals.hh"
#include "../Timer.hh"
#include "../PingusResource.hh"
#include "../globals.hh"
#include "../blitter.hh"
#include "../caimagemanipulation/caimagemanipulation.h"

#include "SurfaceBackground.hh"

SurfaceBackground::SurfaceBackground(const SurfaceBackgroundData& bg_data)
{
  Timer timer;

  if (verbose) {
    timer.start();
    std::cout << "Creating Background..." << std::flush;
  }
  
  surface_need_deletion = false;

  if (bg_data.color.alpha > 1.0) 
    std::cout << "Background: Warning dim larger than 1.0 are no longer supported" << std::endl;
  
  // Testing animatied backgrounds...
  /*std::cout << "Res: " << bg_data.desc.res_name << std::endl
    << "file: " << bg_data.desc.datafile << std::endl;*/

  if (background_manipulation_enabled && bg_data.color.alpha != 0.0)
    {
      std::cout << "SurfaceBackground:: Manipulating background" << std::endl;
      // FIXME: This is extremly buggy and it will crash, no idea why....
      CL_Surface source_surface(PingusResource::load_surface(bg_data.desc));
      
      //CL_Canvas* canvas = Blitter::create_canvas(source_surface);
      //	  canvas->fill_rect(0, 0, 
			    //			    canvas->get_width(), canvas->get_height(),
      //			    bg_data.color.red, bg_data.color.green, bg_data.color.blue, 
      //			    bg_data.color.alpha);
      
      // FIXME: Sat Jul 21 21:57:15 2001
      std::cout << "BUG: Stuff removed because of linker error" << std::endl;
      //bg_surface = CL_Surface (canvas, true);
    }
  else
    {
      bg_surface = PingusResource::load_surface(bg_data.desc);
    }

  //bg_surface = CAImageManipulation::changeHSV(bg_surface, 150, 100, 0);
  counter.set_size(bg_surface.get_num_frames());
  counter.set_speed(1.0);

  stretch_x = bg_data.stretch_x;
  stretch_y = bg_data.stretch_y;

  scroll_x = bg_data.scroll_x;
  scroll_y = bg_data.scroll_y;

  para_x = bg_data.para_x;
  para_y = bg_data.para_y;

  pos = bg_data.pos;

  scroll_ox = 0;
  scroll_oy = 0;

  if (verbose > 1)
    {
      std::cout << "Background: Stretch_X: " << stretch_x << std::endl;
      std::cout << "Background: Stretch_Y: " << stretch_y << std::endl;
    }
  
  if (verbose)
    std::cout << "done" << timer.stop()  << std::endl;
}

SurfaceBackground::~SurfaceBackground()
{
  /*
  std::cout << "Background:~Background" << std::endl;

  if (surface_need_deletion) {
    //std::cout << "Background: Deleting background surface" << std::endl;
    // FIXME: We are /not/ deleting the surface here cause that gives
    // a segfault if the next level is loaded, I have absolutly no
    // idea why. So we have a memory hole here
    //delete bg_surface;
  }
  */
}

/*
boost::shared_ptr<SurfaceBackground>
SurfaceBackground::create (boost::shared_ptr<BackgroundData> arg_data)
{
  SurfaceBackgroundData* data = dynamic_cast<SurfaceBackgroundData*>(arg_data.get());
  return boost::shared_ptr<SurfaceBackground>(new SurfaceBackground (data));
}*/

/*
void
SurfaceBackground::load (SurfaceBackgroundData bg_data)
{
  surface_need_deletion = false;

  if (bg_data.color.alpha > 1.0) 
    std::cout << "Background: Warning dim larger than 1.0 are no longer supported" << std::endl;
  
  // Testing animatied backgrounds...
  std::cout << "Res: " << bg_data.desc.res_name << std::endl
	    << "file: " << bg_data.desc.datafile << std::endl;

  if (bg_data.desc.res_name == "none")
    {
      std::cout << "Background: No surface set..." << std::endl;
      bg_surface = 0;
    }
  else
    {
      if (background_manipulation_enabled)
	{
	  // FIXME: This is extremly buggy and it will crash, no idea why....
	  CL_Surface* source_surface = PingusResource::load_surface(bg_data.desc);

	  CL_Canvas* canvas = new CL_Canvas(source_surface->get_width(),
					    source_surface->get_height());/// Blitter::create_canvas(source_surface);
	  source_surface->put_target(0, 0, 0, canvas);
	  canvas->fill_rect(0, 0, canvas->get_width(), canvas->get_height(),
			    bg_data.color.red, bg_data.color.green, bg_data.color.blue, 
			    bg_data.color.alpha);
	  bg_surface = CL_Surface::create(canvas, true);
	}
      else
	{
	  bg_surface = PingusResource::load_surface(bg_data.desc);
	}
    }

  if (bg_surface) 
    {
      //bg_surface = CAImageManipulation::changeHSV(bg_surface, 150, 100, 0);

      counter.set_size(bg_surface->get_num_frames());
      counter.set_speed(1.0);
    }

  stretch_x = bg_data.stretch_x;
  stretch_y = bg_data.stretch_y;

  scroll_x = bg_data.scroll_x;
  scroll_y = bg_data.scroll_y;

  para_x = bg_data.para_x;
  para_y = bg_data.para_y;

  scroll_ox = 0;
  scroll_oy = 0;

  if (verbose > 1)
    {
      std::cout << "Background: Stretch_X: " << stretch_x << std::endl;
      std::cout << "Background: Stretch_Y: " << stretch_y << std::endl;
    }
}
*/
void
SurfaceBackground::update(float delta)
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
SurfaceBackground::draw_offset(int x_of, int y_of, float s)
{
  std::cout << "BACKGROUND: " << get_z_pos () << std::endl;

  if (fast_mode) 
    {
      CL_Display::clear_display();
    } 
  else 
    {
      int start_x;
      int start_y;
      
      start_x = int((x_of * para_x) + scroll_ox);
      start_y = int((y_of * para_y) + scroll_oy);
      
      if (start_x >= 0)
	start_x = start_x - bg_surface.get_width();
      
      if (start_y >= 0) 
	start_y -= bg_surface.get_height();
      else if (start_y < 0 - int(bg_surface.get_height()))
	start_y += bg_surface.get_height();
      
      if (s == 1.0) 
	{
	  for(int y = start_y; 
	      y < CL_Display::get_height(); 
	      y += bg_surface.get_height()) 
	    {
	      for(int x = start_x;
		  x < CL_Display::get_width(); 
		  x += bg_surface.get_width())
		{
		  bg_surface.put_screen(x, y, counter);
		}
	    }
	}
      else 
	{
	  std::cout << "SurfaceBackground: Zooming not supported: " << s << std::endl;
	  for(int y=(y_of/2); y < CL_Display::get_height(); y += (int)(bg_surface.get_height() * s)) {
	    for(int x = start_x; x < CL_Display::get_width(); x += (int)(bg_surface.get_width() * s)) {
	      bg_surface.put_screen(x, y, s, s);
	    }
	  }
	}
    }
}

/* EOF */
