//  $Id: blitter.cxx,v 1.34 2003/10/21 21:37:05 grumbel Exp $
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

#include <config.h>
#include <stdio.h>
#include <assert.h>

#include "SDL.h"
#include "string_util.hpp"
#include "pingus_error.hpp"
#include "globals.hpp"
#include "math.hpp"
#include "blitter.hpp"
#include "debug.hpp"
#include "blitter_impl.hpp"

/* Headers needed for i18n / gettext */
#include "gettext.h"

SDL_Surface*
Blitter::scale_surface(SDL_Surface* surface, int width, int height)
{
  int i;
  int j;
  unsigned char *pixels;
  unsigned char *new_pixels;
  int x;
  int bpp;
  int new_pitch;
  SDL_Surface* new_surface;

  bpp = surface->format->BytesPerPixel;
  if (bpp == 1) {
    SDL_Color pal[256];
    Uint32 ckey;
    int useckey;

    useckey = surface->flags & SDL_SRCCOLORKEY;
    new_surface = SDL_CreateRGBSurface(SDL_SWSURFACE | (useckey ? SDL_SRCCOLORKEY : 0), width, height, 8, 0, 0, 0, 0);

    SDL_LockSurface(surface);
    SDL_LockSurface(new_surface);

    pixels = (unsigned char *)surface->pixels;
    new_pixels = (unsigned char *)new_surface->pixels;
    new_pitch = new_surface->pitch;

    memcpy(pal, surface->format->palette->colors, sizeof(SDL_Color) * 256);
    ckey = surface->format->colorkey;

    for (i = 0; i < height; ++i) {
      x = i * new_pitch;
      for (j = 0; j < width; ++j) {
        new_pixels[x] = pixels[(i * surface->h / height) * surface->pitch + j * surface->w / width];
        ++x;
      }
    }

    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(new_surface);

    SDL_SetPalette(new_surface, SDL_LOGPAL | SDL_PHYSPAL, pal, 0, 256);
    if (useckey) {
      SDL_SetColorKey(new_surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, ckey);
    }
  } else {
    int ix, iy;
    float fx, fy, fz;
    unsigned char *p1, *p2, *p3, *p4;

    int Rmask, Gmask, Bmask, Amask;
    if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
      Rmask = 0x000000FF;
      Gmask = 0x0000FF00;
      Bmask = 0x00FF0000;
      Amask = (bpp == 4) ? 0xFF000000 : 0;
    } else {
      int s = (bpp == 4) ? 0 : 8;
      Rmask = 0xFF000000 >> s;
      Gmask = 0x00FF0000 >> s;
      Bmask = 0x0000FF00 >> s;
      Amask = 0x000000FF >> s;
    }

    new_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 8 * bpp,
      Rmask, Gmask, Bmask, Amask);

    SDL_LockSurface(surface);
    SDL_LockSurface(new_surface);

    pixels = (unsigned char *)surface->pixels;
    new_pixels = (unsigned char *)new_surface->pixels;
    new_pitch = new_surface->pitch;

    for (i = 0; i < height; ++i) {
      x = i * new_pitch;
      fy = (float)i * surface->h / height;
      iy = (int)fy;
      fy -= iy;
      for (j = 0; j < width; ++j) {
        fx = (float)j * surface->w / width;
        ix = (int)fx;
        fx -= ix;
        fz = (fx + fy) / 2;

        p1 = &pixels[iy * surface->pitch + ix * bpp];
        p2 = (iy != surface->h - 1) ?
          &pixels[(iy + 1) * surface->pitch + ix * bpp] : p1;
        p3 = (ix != surface->w - 1) ?
          &pixels[iy * surface->pitch + (ix + 1) * bpp] : p1;
        p4 = (iy != surface->h - 1 && ix != surface->w - 1) ?
          &pixels[(iy + 1) * surface->pitch + (ix + 1) * bpp] : p1;

        new_pixels[x + 0] = static_cast<unsigned char>(
          (p1[0] * (1 - fy) + p2[0] * fy +
           p1[0] * (1 - fx) + p3[0] * fx +
           p1[0] * (1 - fz) + p4[0] * fz) / 3.0 + .5);
        new_pixels[x + 1] = static_cast<unsigned char>(
          (p1[1] * (1 - fy) + p2[1] * fy +
           p1[1] * (1 - fx) + p3[1] * fx +
           p1[1] * (1 - fz) + p4[1] * fz) / 3.0 + .5);
        new_pixels[x + 2] = static_cast<unsigned char>(
          (p1[2] * (1 - fy) + p2[2] * fy +
           p1[2] * (1 - fx) + p3[2] * fx +
           p1[2] * (1 - fz) + p4[2] * fz) / 3.0 + .5);
        if (bpp == 4) {
          new_pixels[x + 3] = static_cast<unsigned char>(
            (p1[3] * (1 - fy) + p2[3] * fy +
             p1[3] * (1 - fx) + p3[3] * fx +
             p1[3] * (1 - fz) + p4[3] * fz) / 3.0 + .5);
        }
        x += bpp;
      }
    }

    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(new_surface);
  }

  return new_surface;
}

PixelBuffer
Blitter::scale_surface_to_canvas(PixelBuffer provider, int width, int height)
{
    return PixelBuffer(Blitter::scale_surface(provider.get_surface(), width, height));
}

/** Flip a surface horizontal */
PixelBuffer
Blitter::flip_horizontal (PixelBuffer prov)
{
  return BlitterImpl::modify(prov, BlitterImpl::transform_flip());
}

/** Flip a surface vertical */
PixelBuffer
Blitter::flip_vertical (PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180_flip());
}

/** Rotate a surface 90 degrees */
PixelBuffer
Blitter::rotate_90 (PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot90());
}


PixelBuffer
Blitter::rotate_180 (PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180());
}

PixelBuffer
Blitter::rotate_270 (PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot270());
}

PixelBuffer
Blitter::rotate_90_flip (PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot90_flip());
}

PixelBuffer
Blitter::rotate_180_flip (PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180_flip());
}

PixelBuffer
Blitter::rotate_270_flip (PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot270_flip());
}

/* EOF */
