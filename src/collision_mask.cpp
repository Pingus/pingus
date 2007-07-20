/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <iostream>
#include "resource.hpp"
#include "system.hpp"
#include "collision_mask.hpp"

CollisionMask::CollisionMask()
  : buffer(0)
{

}

CollisionMask::CollisionMask(const std::string& name)
  : buffer(0)
{
  //std::cout << "CollisionMask: " << name << std::endl;
  pixelbuffer = Resource::load_pixelbuffer(name);
  //PixelBuffer cmap = pixelbuffer; // Resource::load_pixelbuffer(System::cut_ext(name) + "_cmap");

  width  = pixelbuffer.get_width();
  height = pixelbuffer.get_height();
  
  buffer = new uint8_t[width * height];

  SDL_Surface* surface = pixelbuffer.get_surface();
  SDL_LockSurface(surface);

  if (surface->format->BytesPerPixel == 1)
    {
      uint8_t* source = static_cast<uint8_t*>(surface->pixels);
      for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x)
          {
            if (source[y * width + x] == surface->format->colorkey)
              buffer[y * width + x] = 0;
            else
              buffer[y * width + x] = 1;
          }
    }
  else
    {
      std::cout << "CollisionMask: unsupported image format: " 
                << surface->format->BytesPerPixel << std::endl;     
    }

  SDL_UnlockSurface(surface);
}

CollisionMask::~CollisionMask()
{
  delete[] buffer;
}  

int
CollisionMask::get_width() const
{
  return width;
}

int
CollisionMask::get_height() const
{
  return height;
}

int
CollisionMask::get_pitch() const
{
  return pixelbuffer.get_surface()->pitch;
}

PixelBuffer
CollisionMask::get_pixelbuffer() const
{
  return pixelbuffer;
}

uint8_t*
CollisionMask::get_data() const
{
  return buffer;
}

/* EOF */
