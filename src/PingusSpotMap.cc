//  $Id: PingusSpotMap.cc,v 1.35 2000/09/29 16:21:17 grumbel Exp $
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

#include <algorithm>
#include <cstdio>

#include "PingusResource.hh"
#include "PingusError.hh"
#include "blitter.hh"
#include "algo.hh"
#include "globals.hh"
#include "Timer.hh"
#include "PingusSpotMap.hh"

using namespace std;

MapTileSurface::MapTileSurface()
{
  empty = true;
  needs_reload = false;
  surface = 0;
}

MapTileSurface::~MapTileSurface()
{
}

bool
MapTileSurface::is_empty()
{
  return empty;
}

void
MapTileSurface::set_empty(bool e)
{
  empty = e;
}

void
MapTileSurface::reload(void)
{
  //  if (needs_reload) {
  surface->reload();
  // needs_reload = false;
  //}
}

void
MapTileSurface::mark_dirty()
{
  //  needs_reload = true;
  empty = false;
}

void
MapTileSurface::check_empty()
{
  CL_SurfaceProvider* provider = surface->get_provider();
  unsigned char* buffer;
  int lenght;

  empty = true;

  provider->lock();
  lenght = provider->get_pitch() * provider->get_height(); 
  buffer = static_cast<unsigned char*>(provider->get_data());
  
  // Jumping 4 steps because of RGBA
  for(int i=0; i < lenght; i += 4) {
    if (buffer[i] != 0) {
      empty = false;
      return;
    }
  }

  surface->get_provider()->unlock();
}

PingusSpotMap::PingusSpotMap()
{
}

PingusSpotMap::PingusSpotMap(PLF* plf)
{
  colmap = 0;

  switch(plf->get_foreground().type) 
    {
    case ResDescriptor::RESOURCE:
      throw PingusError("PingusSpotMap: ResType RESOURCE is not implemented, sorry");
      break;

    case ResDescriptor::FILE:
      //if (verbose) std::cout << "PingusSpotMap: Loading... " << CL_System::get_time() << std::endl;
      load(plf);
      gen_tiles();
      break;

    default:
      throw PingusError("PingusSpotMap: Unknown resource type, bailing out");
      break;
    }
}

PingusSpotMap::~PingusSpotMap(void)
{
  std::cout << "PingusSpotMap:~PingusSpotMap" << std::endl;
  std::cout << "Trying to delete the map..." << std::flush;

  delete map_surface;
  delete map_canvas;
  delete colmap;

  for(std::vector<std::vector<MapTileSurface> >::size_type x = 0; x < tile.size(); x++) 
    {
      for(std::vector<MapTileSurface>::size_type y = 0; y < tile[x].size(); y++) {
	if (!tile[x][y].is_empty())
	  delete tile[x][y].surface;
      }
    }
  std::cout << "finished" << std::endl;
}

void
PingusSpotMap::gen_tiles(void)
{
  Timer timer;

  timer.start();

  if (verbose) std::cout << "PingusSpotMap: Generating Tiles..." << std::flush;

  if (verbose > 1) 
    {
      std::cout << "PingusSpotMap_TilewWidth: " << width / tile_size << std::endl;
      std::cout << "PingusSpotMap_TilewHeight: " << height / tile_size << std::endl;
    }  
  
   create_maptiles();

  if (verbose) std::cout << " done " << timer.stop() << std::endl;    
}

