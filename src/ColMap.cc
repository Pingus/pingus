//  $Id: ColMap.cc,v 1.13 2000/06/11 15:23:29 grumbel Exp $
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
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ClanLib/core.h>

#include "algo.hh"
#include "globals.hh"
#include "PingusResource.hh"
#include "PingusError.hh"

#include "ColMap.hh"

ColMap::ColMap()
{
  init = false;
}

// Obtain the colmap from a memory area
ColMap::ColMap(unsigned char* b, int w, int h)
{
  init = true;
  colmap = b;
  width = w;
  height = h;
}

ColMap::~ColMap()
{
  std::cout << "ColMap:~ColMap" << std::endl;
  delete[] colmap;
}

int
ColMap::load(ResDescriptor desc)
{
  switch(desc.type) {
  case ResDescriptor::FILE:
    throw PingusError("ColMap: File load - Feature is no longer implemented"); //    return load(desc.res_name);
    break;
  case ResDescriptor::RESOURCE:
    {
      CL_Surface* sur;
      CL_SurfaceProvider* provider;
      
      sur = CL_Surface::load(desc.res_name.c_str(), PingusResource::get(desc.filename));
      provider = sur->get_provider();
      if (provider->get_depth() != 8)
	throw PingusError("PingusBmpMap::get_colmap: Surface has wrong pixel format, need 8bpp!"); 

      width  = provider->get_width();
      height = provider->get_height();
     
      provider->lock();    
      
      colmap = new unsigned char[provider->get_pitch() * provider->get_height()];
      memcpy(colmap, provider->get_data(), provider->get_pitch() *  provider->get_height());
      
      provider->unlock();
      
      init = true;

      return 0;
    }
    break;
  default:
    assert(false);
    break;
  }
}

int
ColMap::getpixel(int x, int y)
{
  assert(init);

  if (x >= 0 && x < width && y >= 0 && y < height) {
    return colmap[x+y*width];
  } else {
    return OUTOFSCREEN; 
  }
}

unsigned char* 
ColMap::get_data()
{
  return colmap;
}

int
ColMap::get_height()
{
  return height;
}

int
ColMap::get_width()
{
  return width;
}

void 
ColMap::remove(CL_Surface* sur, int x, int y)
{
  remove(sur->get_provider(), x, y);
}

void 
ColMap::remove(CL_SurfaceProvider* provider, int x, int y)
{
  assert(provider);

  if (provider->get_depth() == 32) 
    {
      unsigned char* buffer;
      int swidth = provider->get_width();
      int sheight = provider->get_height();
      int y_offset = -y;
      int x_offset = -x;
      if (y_offset < 0) y_offset = 0;
      if (x_offset < 0) x_offset = 0;

      provider->lock();
      buffer = static_cast<unsigned char*>(provider->get_data());

      for(int line = y_offset; line < sheight && (line + y) < height; ++line) 
	{
	  for (int i = x_offset; i < swidth && (i+x) < width; ++i) 
	    {
	      if (buffer[(i + (swidth*line)) * 4] != 0) 
		{
		  if (!(colmap[i + (width*(line+y) + x)] & SOLID))
		    colmap[i + (width*(line+y) + x)] = NOTHING;
		}
	    }
	}
      provider->unlock();
    }
  else if (provider->get_depth() == 8)
    {
      unsigned char* buffer;
      int swidth = provider->get_width();
      int sheight = provider->get_height();
      int y_offset = -y;
      int x_offset = -x;
      if (y_offset < 0) y_offset = 0;
      if (x_offset < 0) x_offset = 0;

      provider->lock();
      buffer = static_cast<unsigned char*>(provider->get_data());

      for(int line = y_offset; line < sheight && (line + y) < height; ++line) 
	{
	  for (int i = x_offset; i < swidth && (i+x) < width; ++i) 
	    {
	      if (buffer[i + (swidth*line)]) 
		{
		  if (!(colmap[i + (width*(line+y) + x)] & SOLID))
		    colmap[i + (width*(line+y) + x)] = NOTHING;
		}
	    }
	}
      provider->unlock();
    }
  else
    {
      assert(!"ColMap::remove: Color depth not supported");
    }
}

void
ColMap::put(int x, int y, PixelStatus p)
{
  if (   x > 0 && x < width
      && y > 0 && y < height) 
    {
      colmap[x+y*width] = p;
    }
  else
    {
      if (verbose > 2)
	std::cout << "ColMap: remove: Out of map" << std::endl;
    }
}

int
ColMap::load(unsigned char* b, int w, int h)
{
  if (!init) 
    {
      std::cout << "ColMap: loading..." << std::endl;
      init = true;
      colmap = b;
      width = w;
      height = h;
      return 1;
    } else {
      throw PingusError("ColMap::load: Reloaded ColMap!");
      return 0;
    }
}

void
ColMap::put(CL_Surface* sur, int sur_x, int sur_y, surface_data::Type type)
{
  put(sur->get_provider(), sur_x, sur_y, type);
}

