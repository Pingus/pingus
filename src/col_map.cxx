//  $Id: col_map.cxx,v 1.2 2002/06/13 14:25:12 torangan Exp $
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

#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include "globals.hxx"
#include "pingus_resource.hxx"
#include "pingus_error.hxx"
#include "col_map.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"


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
  case ResDescriptor::RD_FILE:
    throw PingusError(_("ColMap: File load - Feature is no longer implemented")); //    return load(desc.res_name);
    break;
  case ResDescriptor::RD_RESOURCE:
    {
      CL_Surface sur;
      CL_SurfaceProvider* provider;
      
      sur = PingusResource::load_surface(desc);
      provider = sur.get_provider();
      if (provider->get_depth() != 8)
	throw PingusError(_("PingusBmpMap::get_colmap: Surface has wrong pixel format, need 8bpp!")); 

      width  = provider->get_width();
      height = provider->get_height();
     
      provider->lock();    
      
      colmap = new unsigned char[provider->get_pitch() * provider->get_height()];
      memcpy(colmap, provider->get_data(), provider->get_pitch() *  provider->get_height());
      
      // FIXME: Memory hole
      //provider->unlock();
      
      init = true;

      return 0;
    }
    break;
  default:
    assert(false);
    return 0;
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
ColMap::remove(const CL_Surface& sur, int x, int y)
{
  remove(sur.get_provider(), x, y);
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
      // FIXME: Memory hole      
      //provider->unlock();
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
      // FIXME: Memory hole      
      //provider->unlock();
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
      throw PingusError(_("ColMap::load: Reloaded ColMap!"));
      return 0;
    }
}

void
ColMap::put(const CL_Surface& sur, int sur_x, int sur_y, GroundpieceData::GPType gptype)
{
  put(sur.get_provider(), sur_x, sur_y, gptype);
}

// Puts a surface on the colmap
void
ColMap::put(CL_SurfaceProvider* provider, int sur_x, int sur_y, GroundpieceData::GPType gptype)
{
  if (gptype == GroundpieceData::GP_TRANSPARENT)
    return;

  PixelStatus pixel;
  switch (gptype) 
    {
    case GroundpieceData::GP_GROUND:  pixel = WALL; break;
    case GroundpieceData::GP_SOLID:   pixel = (PixelStatus)(SOLID | WALL); break;
    case GroundpieceData::GP_BRIDGE:  pixel = (PixelStatus)(BRIDGE | WALL); break;
    case GroundpieceData::GP_WATER:   pixel = (PixelStatus)(SOLID | WATER); break;
    case GroundpieceData::GP_LAVA:    pixel = (PixelStatus)(SOLID | LAVA); break;
    case GroundpieceData::GP_NOTHING: pixel = (PixelStatus)0; break;
    default:
      std::cout << "Colmap::put() Indefinite type" << std::endl;
      pixel = (PixelStatus)0;
      break;
    }
  
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
      float r, g, b, a;
      // Rewritting blitter for 32bit depth (using get_pixel())
      for (unsigned int y=0; y < provider->get_height(); y++)
	for (unsigned int x=0; x < provider->get_width(); x++) 
	  {
	    provider->get_pixel(x, y, &r, &g, &b, &a);
	    if (a > 0.1)
	      {
		put(x + sur_x, y + sur_y, (PixelStatus) pixel);
	      }
	  }
    }
  else if (provider->get_depth() == 8)
    {
      unsigned char* buffer;
      int swidth = provider->get_width();
      int sheight = provider->get_height();
      int y_offset = -sur_y;
      int x_offset = -sur_x;
      if (y_offset < 0) y_offset = 0;
      if (x_offset < 0) x_offset = 0;

      //provider->lock();
      buffer = static_cast<unsigned char*>(provider->get_data());

      if (provider->uses_src_colorkey())
	{
	  unsigned int colorkey = provider->get_src_colorkey();
	  for(int line = y_offset; line < sheight && (line + sur_y) < height; ++line) 
	    for (int i = x_offset; i < swidth && (i+sur_x) < width; ++i) 
	      {
		if (buffer[i + (swidth*line)] != colorkey)
		  colmap[i + (width*(line+sur_y) + sur_x)] = pixel;		    
	      }
	}
      else
	{
	  for(int line = y_offset; line < sheight && (line + sur_y) < height; ++line) 
	    for (int i = x_offset; i < swidth && (i+sur_x) < width; ++i) 
	      {
		colmap[i + (width*(line+sur_y) + sur_x)] = pixel;
	      }
	}
    }
  else
    {
      std::cout << "ColMap: Unsupported color depth, ignoring" << std::endl;
    }
  // FIXME: Memory hole
  // provider->unlock();
}

void
ColMap::draw(int x_of, int y_of, float /*s*/)
{
  CL_Canvas* canvas = new CL_Canvas(width, height);
  CL_Surface sur;
  unsigned char* buffer;

  canvas->lock();
  buffer = static_cast<unsigned char*>(canvas->get_data());
  
  for(int i = 0; i < (width * height); ++i)
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

  // FIXME: Memory hole
  //canvas->unlock();

  sur = CL_Surface(canvas, true);

  sur.put_screen(x_of, y_of);
}

/* EOF */
