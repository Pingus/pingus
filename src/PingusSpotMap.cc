//  $Id: PingusSpotMap.cc,v 1.5 2000/02/16 23:34:11 grumbel Exp $
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

#include "PingusResource.hh"
#include "PingusError.hh"
#include "blitter.hh"
#include "globals.hh"

#include "PingusSpotMap.hh"

MapTileSurface::MapTileSurface()
{
  empty = true;
  needs_reload = false;
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

  switch(plf->get_fg().type) 
    {
    case ResDescriptor::RESOURCE:
      throw PingusError("PingusSpotMap: ResType RESOURCE is not implemented, sorry");
      break;

    case ResDescriptor::FILE:
      if (verbose) std::cout << "PingusSpotMap: Loading..." << std::endl;
      load(plf);
      if (verbose) std::cout << "PingusSpotMap: Generating Tiles..." << std::endl;
      gen_tiles();
      if (verbose) std::cout << "PingusSpotMap: Generating Tiles... done" << std::endl;    
      break;

    default:
      throw PingusError("PingusSpotMap: Unknown resource type, bailing out");
      break;
    }
}

PingusSpotMap::~PingusSpotMap(void)
{
  delete map_surface;
  std::cout << "Trying to delete the map..." << std::flush;
  for(std::vector<vector<MapTileSurface> >::size_type x = 0; x < tile.size(); x++) 
    {
      for(std::vector<MapTileSurface>::size_type y = 0; y < tile[x].size(); y++) {
	delete tile[x][y].surface;
      }
    }
  delete colmap;
  std::cout << "finished" << std::endl;
}

void
PingusSpotMap::gen_tiles(void)
{
  if (verbose) {
    std::cout << "PingusSpotMap_TilewWidth: " << width / tile_size << std::endl;
    std::cout << "PingusSpotMap_TilewHeight: " << height / tile_size << std::endl;
  }  
  
  tile.resize(width/tile_size);
  for(TileIter i=0; i < tile.size(); ++i) {
    tile[i].resize(height/tile_size);
  }
  create_maptiles(map_surface->get_provider());
}

void
PingusSpotMap::load(PLF* plf)
{
  ResDescriptor name = plf->get_fg();
  width  = plf->get_width();
  height = plf->get_height();
  load(plf->get_fg().res_name);
}

// Load the map from a *.psm (Pingu Spot Map) file and load the surfaces
void
PingusSpotMap::load(std::string filename)
{
  psm_parser.parse(filename);
  psm_parser.load_surfaces();
  surfaces = psm_parser.get_surfaces();

  if ((width % tile_size) != 0) 
    {
      cerr << "Warrning: Width is not a multible of " << tile_size << std::endl;
      width += (tile_size - (width % tile_size));
      cerr << "Warning: Fixing height to: " << width << std::endl;
    }
  
  if ((height % tile_size) != 0) 
    {
      cerr << "Warning: Width is not a multible of " << tile_size << std::endl;
      height += (tile_size - (height % tile_size));
      cerr << "Warning: Fixing height to: " << height << std::endl;
    }
  
  // Allocating the map provider
  provider = new CL_Canvas(width, height, 1);

  // Drawing all surfaces to the provider
  for(std::vector<surface_data>::size_type i=0; i < surfaces.size(); ++i) 
    {
      // test cause png
       put_surface(provider, surfaces[i].surface->get_provider(),
      surfaces[i].x_pos, surfaces[i].y_pos);
      //CL_Target* target = provider;
       //surfaces[i].surface->put_target(surfaces[i].x_pos, surfaces[i].y_pos, 0, provider);
    }

  // Generate the map surface
  map_surface = CL_Surface::create(provider, true);
}

// Draws the map with a offset, needed for scrolling
void
PingusSpotMap::draw_offset(int of_x, int of_y, float s)
{
  if (s == 1.0) 
    {
      for (TileIter x = 0; x < tile.size(); ++x) 
	{
	  for (TileIter y = 0; y < tile[x].size(); ++y) 
	    {
	      if (tile[x][y].is_empty()) 
		{
		  // Uncomment the following lines to see the empty tiles
		  /*
		    CL_Display::fill_rect(x * tile_size + of_x, y * tile_size + of_y, 
		    x * tile_size + tile_size - 1 + of_x, y * tile_size + tile_size -1 + of_y,
		    1.0, 0.0, 0.0, 1.0);
		  */
		} 
	      else 
		{
		  tile[x][y].surface->put_screen(x * tile_size + of_x, y * tile_size + of_y);
		}
	    }
	}
    } 
  else 
    {
      map_surface->put_screen(int(of_x * s), int(of_y * s), s, s);
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
  if (debug_actions) 
    {
      std::cout << "Bug: Debug actions is no longer supported" << std::endl;
      put_surface(provider, sprovider, x, y);  
    } 
  else 
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
				    x - (ix * tile_size), y - (iy * tile_size));
		  tile[ix][iy].reload();
		}
	    }
	}
    }
}

void
PingusSpotMap::put(CL_SurfaceProvider* sprovider, int x, int y)
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
	  // PNG change
	  put_surface(static_cast<CL_Canvas*>(tile[ix][iy].surface->get_provider()),
		      sprovider,
		      x - (ix * tile_size), y - (iy * tile_size));
	  /*
	  CL_Surface* s = CL_Surface::create(sprovider);
	  s->put_target(x - (ix * tile_size), y - (iy * tile_size), 0, 
			tile[ix][iy].surface->get_provider());*/
	  tile[ix][iy].reload();
	  tile[ix][iy].mark_dirty();
	}
    }
}

// Generates a ColMap (with new) and returns a pointer to it
ColMap*
PingusSpotMap::get_colmap(void)
{
  if (colmap) 
    {
      return colmap;
    } 
  else 
    {
      if (verbose) std::cout << "PingusSpotMap: Starting ColMap creation" << std::endl;
      unsigned char* buffer;
      
      // Allocate the space for the colmap buffer
      // But don't delete it, since the ColMap will do that.
      buffer = new unsigned char[width * height];
      
      if (verbose) std::cout << "PingusSpotMap: Clearing ColMap buffer..." << std::flush;
      for(int i=0; i < width * height; ++i) 
	{
	  buffer[i] = 0;
	}
      
      std::cout << " done" << std::endl;

      // Create a empty ColMap
      colmap = new ColMap(buffer, width, height);
      
      if (verbose) std::cout << "PingusSpotMap: Putting objects to ColMap" << std::endl;
      for(std::vector<surface_data>::iterator i = surfaces.begin(); i != surfaces.end(); i++) 
	{
	  colmap->put(i->surface, i->x_pos, i->y_pos, i->type);
	}
      
      if (verbose)
	cout << "Returning ColMap" << std::endl;
      
      return colmap;
    }
}

CL_Surface*
PingusSpotMap::get_surface(void)
{
  return map_surface;
}

void
PingusSpotMap::create_maptiles(CL_SurfaceProvider* prov)
{
  CL_Surface* temp_surface;

  for(TileIter x=0; x < tile.size(); ++x) {
    for(TileIter y=0; y < tile[x].size(); ++y) {
      // FIXME: Memory leak?!
      temp_surface = CL_SpriteSubarrayProvider::create(prov,
						       x * tile_size,
						       y * tile_size,
						       tile_size,
						       tile_size,
						       1, 1);
      tile[x][y].surface = convert_to_emptyprovider(temp_surface);
      tile[x][y].check_empty();
      // We are not deleting temp_surface, since elsewhere we would get a segfault
    }
  }
}

/* EOF */
