//  $Id: ColMap.cc,v 1.5 2000/02/18 03:08:41 grumbel Exp $
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

void 
ColMap::remove(CL_Surface* sur, int x, int y)
{
  remove(sur->get_provider(), x, y);
}

void 
ColMap::remove(CL_SurfaceProvider* provider, int x, int y)
{
  assert(provider);
  assert(provider->get_depth() == 8);

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
	      if (colmap[i + (width*(line+y) + x)] != SOLID)
		colmap[i + (width*(line+y) + x)] = NOTHING;
	    }
	}
    }
  
  provider->unlock();
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
	cout << "ColMap: remove: Out of map" << std::endl;
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
    //    throw PingusError("Spot out of screen, please repair");
    if (verbose) std::cerr << "Warning: ColMap: Spot out of screen, please repair" << std::endl;
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
	       (i < (width * height)) && ((j - w * line) < w);
	       i++, j++)
	    {
	      if (j < 0 || j > (w * h))
		continue;
	  
	      if (i < 0 || i > (width * height))
		continue;
	  
	      //cout << " " << (int)buffer[j*4] << flush;
	      if (buffer[j*4] != 0) 
		{
		  switch (type) 
		    {
		    case surface_data::GROUND:
		      colmap[i] = WALL;
		      break;
		    case surface_data::SOLID:
		      colmap[i] = SOLID;
		      break;
		    case surface_data::BRIDGE:
		      colmap[i] = BRIDGE;
		      break;
		    default:
		      std::cout << "Colmap::put() Undefinit type" << std::endl;
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
	     (i < (width * height)) && ((j - w * line) < w);
	     ++i, ++j)
	  {
	    if (j < 0 || j > (w * h))
	      continue;
	  
	    if (i < 0 || i > (width * height))
	      continue;
	  
	    if (buffer[j]) {
	      switch (type) {
	      case surface_data::GROUND:
		colmap[i] = WALL;
		break;
	      case surface_data::SOLID:
		colmap[i] = SOLID;
		break;
	      case surface_data::BRIDGE:
		colmap[i] = BRIDGE;
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

/* EOF */
