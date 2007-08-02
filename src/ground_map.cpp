//  $Id: spot_map.cxx,v 1.31 2003/10/22 11:11:22 grumbel Exp $
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

#include <stdio.h>
#include <iostream>
#include "display/scene_context.hpp"
#include "pingus_error.hpp"
#include "blitter.hpp"
#include "ground_map.hpp"
#include "pingus_level.hpp"
#include "gettext.h"
#include "col_map.hpp"
#include "math.hpp"
#include "SDL.h"

MapTile::MapTile () 
{
}

MapTile::~MapTile()
{
}

void
MapTile::prepare()
{
  if (!pixelbuffer)
    {
      pixelbuffer = PixelBuffer(tile_size, tile_size);
      //Blitter::clear_canvas(pixelbuffer);
    }
}

void
MapTile::remove(PixelBuffer obj, int x, int y, 
                int real_x, int real_y, GroundMap* parent)
{
  if (sprite)
    {
      parent->put_alpha_surface(pixelbuffer, obj, x, y, real_x, real_y);
      sprite = Sprite(pixelbuffer);
    }
}

void
MapTile::put(PixelBuffer obj, int x, int y)
{
  prepare();
  pixelbuffer.blit(obj, x, y);
  sprite = Sprite(pixelbuffer);
}

GroundMap::GroundMap(const PingusLevel& plf)
{
  width  = plf.get_size().width;
  height = plf.get_size().height;

  colmap = new ColMap(width, height);

  // Checking that the map has the correct size, only multiples of
  // tile_size are allowed, anything else wouldn't fit very well on
  // the colmap
  if ((width % tile_size) != 0)
    {
      width += (tile_size - (width % tile_size));
    }

  if ((height % tile_size) != 0)
    {
      height += (tile_size - (height % tile_size));
    }

  tile_width  = width/tile_size;
  tile_height = height/tile_size;

  // Allocating tile map
  tile.resize(tile_width);
  for(unsigned int i=0; i < tile.size(); ++i)
    tile[i].resize(tile_height);

  // fix the height back to the correct values
  width  = plf.get_size().width;
  height = plf.get_size().height;
}

GroundMap::~GroundMap(void)
{
  delete colmap;
}

void
GroundMap::draw_colmap(SceneContext& gc)
{
  colmap->draw(gc.color());
}

// Draws the map with a offset, needed for scrolling
void
GroundMap::draw(SceneContext& gc)
{
  const Rect& display = gc.color().get_clip_rect();

  // FIXME: delete the next four lines and replace them with gc.get_clip_rect()
  if (draw_collision_map)
    {
      draw_colmap(gc);
    }
  else
    {
      // Trying to calc which parts of the tilemap needs to be drawn
      int start_x = Math::max(0, display.left/tile_size);
      int start_y = Math::max(0, display.top/tile_size);
      int tilemap_width  = display.get_width()  / tile_size + 1;
      int tilemap_height = display.get_height() / tile_size + 1;

      // drawing the stuff
      for (int x = start_x; x <= (start_x + tilemap_width) && x < int(tile.size()); ++x)
        for (int y = start_y; y <= start_y + tilemap_height && y < int(tile[x].size()); ++y)
          {
            if (tile[x][y].get_sprite())
              {
                //std::cout << "Drawing GroundMap Tile " << std::endl;
                gc.color().draw(tile[x][y].get_sprite(),
                                Vector3f((float)x * tile_size, (float)y * tile_size));
              }
            else
              {
                if (0 /*pingus_debug_flags & PINGUS_DEBUG_TILES*/)
                  gc.color().draw_fillrect((float)x * tile_size,
                                           (float)y * tile_size,
                                           (float)x * tile_size + tile_size,
                                           (float)y * tile_size + tile_size,
                                           Color(255, 0, 0, 75));
              }
          }
    }
}

// Returns the width of the map, it is read directly from the *.psm file
int
GroundMap::get_width(void)
{
  return width;
}

// Returns the height of the map, it is read directly from the *.psm file
int
GroundMap::get_height(void)
{
  return height;
}

