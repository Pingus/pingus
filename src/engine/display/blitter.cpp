// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/display/blitter.hpp"

#include "engine/display/blitter_impl.hpp"

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
    SDL_Palette* pal = SDL_AllocPalette(256);

    Uint32 ckey;
    int useckey = 0;

    useckey = SDL_GetColorKey(surface, &ckey) == 0;

    new_surface = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);

    SDL_LockSurface(surface);
    SDL_LockSurface(new_surface);

    pixels     = static_cast<unsigned char*>(surface->pixels);
    new_pixels = static_cast<unsigned char*>(new_surface->pixels);
    new_pitch  = new_surface->pitch;

    memcpy(pal->colors, surface->format->palette->colors, sizeof(SDL_Color) * 256);

    for (i = 0; i < height; ++i) {
      x = i * new_pitch;
      for (j = 0; j < width; ++j) {
        new_pixels[x] = pixels[(i * surface->h / height) * surface->pitch + j * surface->w / width];
        ++x;
      }
    }

    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(new_surface);

    SDL_SetSurfacePalette(new_surface, pal);
    if (useckey)
    {
      SDL_SetColorKey(new_surface, SDL_TRUE, ckey);
      SDL_SetSurfaceRLE(new_surface, SDL_TRUE);
    }
  } else {
    int ix, iy;
    float fx, fy, fz;
    unsigned char *p1, *p2, *p3, *p4;

    new_surface = SDL_CreateRGBSurface(0, width, height, surface->format->BitsPerPixel,
                                       surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

    SDL_LockSurface(surface);
    SDL_LockSurface(new_surface);

    pixels     = static_cast<unsigned char*>(surface->pixels);
    new_pixels = static_cast<unsigned char*>(new_surface->pixels);
    new_pitch = new_surface->pitch;

    for (i = 0; i < height; ++i) {
      x = i * new_pitch;
      fy = static_cast<float>(i) * static_cast<float>(surface->h) / static_cast<float>(height);
      iy = static_cast<int>(fy);
      fy -= static_cast<float>(iy);
      for (j = 0; j < width; ++j) {
        fx = static_cast<float>(j) * static_cast<float>(surface->w) / static_cast<float>(width);
        ix = static_cast<int>(fx);
        fx -= static_cast<float>(ix);
        fz = (fx + fy) / 2;

        p1 = &pixels[iy * surface->pitch + ix * bpp];
        p2 = (iy != surface->h - 1) ?
          &pixels[(iy + 1) * surface->pitch + ix * bpp] : p1;
        p3 = (ix != surface->w - 1) ?
          &pixels[iy * surface->pitch + (ix + 1) * bpp] : p1;
        p4 = (iy != surface->h - 1 && ix != surface->w - 1) ?
          &pixels[(iy + 1) * surface->pitch + (ix + 1) * bpp] : p1;

        new_pixels[x + 0] = static_cast<unsigned char>(
          (static_cast<float>(p1[0]) * (1 - fy) + static_cast<float>(p2[0]) * fy +
           static_cast<float>(p1[0]) * (1 - fx) + static_cast<float>(p3[0]) * fx +
           static_cast<float>(p1[0]) * (1 - fz) + static_cast<float>(p4[0]) * fz) / 3.0 + .5);
        new_pixels[x + 1] = static_cast<unsigned char>(
          (static_cast<float>(p1[1]) * (1 - fy) + static_cast<float>(p2[1]) * fy +
           static_cast<float>(p1[1]) * (1 - fx) + static_cast<float>(p3[1]) * fx +
           static_cast<float>(p1[1]) * (1 - fz) + static_cast<float>(p4[1]) * fz) / 3.0 + .5);
        new_pixels[x + 2] = static_cast<unsigned char>(
          (static_cast<float>(p1[2]) * (1 - fy) + static_cast<float>(p2[2]) * fy +
           static_cast<float>(p1[2]) * (1 - fx) + static_cast<float>(p3[2]) * fx +
           static_cast<float>(p1[2]) * (1 - fz) + static_cast<float>(p4[2]) * fz) / 3.0 + .5);
        if (bpp == 4) {
          new_pixels[x + 3] = static_cast<unsigned char>(
            (static_cast<float>(p1[3]) * (1 - fy) + static_cast<float>(p2[3]) * fy +
             static_cast<float>(p1[3]) * (1 - fx) + static_cast<float>(p3[3]) * fx +
             static_cast<float>(p1[3]) * (1 - fz) + static_cast<float>(p4[3]) * fz) / 3.0 + .5);
        }
        x += bpp;
      }
    }

    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(new_surface);
  }

  return new_surface;
}