void
PingusSpotMap::load(PLF* plf)
{
  ResDescriptor name = plf->get_foreground();

  width  = plf->get_width();
  height = plf->get_height();

  // Checking that the map has the correct size, only multiples of
  // tile_size are allowed, anything else wouldn't fit very well on
  // the colmap
  if ((width % tile_size) != 0) 
    {
      std::cout << "Warrning: Width is not a multible of " << tile_size << std::endl;
      width += (tile_size - (width % tile_size));
      std::cout << "Warning: Fixing height to: " << width << std::endl;
    }
  
  if ((height % tile_size) != 0) 
    {
      std::cout << "Warning: Width is not a multible of " << tile_size << std::endl;
      height += (tile_size - (height % tile_size));
      std::cout << "Warning: Fixing height to: " << height << std::endl;
    } 

  // Allocating tile map
  tile.resize(width/tile_size);
  for(TileIter i=0; i < tile.size(); ++i) 
    tile[i].resize(height/tile_size);
  
  surfaces = plf->get_groundpieces();

  for (vector<SurfaceData>::iterator i = surfaces.begin();
       i != surfaces.end();
       i++)
    {
      i->surface = PingusResource::load_surface(i->desc);
    }
  create_map();
}
/*
// Load the map from a *.psm (Pingu Spot Map) file and load the surfaces
void
PingusSpotMap::load_psm(std::string filename)
{
  Timer timer;

  timer.start();
  std::cout << "PingusSpotMap: Parsing file..." << std::flush;
  psm_parser.parse(filename);
  std::cout << " done " << timer.stop() << std::endl;

  timer.start();
  std::cout << "PingusSpotMap: Loading surfaces..." << std::flush;
  psm_parser.load_surfaces();
  std::cout << " done " << timer.stop() << std::endl;

  timer.start();
  std::cout << "PingusSpotMap: Generating Map..." << std::flush;
  surfaces = psm_parser.get_surfaces();
}
*/
void
PingusSpotMap::create_map()
{
  // Allocating the map provider
  map_canvas = new CL_Canvas(width, height);
  
  // Is clearing the canvas really needed, or am I just work around
  // another bug...?
  Blitter::clear_canvas(map_canvas);
  
  // Drawing all surfaces to the provider
  for(std::vector<SurfaceData>::iterator i = surfaces.begin(); 
      i != surfaces.end(); 
      i++)
    {
      mark_tiles_not_empty(i->pos.x_pos, i->pos.y_pos,
			   i->surface->get_width(), i->surface->get_height());
      // test cause png
      if (i->surface->get_provider()->get_depth() == 8)
	{
	  mark_tiles_not_empty(i->pos.x_pos, i->pos.y_pos,
			      i->surface->get_width(), i->surface->get_height());
	  // FIXME: Replace this with a ClanLib built in
	  //i->surface->put_target(i->pos.x_pos, i->pos.y_pos, 0, map_canvas);
	  Blitter::put_surface(map_canvas, i->surface,
			       i->pos.x_pos, i->pos.y_pos);
	}
      else
	{
	  Blitter::put_surface(map_canvas, i->surface,
	    i->pos.x_pos, i->pos.y_pos);
	  //i->surface->put_target(i->pos.x_pos, i->pos.y_pos, 0, map_canvas);
	}
    }

  // Generate the map surface
  map_surface = CL_Surface::create(map_canvas);
  //  std::cout << " done " << timer.stop() << std::endl;
}

void
PingusSpotMap::draw_colmap(int x_pos, int y_pos, int w, int h, 
			   int x_of, int y_of, float s)
{
  colmap->draw(x_of, y_of, s);
}

// Draws the map with a offset, needed for scrolling
void
PingusSpotMap::draw(int x_pos, int y_pos, int w, int h, 
		    int of_x, int of_y, float s)
{
  //std::cout << "Draw: " << " x_pos: " << x_pos << " y_pos: " 
  //<< " w: " << w << " h: " << h << " s: " << s << std::endl;

  if (draw_collision_map)
    {
      draw_colmap(x_pos, y_pos, w, h, of_x, of_y, s);
    }
  else
    {
      if (s == 1.0)
	{
	  // Trying to calc which parts of the tilemap needs to be drawn
	  int start_x = -of_x/tile_size;
	  int start_y = -of_y/tile_size;
	  unsigned int tilemap_width = w / tile_size;
	  unsigned int tilemap_height = h / tile_size + 1;

	  //	  std::cout  << " th: " << tilemap_height << " tw: " << tilemap_width << std::endl;

	  if (start_x < 0)
	    start_x = 0;
	  if (start_y < 0)
	    start_y = 0;

	  // drawing the stuff
	  for (TileIter x = start_x; 
	       x <= (start_x + tilemap_width) && x < tile.size();
	       x++)
	    {
	      for (TileIter y = start_y;
		   y <= start_y + tilemap_height && y < tile[x].size();
		   y++)
		{
		  if (!tile[x][y].is_empty()) 
		    {
		      tile[x][y].surface->put_screen(x * tile_size + of_x, 
						     y * tile_size + of_y);
		    }
		  else
		    {
		      if (debug_tiles)
			CL_Display::fill_rect(x * tile_size + of_x, y * tile_size + of_y,
					      x * tile_size + of_x + tile_size, y * tile_size + of_y + tile_size,
					      1.0, 0.0, 0.0, 0.3);
		    }
		}
	    }
	} 
      else 
	{
	  std::cout << "PingusSpotMap: Zooming is at the moment not supported" << std::endl;
	  // map_surface->put_screen(int(of_x * s), int(of_y * s), s, s);
	}
    }
}

