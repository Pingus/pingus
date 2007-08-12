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

#include "SDL_image.h"
#include <sstream>
#include <iostream>
#include "pixel_buffer.hpp"

class PixelBufferImpl
{
public:
  PixelBufferImpl(SDL_Surface* surface = NULL) : surface(surface) {}
  ~PixelBufferImpl() {
    SDL_FreeSurface(surface);
  }
  SDL_Surface* surface;
};

PixelBuffer::PixelBuffer()
{
}

PixelBuffer::PixelBuffer(const std::string& name_)
  : impl(new PixelBufferImpl())
{
  impl->surface = IMG_Load(name_.c_str());
  if (!impl->surface)
    std::cout << "XXXXXX Failed to load: " << name_ << std::endl;
  ///else
  //std::cout << "Loaded pixelbuffer: " << name << ": " << surface->w << "x" << surface->h << std::endl;

}

PixelBuffer::PixelBuffer(int width, int height, SDL_Palette* palette, int colorkey)
  : impl(new PixelBufferImpl())
{
  if (colorkey == -1)
    {
      impl->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 8,
                                           0, 0, 0 ,0);
    }
  else
    {
      impl->surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, width, height, 8,
                                           0, 0, 0 ,0);
      SDL_SetColorKey(impl->surface, SDL_SRCCOLORKEY, colorkey);
    }

  SDL_SetColors(impl->surface, palette->colors, 0, palette->ncolors);
}

PixelBuffer::PixelBuffer(int width, int height)
  : impl(new PixelBufferImpl())
{
  impl->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
                                       0x000000ff,
                                       0x0000ff00,
                                       0x00ff0000,
                                       0xff000000);
  //SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
}

PixelBuffer::PixelBuffer(SDL_Surface* surface)
  : impl(new PixelBufferImpl(surface))
{
}

PixelBuffer::~PixelBuffer()
{
}

void
PixelBuffer::blit(const PixelBuffer& src, int x, int y)
{
  if (!get_surface())
    {
      std::cout << "PixelBuffer: Trying to blit to empty surface" << std::endl;
    }
  else if (!src.get_surface())
    {
      std::cout << "PixelBuffer: Trying to blit with an empty surface" << std::endl;
    }
  else
    {
      SDL_Rect dstrect;

      dstrect.x = x;
      dstrect.y = y;

      SDL_BlitSurface(src.get_surface(), NULL, get_surface(), &dstrect);
    }
}

void
PixelBuffer::lock()
{
  SDL_LockSurface(get_surface());
}

void
PixelBuffer::unlock()
{
  SDL_UnlockSurface(get_surface());
}

uint8_t*
PixelBuffer::get_data() const
{
  return static_cast<uint8_t*>(get_surface()->pixels);
}

int
PixelBuffer::get_width()  const
{
  if (get_surface())
    return get_surface()->w;
  else
    return 0;
}

int
PixelBuffer::get_height() const
{
  if (get_surface())
    return get_surface()->h;
  else
    return 0;
}

int
PixelBuffer::get_pitch() const
{
  if (get_surface())
    return get_surface()->pitch;
  else
    return 0;
}

SDL_Surface* 
PixelBuffer::get_surface() const
{
  return impl ? impl->surface : 0;
}

PixelBuffer::operator bool() const
{
  return impl ? impl->surface != NULL : false;
}

Color
PixelBuffer::get_pixel(int x, int y) const
{
  Uint8 *p = (Uint8 *)get_surface()->pixels + y * get_surface()->pitch + x * get_surface()->format->BytesPerPixel;
  Uint32 pixel;

  switch(get_surface()->format->BytesPerPixel)
    {
    case 1:
      pixel = *p;
    case 2: /* This will cause some problems ... */
      pixel = *(Uint16 *)p;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	pixel = p[0] << 16 | p[1] << 8 | p[2];
      else
	pixel = p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      pixel = *(Uint32 *)p;
    default:
      pixel = 0;       /* shouldn't happen, but avoids warnings */
    } 

  Color color;
  SDL_GetRGBA(pixel, get_surface()->format, &color.r, &color.g, &color.b, &color.a);
  return color;
}

/* EOF */
