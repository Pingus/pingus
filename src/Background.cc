//  $Id: Background.cc,v 1.5 2000/02/18 03:08:41 grumbel Exp $
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
	  sur = CL_Surface::load(bg.desc.res_name.c_str(), PingusResource::get(bg.desc.filename));

	  //canvas = convert_to_emptyprovider(sur->get_provider());
	  canvas = new CL_Canvas(sur->get_width(), sur->get_height());
	  canvas->lock();
	  sur->put_target(0,0,0, canvas);
	  
	  if (bg.dim > 1.0) 
	    cerr << "Background: Warning dim larger than 1.0 are no longer supported" << std::endl;
	  
	  canvas->fill_rect(0, 0, sur->get_width(), sur->get_height(),
			    bg.red, bg.green, bg.blue, bg.dim);
	  canvas->unlock();
	  bg_surface = CL_Surface::create(canvas, true);
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
  scroll_x = bg.scroll_x;
  scroll_y = bg.scroll_y;

  scroll_ox = 0;
  scroll_oy = 0;
}

Background::~Background()
{
  delete bg_surface;
}

void
Background::let_move()
{
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
      
      start_x = int((x_of / 2) + scroll_ox);
      start_y = int((y_of / 2) + scroll_oy);
      
      if (start_x >= 0)
	start_x = start_x - bg_surface->get_width();
      
      if (start_y >= 0) 
	start_y -= bg_surface->get_height();
      else if (start_y < 0 - int(bg_surface->get_height()))
	start_y += bg_surface->get_height();
      
      if (s == 1.0) 
	{
	  for(int y = start_y; y < CL_Display::get_height(); y += bg_surface->get_height()) {
	    for(int x = start_x; x < CL_Display::get_width(); x += bg_surface->get_width()) {
	      bg_surface->put_screen(x, y);
  	    }
	  }
	} 
      else 
	{
	  for(int y=(y_of/2); y < CL_Display::get_height(); y += (int)(bg_surface->get_height() * s)) {
	    for(int x = start_x; x < CL_Display::get_width(); x += (int)(bg_surface->get_width() * s)) {
	      bg_surface->put_screen(x, y, s, s);
	    }
	  }
	}
    }
}

/* EOF */