// Puts a surface on the colmap
void
ColMap::put(CL_SurfaceProvider* provider, int sur_x, int sur_y, surface_data::Type type)
{
  if ((sur_x > width) || (sur_y > height)) {
    if (verbose > 3) {
      std::cout << "Warning: ColMap: Spot out of screen" << std::endl;
      std::cout << "sur_x: " << sur_x << " sur_y: " << sur_y << std::endl;
    }
    return;
  }

  provider->lock();
  
  if (provider->get_depth() == 32) 
    {
      //std::cout << "ColMap: Not a 256 color hotspot! Ignoring the surface." << std::endl;
      unsigned char* buffer;
      int w = provider->get_width();
      int h = provider->get_height();
      
      buffer = static_cast<unsigned char*>(provider->get_data());
      
      for(int line = 0; line < h; ++line) 
	{
	  for (int i = (width * (sur_y + line)) + sur_x, j=w * line;
	       (i < (width * height)) && ((j - w * line) < w) && (i < (width * (sur_y + line + 1)));
	       i++, j++)
	    {
	      if (j < 0 || j > (w * h))
		continue;
	  
	      if (i < 0 || i > (width * height))
		continue;
	  
	      if (buffer[j]) 
		{
		  switch (type) 
		    {
		    case surface_data::GROUND:
		      colmap[i] = WALL;
		      break;
		    case surface_data::TRANSPARENT:
		      // doing nothing
		      break;
		    case surface_data::SOLID:
		      colmap[i] = SOLID | WALL;
		      break;
		    case surface_data::BRIDGE:
		      colmap[i] = BRIDGE;
		      break;
		    case surface_data::WATER:
		      colmap[i] = SOLID | WATER;
		      break;
		    case surface_data::LAVA:
		      colmap[i] = SOLID | LAVA;
		      break;
		    case surface_data::NOTHING:
		      colmap[i] = 0;
		      break;
		    }
		}
	    }
	}
    }
  else if ((provider->get_depth() == 8)) 
    {
      unsigned char* buffer;
      int w = provider->get_width();
      int h = provider->get_height();

      buffer = static_cast<unsigned char*>(provider->get_data());
    
      for(int line = 0; line < h; ++line) {
	for (int i = (width * (sur_y + line)) + sur_x, j=w * line;
	     (i < (width * height)) && ((j - w * line) < w)  && (i < (width * (sur_y + line + 1)));
	     ++i, ++j)
	  {
	    if (j < 0 || j > (w * h))
	      continue;
	  
	    if (i < 0 || i > (width * height))
	      continue;
	  
	    if (!provider->uses_src_colorkey() || buffer[j] != provider->get_src_colorkey())
	      {
		switch (type) 
		  {
		  case surface_data::GROUND:
		    colmap[i] = WALL;
		    break;
		  case surface_data::TRANSPARENT:
		    // doing nothing
		    break;
		  case surface_data::SOLID:
		    colmap[i] = SOLID | WALL;
		    break;
		  case surface_data::BRIDGE:
		    colmap[i] = BRIDGE | WALL;
		    break;
		  case surface_data::WATER:
		    colmap[i] = SOLID | WATER;
		    break;
		  case surface_data::LAVA:
		    colmap[i] = SOLID | LAVA;
		    break;
		  default:
		    std::cout << "Colmap::put() Undefinit type" << std::endl;
		    break;
		  }
	      }
	  }
      }
    }
  else
    {
      std::cout << "ColMap: Unsupported color depth, ignoring" << std::endl;
    }
  provider->unlock();
}

void
ColMap::draw(int x_of, int y_of, float s)
{
  CL_Canvas* canvas = new CL_Canvas(width, height);
  CL_Surface* sur;
  unsigned char* buffer;

  canvas->lock();
  buffer = static_cast<unsigned char*>(canvas->get_data());
  
  for(int i = 0; i < (width * height); i++)
    {
      switch(colmap[i])
	{
	case ColMap::NOTHING:
	  buffer[i * 4 + 0] = 0;
	  buffer[i * 4 + 1] = 0;
	  buffer[i * 4 + 2] = 0;
	  buffer[i * 4 + 3] = 0;
	  break;
	case ColMap::SOLID:
	  buffer[i * 4 + 0] = 255;
	  buffer[i * 4 + 1] = 100;
	  buffer[i * 4 + 2] = 100;
	  buffer[i * 4 + 3] = 100;
	  break;
	default:
	  buffer[i * 4 + 0] = 255;
	  buffer[i * 4 + 1] = 200;
	  buffer[i * 4 + 2] = 200;
	  buffer[i * 4 + 3] = 200;
	  break;
	}
    }

  canvas->unlock();

  sur = CL_Surface::create(canvas, false);

  sur->put_screen(x_of, y_of);

  delete sur;
  delete canvas;
}

/* EOF */
