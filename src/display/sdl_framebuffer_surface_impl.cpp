//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdexcept>
#include "display/sdl_framebuffer_surface_impl.hpp"

SDLFramebufferSurfaceImpl::SDLFramebufferSurfaceImpl(SDL_Surface* src)
{
  if (src->format->Amask != 0 || (src->flags & SDL_SRCCOLORKEY))
    surface = SDL_DisplayFormatAlpha(src);
  else
    surface = SDL_DisplayFormat(src);
}

SDLFramebufferSurfaceImpl::~SDLFramebufferSurfaceImpl()
{
  SDL_FreeSurface(surface);
}

Surface
SDLFramebufferSurfaceImpl::to_surface() const
{
  //  Convert the surface to a format usable for the Blitter code
  SDL_Surface* convert; 
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN) 
    {
      convert = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                     surface->w, surface->h, 32,
                                     0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    }
  else
    {
      convert = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                     surface->w, surface->h, 32,
                                     0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    }

  SDL_SetAlpha(surface, 0, 0); // FIXME: Do we have to reset this?
  SDL_BlitSurface(surface, 0, convert, 0);

  return Surface(surface);
}

/* EOF */
