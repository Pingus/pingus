//  $Id: Background.cc,v 1.10 2000/04/10 21:33:06 grumbel Exp $
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

#include "PingusResource.hh"
#include "globals.hh"
#include "blitter.hh"

#include "Background.hh"

Background::Background(background_data bg)
{
  CL_Canvas* canvas;
  CL_Surface* sur;
  
  if (bg.desc.filename == "none")
    {
      bg_surface = 0;
    }
  else
    {
      try
	{
	  /* Testing animatied backgrounds... */
	  sur = CL_Surface::load(bg.desc.res_name.c_str(), PingusResource::get(bg.desc.filename));

	  if (sur->get_num_frames() > 1)
	    {
	      bg_surface = sur;
	    }
	  else
	    {
	      canvas = new CL_Canvas(sur->get_width(), sur->get_height(), sur->get_num_frames());
	    
	      if (bg.dim > 1.0) 
		std::cout << "Background: Warning dim larger than 1.0 are no longer supported" << std::endl;

	      canvas->fill_rect(0, 0,
				sur->get_width(),
				sur->get_height() * sur->get_num_frames(),
				1.0, 1.0, 1.0, 1.0);
	      /*
	      std::cout << "width:" << canvas->get_width() << std::endl;
	      std::cout << "height:" << canvas->get_height() << std::endl;
	      std::cout << "pitch:" << canvas->get_pitch() << std::endl;

	      std::cout << "---------------------" << std::endl;
	      */
	      for (unsigned int i = 0; i < sur->get_num_frames(); i++)
		{
		  sur->put_target(0,i * sur->get_height(), i, canvas);
	      
		  canvas->fill_rect(0, 
				    i * sur->get_height(),
				    sur->get_width(),
				    i * sur->get_height() + sur->get_height(),
				    bg.red, bg.green, bg.blue, bg.dim);
		}
	      bg_surface = CL_Surface::create(canvas, false);
	      /*
	      std::cout << "Frames: " << bg_surface->get_num_frames() << std::endl;
	      std::cout << "width: " << bg_surface->get_width() << std::endl;
	      std::cout << "height: " << bg_surface->get_height() << std::endl;
	      */
	      delete canvas;
	    }	  
	  counter.set_size(bg_surface->get_num_frames());
	  counter.set_speed(1.0);
	}

      catch (CL_Error err)
	{
	  std::cout << "-------------------------------------\n" 
		    << "Bug triggered, workaround enabled\n"
		    << "The game might crash real soon, sorry\n" 
		    << "CL_Error: " << err.message  << "\n"
		    << "-------------------------------------" << std::endl;
	  bg_surface = 0;
	}
    }

  stretch_x = bg.stretch_x;
  stretch_y = bg.stretch_y;

  scroll_x = bg.scroll_x;
  scroll_y = bg.scroll_y;

  para_x = bg.para_x;
  para_y = bg.para_y;

  scroll_ox = 0;
  scroll_oy = 0;

  std::cout << "Stretch_X: " << stretch_x << std::endl;
  std::cout << "Stretch_Y: " << stretch_y << std::endl;
}

Background::~Background()
{
}

void
Background::let_move()
{
  counter++;

  if (bg_surface)
    {
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