// Returns the width of the map, it is read directly from the *.psm file
int
PingusSpotMap::get_width(void)
{
  return width;
}

// Returns the height of the map, it is read directly from the *.psm file
int 
PingusSpotMap::get_height(void)
{
  return height;
}

void 
PingusSpotMap::remove(CL_SurfaceProvider* sprovider, int x, int y)
{
  // Get the start tile and end tile
  int start_x = x / tile_size;
  int start_y = y / tile_size;
  int end_x = (x + sprovider->get_width()) / tile_size;
  int end_y = (y + sprovider->get_height()) / tile_size;
      
  for(int ix = start_x; ix <= end_x; ++ix) 
    {
      for(int iy = start_y; iy <= end_y; ++iy) 
	{
	  if (!tile[ix][iy].is_empty()) 
	    {
	      put_alpha_surface(static_cast<CL_Canvas*>(tile[ix][iy].surface->get_provider()),
				sprovider,
				x - (ix * tile_size), y - (iy * tile_size),
				// FIXME, I am broken
				max(x, ix * tile_size),
				max(y, iy * tile_size));
	      tile[ix][iy].reload();
	    }
	}
    }
}

void
PingusSpotMap::put_alpha_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
				 int x, int y, int real_x_arg, int real_y_arg)
{
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;
  
  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  int x_offset, y_offset;

  int real_x;
  int real_y;

  //  assert(sprovider->get_depth() == 8);
  if (sprovider->get_depth() != 8)
    {
      char str[1024];
      sprintf(str, "Image has wrong color depth: %d", sprovider->get_depth());
      throw PingusError(str);
    }
  //  assert(provider->get_pixel_format() == RGBA8888);

  provider->lock();
  sprovider->lock();

  tbuffer = static_cast<unsigned char*>(provider->get_data());
  sbuffer = static_cast<unsigned char*>(sprovider->get_data());
  
  twidth = provider->get_width();
  theight = provider->get_height();
  tpitch = provider->get_pitch();

  swidth = sprovider->get_width();
  sheight = sprovider->get_height();
  spitch = sprovider->get_pitch();

  if (y < 0) {
    y_offset = 0-y;
  } else {
    y_offset = 0;
  }

  if (x < 0) {
    x_offset = -x;
  } else {
    x_offset = 0;
  }

  real_y = real_y_arg;
  real_x = real_x_arg;
  
  for(int line=y_offset; line < sheight && (line + y) < theight; ++line) 
    {
      start_i = ((line + y) * tpitch) + (x*4);
      
      real_x = real_x_arg;
      for(int i=start_i+(4*x_offset),j=line*spitch+x_offset; 
	  i < start_i + (4*swidth) && (i-start_i+(x*4)) < (4*twidth); i+=4,++j)
	{
	  if (sbuffer[j]) 
	    {
	      if (debug_actions)
		{
		  if (!(colmap->getpixel(real_x, real_y) & ColMap::SOLID)) 
		    {
		      tbuffer[i + 0] = 255;
		      tbuffer[i + 1] = 255;
		      tbuffer[i + 2] = 255;
		      tbuffer[i + 3] = 255;
		    } 
		  else 
		    {
		      tbuffer[i + 0] = 255;
		      tbuffer[i + 1] = 255;
		      tbuffer[i + 2] = 0;
		      tbuffer[i + 3] = 0;
		    }
		}
	      else
		{
		  if (!(colmap->getpixel(real_x, real_y) & ColMap::SOLID))
		    {
		      tbuffer[i + 0] = 0;		      
		    }
		}
	    }
	  ++real_x;
	}
      ++real_y;
    }

  sprovider->unlock();
  provider->unlock();  
}