/** Flip a surface horizontal */
Surface
Blitter::flip_horizontal (Surface prov)
{
  return BlitterImpl::modify<BlitterImpl::transform_flip>(prov);
}

/** Flip a surface vertical */
Surface
Blitter::flip_vertical (Surface sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot180_flip>(sur);
}

/** Rotate a surface 90 degrees */
Surface
Blitter::rotate_90 (Surface sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot90>(sur);
}

Surface
Blitter::rotate_180 (Surface sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot180>(sur);
}

Surface
Blitter::rotate_270 (Surface sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot270>(sur);
}

Surface
Blitter::rotate_90_flip (Surface sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot90_flip>(sur);
}

Surface
Blitter::rotate_180_flip (Surface sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot180_flip>(sur);
}

Surface
Blitter::rotate_270_flip (Surface sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot270_flip>(sur);
}

SDL_Surface*
Blitter::create_surface_rgba(int w, int h)
{
  return SDL_CreateRGBSurface(0, w, h, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                              0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
                              0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
    );
}

SDL_Surface*
Blitter::create_surface_rgb(int w, int h)
{
  return SDL_CreateRGBSurface(0, w, h, 24,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                              0xff000000, 0x00ff0000, 0x0000ff00, 0x00000000
#else
                              0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000
#endif
    );
}

SDL_Surface*
Blitter::create_surface_from_format(SDL_Surface* surface, int w, int h)
{
  SDL_Surface* new_surface = SDL_CreateRGBSurface(0, w, h,
                                                  surface->format->BitsPerPixel,
                                                  surface->format->Rmask,
                                                  surface->format->Gmask,
                                                  surface->format->Bmask,
                                                  surface->format->Amask);

  Uint8 alpha;
  if (SDL_GetSurfaceAlphaMod(surface, &alpha) == 0)
  {
    SDL_SetSurfaceAlphaMod(new_surface, alpha);
  }
  else
  {
    log_error("SDL_GetSurfaceAlphaMod failed: %1%", SDL_GetError());
  }

  SDL_BlendMode blend_mode;
  if (SDL_GetSurfaceBlendMode(surface, &blend_mode) == 0)
  {
    SDL_SetSurfaceBlendMode(new_surface, blend_mode);
  }
  else
  {
    log_error("SDL_GetSurfaceBlendMode failed: %1%", SDL_GetError());
  }

  Uint8 r, g, b;
  if (SDL_GetSurfaceColorMod(surface, &r, &g, &b) == 0)
  {
    SDL_SetSurfaceColorMod(new_surface, r, g, b);
  }
  else
  {
    log_error("SDL_GetSurfaceColorMod failed: %1%", SDL_GetError());
  }

  if (surface->format->palette)
  {
    if (surface->format->palette->ncolors <= new_surface->format->palette->ncolors)
    {
      // An SDL_Surface loaded from file can have a palette with very
      // few colors, SDL_SetSurfacePalette() however expects the
      // number of colors to match the bits per pixel:
      //
      // if (palette && palette->ncolors != (1 << format->BitsPerPixel)) {
      //   return SDL_SetError("SDL_SetPixelFormatPalette() passed a palette that doesn't match the format");
      // }
      //
      // As a result, we are not using SDL_SetSurfacePalette() here,
      // but a manual memcpy().
      memcpy(new_surface->format->palette->colors,
             surface->format->palette->colors,
             static_cast<size_t>(surface->format->palette->ncolors) * sizeof(SDL_Color));
    }
    else
    {
      log_error("target palette smaller then source palette: src:%1% > tgt:%2%",
                surface->format->palette->ncolors,
                new_surface->format->palette->ncolors);
    }
  }

  Uint32 colorkey;
  int ret = SDL_GetColorKey(surface, &colorkey);
  if (ret == -1)
  {
    // no colorkey
  }
  else if (ret < 0)
  {
    log_error("SDL_GetColorKey failed: %1%", SDL_GetError());
  }
  else
  {
    SDL_SetColorKey(new_surface, SDL_TRUE, colorkey);
  }

  return new_surface;
}

/* EOF */