void
GroundMap::remove(PixelBuffer sprovider, int x, int y)
{
  // Get the start tile and end tile
  int start_x = Math::max(x / tile_size, 0);
  int start_y = Math::max(y / tile_size, 0);
  int end_x   = Math::min((x + sprovider.get_width()) / tile_size,
                          (width - 1) / tile_size);
  int end_y   = Math::min((y + sprovider.get_height()) / tile_size,
                          (height - 1) / tile_size);

  for(int ix = start_x; ix <= end_x; ++ix)
    for(int iy = start_y; iy <= end_y; ++iy)
      {
        tile[ix][iy].remove(sprovider, x - (ix * tile_size),
                            y - (iy * tile_size), x, y, this);
      }
}

void
GroundMap::put_alpha_surface(PixelBuffer provider, PixelBuffer sprovider,
                             int x_pos, int y_pos, int real_x_arg, int real_y_arg)
{
  if (sprovider.get_surface()->format->BitsPerPixel != 8)
    {
      PingusError::raise(std::string("SpotMap::put_alpha_surface: Image has wrong color depth: " 
                                     + sprovider.get_surface()->format->BitsPerPixel));
    }

  provider.lock();
  sprovider.lock();

  int swidth  = sprovider.get_width();
  int twidth  = provider.get_width();

  int spitch = sprovider.get_pitch();
  int tpitch = sprovider.get_pitch();

  int start_x = std::max(0, -x_pos);
  int start_y = std::max(0, -y_pos);

  int end_x = std::min(swidth,  twidth  - x_pos);
  int end_y = std::min(sprovider.get_height(), provider.get_height() - y_pos);

  if (end_x - start_x <= 0 || end_y - start_y <= 0)
    return;

  Uint8* target_buf = static_cast<Uint8*>(provider.get_data());
  Uint8* source_buf = static_cast<Uint8*>(sprovider.get_data());

  if (sprovider.get_surface()->flags & SDL_SRCCOLORKEY)
    {
      Uint32 colorkey = sprovider.get_surface()->format->colorkey;

      for (int y = start_y; y < end_y; ++y)
        {
          Uint8* tptr = target_buf + tpitch*(y+y_pos) + 4*(x_pos + start_x);
          Uint8* sptr = source_buf + spitch*y + start_x;

          for (int x = start_x; x < end_x; ++x)
            { 
              if (*sptr != colorkey && colmap->getpixel(real_x_arg+x, real_y_arg+y) != Groundtype::GP_SOLID)
                {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    *tptr = 0;
#else
                    tptr[3] = 0;
#endif
                }

              tptr += 4;
              sptr += 1;
            }
        }
    }
  else
    {
      for (int y = start_y; y < end_y; ++y)
        {
          Uint8* tptr = target_buf + tpitch*(y+y_pos) + 4*(x_pos + start_x);
          Uint8* sptr = source_buf + spitch*y + start_x;

          for (int x = start_x; x < end_x; ++x)
            { 
              if (colmap->getpixel(real_x_arg+x, real_y_arg+y) != Groundtype::GP_SOLID)
                {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    *tptr = 0;
#else
                    tptr[3] = 0;
#endif
                }
              
              tptr += 4;
              sptr += 1;
            }
        }
    }
  
  sprovider.unlock();
  provider.unlock();
}

void
GroundMap::put(PixelBuffer source, int x, int y)
{
  // Get the start tile and end tile
  int start_x = std::max(0, x / tile_size);
  int start_y = std::max(0, y / tile_size);
  int end_x   = std::min(tile_width,  (x + source.get_width())  / tile_size + 1);
  int end_y   = std::min(tile_height, (y + source.get_height()) / tile_size + 1);

  //std::cout << "GroundMap:put: " << source.get_width() << "x" << source.get_height() << std::endl;

  for(int ix = start_x; ix < end_x; ++ix)
    for(int iy = start_y; iy < end_y; ++iy)
      {
        tile[ix][iy].put(source,
                         x - (ix * tile_size), y - (iy * tile_size));
      }
}

ColMap*
GroundMap::get_colmap(void)
{
  return colmap;
}


/* EOF */
