//  $Id: Background.cc,v 1.16 2000/06/15 19:31:32 grumbel Exp $
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

#include <assert.h>

#include "Timer.hh"
#include "PingusResource.hh"
#include "globals.hh"
#include "blitter.hh"

#include "Background.hh"

Background::Background(background_data bg)
{
  Timer timer;

  if (verbose) {
    timer.start();
    std::cout << "Creating Background..." << std::flush;
  }

  // Load the background
  load(bg);
  
  if (verbose)
    std::cout << "done" << timer.stop()  << std::endl;
}

Background::~Background()
{
  std::cout << "Background:~Background" << std::endl;

  if (surface_need_deletion) {
    //std::cout << "Background: Deleting background surface" << std::endl;
    // FIXME: We are /not/ deleting the surface here cause that gives
    // a segfault if the next level is loaded, I have absolutly no
    // idea why. So we have a memory hole here
    //delete bg_surface;
  }
}

void
Background::load (background_data bg_data)
{
  surface_need_deletion = false;

  if (bg_data.dim > 1.0) 
    std::cout << "Background: Warning dim larger than 1.0 are no longer supported" << std::endl;
  
  // Testing animatied backgrounds...
  std::cout << "Res: " << bg_data.desc.res_name << std::endl
	    << "file: " << bg_data.desc.filename << std::endl;

  if (bg_data.desc.res_name == "none")
    {
      std::cout << "Background: No surface set..." << std::endl;
      bg_surface = 0;
    }
  else
    {
      // FIXME: Background + fill_rect() should be here, but that
      // didn't work, so it is now removed, will be rewritten if I
      // have a better day.
      bg_surface = CL_Surface::load(bg_data.desc.res_name.c_str(),
				    PingusResource::get(bg_data.desc.filename));
    }

  if (bg_surface) 
    {
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

void
Background::let_move()
{
  if (bg_surface)
    {
      counter++;

      scroll_ox += scroll_x;
      scroll_oy += scroll_y;
  
      if (scroll_ox > bg_surface->get_width()) 
	{
	  scroll_ox -= bg_surface->get_width();
	} 
      else if (-scroll_ox > bg_surface->get_width()) 
	{
	  scroll_ox += bg_surface->get_width();
	}
  
      if (scroll_oy > bg_surface->get_height()) 
	{
	  scroll_oy -= bg_surface->get_height();
	} 
      else if (-scroll_oy > bg_surface->get_height()) 
	{
	  scroll_oy += bg_surface->get_height();
	}
    }
}

void
Background::draw_offset(int x_of, int y_of, float s)
{
  if (!bg_surface || fast_mode) 
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
	start_x = start_x - bg_surface->get_width();
      
      if (start_y >= 0) 
	start_y -= bg_surface->get_height();
      else if (start_y < 0 - int(bg_surface->get_height()))
	start_y += bg_surface->get_height();
      
      if (s == 1.0) 
	{
	  for(int y = start_y; 
	      y < CL_Display::get_height(); 
	      y += bg_surface->get_height()) 
	    {
	      for(int x = start_x;
		  x < CL_Display::get_width(); 
		  x += bg_surface->get_width())
		{
		  bg_surface->put_screen(x, y, counter);
		}
	    }
	}
      else 
	{
	  std::cout << "Zooming not supported" << std::endl;
	  for(int y=(y_of/2); y < CL_Display::get_height(); y += (int)(bg_surface->get_height() * s)) {
	    for(int x = start_x; x < CL_Display::get_width(); x += (int)(bg_surface->get_width() * s)) {
	      bg_surface->put_screen(x, y, s, s);
	    }
	  }
	}
    }
}

/* EOF */