void
PingusSpotMap::put(CL_SurfaceProvider* sprovider, int x, int y)
{
  // Get the start tile and end tile
  int start_x = x / tile_size;
  int start_y = y / tile_size;
  int end_x = (x + sprovider->get_width()) / tile_size;
  int end_y = (y + sprovider->get_height()) / tile_size;
    
  if (start_x < 0)
    start_x = 0;
  if (start_y < 0)
    start_y = 0;

  if (end_x > (int) tile.size() - 1)
    end_x = tile.size() - 1;
  if (end_y > (int) tile[0].size() - 1)
    end_y = tile[0].size() - 1;

  for(int ix = start_x; ix <= end_x; ++ix) 
    {
      for(int iy = start_y; iy <= end_y; ++iy) 
	{
	  if (tile[ix][iy].surface == 0)
	    {
	      CL_Canvas* canvas;
	      std::cout << "PingusSpotMap: Drawing to an emtpy tile: " << ix << " " << iy << std::endl;
	      canvas = new CL_Canvas(tile_size, tile_size);
	      Blitter::clear_canvas(canvas);

	      Blitter::put_surface(canvas, sprovider,
				   x - (ix * tile_size), y - (iy * tile_size));

	      tile[ix][iy].surface = CL_Surface::create(canvas, true);
	    }
	  else
	    {
	      Blitter::put_surface(static_cast<CL_Canvas*>(tile[ix][iy].surface->get_provider()),
				   sprovider,
				   x - (ix * tile_size), y - (iy * tile_size));
	    }
	  /*
	  CL_Surface* s = CL_Surface::create(sprovider);
	  s->put_target(x - (ix * tile_size), y - (iy * tile_size), 0, 
			tile[ix][iy].surface->get_provider());*/
	  tile[ix][iy].reload();
	  tile[ix][iy].set_empty(false);
	}
    }
}

// Generates a ColMap (with new) and returns a pointer to it
ColMap*
PingusSpotMap::get_colmap(void)
{
  Timer timer;

  if (colmap) 
    {
      return colmap;
    } 
  else 
    {
      if (verbose) {
	std::cout << "PingusSpotMap: Starting ColMap creation..." << std::endl;
      }
      unsigned char* buffer;
      
      // Allocate the space for the colmap buffer
      // But don't delete it, since the ColMap will do that.
      buffer = new unsigned char[width * height];
      
      if (verbose) {
	timer.start();
	std::cout << "PingusSpotMap: Clearing ColMap buffer..." << std::flush;
      }
      for(int i=0; i < width * height; ++i) 
	{
	  buffer[i] = 0;
	}
      
      if (verbose) std::cout << "done " << timer.stop() << std::endl;

      // Create a empty ColMap
      colmap = new ColMap(buffer, width, height);
      
      if (verbose) {
	timer.start();
	std::cout << "PingusSpotMap: Generating Colision Map..." << std::flush;
      }

      for(std::vector<SurfaceData>::iterator i2 = surfaces.begin();
	  i2 != surfaces.end(); 
	  i2++) 
	{
	  colmap->put(i2->surface, i2->pos.x_pos, i2->pos.y_pos, i2->type);
	}
      
      if (verbose)
	std::cout << "done " << timer.stop() << std::endl;
      
      return colmap;
    }
}

CL_Surface*
PingusSpotMap::get_surface(void)
{
  std::cout << "PingusSpotMap::get_surface() not supported" << std::endl;
  return map_surface;
}

void
PingusSpotMap::create_maptiles()
{
  CL_Canvas* canvas;

  for(TileIter x=0; x < tile.size(); ++x) 
    {
      for(TileIter y=0; y < tile[x].size(); ++y) 
	{
	  if (!tile[x][y].is_empty())
	    {
	      canvas = new CL_Canvas(tile_size, tile_size);
	      Blitter::clear_canvas(canvas);
	      //map_surface->put_target(-x * tile_size, -y * tile_size, 0, canvas);
	      Blitter::put_surface(canvas, map_surface, -x * tile_size, -y * tile_size);
	      tile[x][y].surface = CL_Surface::create(canvas, true);
	    }
	}
    }
}

void
PingusSpotMap::mark_tiles_not_empty(int x_pos, int y_pos, int sur_width, int sur_height)
{
  int start_x = max(0, x_pos / tile_size);
  int start_y = max(0, y_pos / tile_size);
  int stop_x  = min(width / tile_size,  (x_pos + sur_width - 1) / tile_size + 1);
  int stop_y  = min(height / tile_size, (y_pos + sur_height - 1) / tile_size + 1);

  //cout << "X: " << start_x << " Y: " << start_y << endl;
  //cout << "stop_X: " << stop_x << " stop_Y: " << stop_y << endl;

  for(int y = start_y; y < stop_y; y++) {
    for(int x = start_x; x < stop_x; x++) {
      tile[x][y].set_empty(false);
    }
  }
}

/* EOF */
