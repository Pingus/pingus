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

#if 0
void
Blitter::put_surface(PixelBuffer canvas, PixelBuffer provider,
		     int x, int y)
{
  switch(provider.get_format().get_depth())
    {
    case  8:
      put_surface_8bit(canvas, provider, x, y);
      break;
    case 32:
      put_surface_32bit(canvas, provider, x, y);
      break;
    default:
      PingusError::raise("Blitter:put_surface:Unknown color depth: " + StringUtil::to_string(provider.get_format().get_depth()));
      break;
    }
}

void
Blitter::put_surface_8bit(PixelBuffer target, PixelBuffer source,
                          int x_pos, int y_pos)
{
  //std::cout << "8bit blit" << std::endl;

  assert(target.get_format().get_depth() == 32);

  target.lock();
  source.lock();

  int swidth  = source.get_width();
  int twidth  = target.get_width();

  int start_x = std::max(0, -x_pos);
  int start_y = std::max(0, -y_pos);

  int end_x = std::min(swidth,  twidth  - x_pos);
  int end_y = std::min(source.get_height(), target.get_height() - y_pos);

  if (end_x - start_x <= 0 || end_y - start_y <= 0)
    return;

  cl_uint8* target_buf = static_cast<cl_uint8*>(target.get_data());
  cl_uint8* source_buf = static_cast<cl_uint8*>(source.get_data());

  CL_Palette palette = source.get_palette();

  if (source.get_format().has_colorkey())
    {
      unsigned int colorkey = source.get_format().get_colorkey();

      for (int y = start_y; y < end_y; ++y)
        {
          cl_uint8* tptr = target_buf + 4*((twidth*(y+y_pos)) + x_pos + start_x);
          cl_uint8* sptr = source_buf + swidth*y + start_x;

          for (int x = start_x; x < end_x; ++x)
            { 
              if (*sptr != colorkey)
                {
				  if (!CL_Endian::is_system_big())
				  {
                    tptr[0] = 255;
                    tptr[1] = palette.colors[*sptr].get_blue();
                    tptr[2] = palette.colors[*sptr].get_green();
                    tptr[3] = palette.colors[*sptr].get_red();
				  }
				  else
				  {
                    tptr[3] = 255;
                    tptr[2] = palette.colors[*sptr].get_blue();
                    tptr[1] = palette.colors[*sptr].get_green();
                    tptr[0] = palette.colors[*sptr].get_red();
				  }
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
          cl_uint8* tptr = target_buf + 4*((twidth*(y+y_pos)) + x_pos + start_x);
          cl_uint8* sptr = source_buf + swidth*y + start_x;

          for (int x = start_x; x < end_x; ++x)
            {
              if (!CL_Endian::is_system_big())
              {
                tptr[0] = 255;
                tptr[1] = palette.colors[*sptr].get_blue();
                tptr[2] = palette.colors[*sptr].get_green();
                tptr[3] = palette.colors[*sptr].get_red();
              }
              else
              {
                tptr[3] = 255;
                tptr[2] = palette.colors[*sptr].get_blue();
                tptr[1] = palette.colors[*sptr].get_green();
                tptr[0] = palette.colors[*sptr].get_red();
              }

              tptr += 4;
              sptr += 1;
            }
        }
    }
  
  source.unlock();
  target.unlock();
}

void
Blitter::put_surface_32bit(PixelBuffer target, PixelBuffer source,
			   const int x_pos, const int y_pos)
{
  //std::cout << "32bit blit" << std::endl;

  target.lock();
  source.lock();

  int swidth  = source.get_width();
  int sheight = source.get_height();

  int twidth  = target.get_width();
  int theight = target.get_height();

  int start_x = std::max(0, -x_pos);
  int start_y = std::max(0, -y_pos);

  int end_x = std::min(swidth,  twidth  - x_pos);
  int end_y = std::min(sheight, theight - y_pos);

  if (end_x - start_x <= 0
      || end_y - start_y <= 0)
    return;

  /* Benchmarks: 
   * ===========
   * 6msec with memcpy
   * 10msec with uint32
   * 17msec with uint8
   */
      
  cl_uint8* target_buf = static_cast<cl_uint8*>(target.get_data());
  cl_uint8* source_buf = static_cast<cl_uint8*>(source.get_data());

  for (int y = start_y; y < end_y; ++y)
    {
      int tidx = 4*(twidth * (y_pos + y) + x_pos);
      int sidx = 4*(swidth * y);
      
			/*
      if (0)
        { // Fast but doesn't handle masks
          memcpy(target_buf + tidx + 4*start_x, source_buf + sidx + 4*start_x, 
                 sizeof(cl_uint32)*(end_x - start_x));
        }
      else if (0)
        { // doesn't handle masks either, but looks half correct
          cl_uint8* tptr = target_buf + tidx + 4*start_x;
          cl_uint8* sptr = source_buf + sidx + 4*start_x;

          for (int x = start_x; x < end_x; ++x)
            {
              if (!CL_Endian::is_system_big())
              {
                tptr[0] = sptr[3];
                tptr[1] = sptr[0];
                tptr[2] = sptr[1];
                tptr[3] = sptr[2];
              }
              else
              {
                tptr[3] = sptr[3];
                tptr[2] = sptr[0];
                tptr[1] = sptr[1];
                tptr[0] = sptr[2];
              }

              tptr += 4;
              sptr += 4;
            }
        }
      else */
        {
          // doesn't handle masks either, but looks half correct
          cl_uint8* tptr = target_buf + tidx + 4*start_x;
          cl_uint8* sptr = source_buf + sidx + 4*start_x;

          for (int x = start_x; x < end_x; ++x)
            {
              if (!CL_Endian::is_system_big())
              {
								float a = sptr[0]/255.0f;
                tptr[0] = Math::mid(0, int((1.0f - a) * tptr[0] + a * sptr[0]), 255);
                tptr[1] = Math::mid(0, int((1.0f - a) * tptr[1] + a * sptr[1]), 255);
                tptr[2] = Math::mid(0, int((1.0f - a) * tptr[2] + a * sptr[2]), 255);
                tptr[3] = Math::mid(0, int((1.0f - a) * tptr[3] + a * sptr[3]), 255);
              }
              else
              {
                float a = sptr[3]/255.0f;
                tptr[3] = Math::mid(0, int((1.0f - a) * tptr[3] + a * sptr[3]), 255);
                tptr[2] = Math::mid(0, int((1.0f - a) * tptr[2] + a * sptr[0]), 255);
                tptr[1] = Math::mid(0, int((1.0f - a) * tptr[1] + a * sptr[1]), 255);
                tptr[0] = Math::mid(0, int((1.0f - a) * tptr[0] + a * sptr[2]), 255);
              }

              tptr += 4;
              sptr += 4;
            }
        }
    }
  
  source.unlock();
  target.unlock();
}

void
Blitter::fill_rect(PixelBuffer target, const CL_Rect& rect, const Color& color)
{
  if (target.get_format().get_depth() != 32
      && target.get_format().get_depth() != 24)
    {
      std::cout << "Blitter::fill_rect: depth must be 32 but is " << target.get_format().get_depth() << std::endl;
      return;
    }

  target.lock();
  
  int twidth  = target.get_width();
  int swidth  = rect.get_width();

  int start_x = std::max(0, -rect.left);
  int start_y = std::max(0, -rect.top);

  int end_x = std::min(swidth,  twidth  - rect.left);
  int end_y = std::min(rect.get_height(), target.get_height() - rect.top);

  if (end_x - start_x <= 0 || end_y - start_y <= 0)
    return;

  cl_uint8* target_buf = static_cast<cl_uint8*>(target.get_data());

  if (target.get_format().get_depth() == 24)
    {
      if (color.get_alpha() == 255)
        {
          for (int y = start_y; y < end_y; ++y)
            {
              cl_uint8* tptr = target_buf + 3*((twidth*(y + rect.top)) + rect.left + start_x);

              for (int x = start_x; x < end_x; ++x)
                { 
                  if (!CL_Endian::is_system_big())
                  {
                    tptr[0] = color.get_red();
                    tptr[1] = color.get_green();
                    tptr[2] = color.get_blue();
                  }
                  else
                  {
                    tptr[2] = color.get_red();
                    tptr[1] = color.get_green();
                    tptr[0] = color.get_blue();
                  }
                  tptr += 3;
                }
            }
        }
      else
        {
          float a = color.get_alpha()/255.0f;

          for (int y = start_y; y < end_y; ++y)
            {
              cl_uint8* tptr = target_buf + 3*((twidth*(y + rect.top)) + rect.left + start_x);

              for (int x = start_x; x < end_x; ++x)
                { 
                  if (!CL_Endian::is_system_big())
                  {
                    tptr[0] = Math::mid(0, int(((1.0f - a) * (tptr[0])) + a * color.get_blue()) , 255); //blue
                    tptr[1] = Math::mid(0, int(((1.0f - a) * (tptr[1])) + a * color.get_green()), 255); //green
                    tptr[2] = Math::mid(0, int(((1.0f - a) * (tptr[2])) + a * color.get_red()), 255); //red
                  }
                  else
                  {
                    tptr[2] = Math::mid(0, int(((1.0f - a) * (tptr[2])) + a * color.get_blue()) , 255); //blue
                    tptr[1] = Math::mid(0, int(((1.0f - a) * (tptr[1])) + a * color.get_green()), 255); //green
                    tptr[0] = Math::mid(0, int(((1.0f - a) * (tptr[0])) + a * color.get_red()), 255); //red
                  }

                  tptr += 3;
                }
            }
        }
    }
  else if (target.get_format().get_depth() == 32)
    {
      if (color.get_alpha() == 255)
        {
          for (int y = start_y; y < end_y; ++y)
            {
              cl_uint8* tptr = target_buf + 4*((twidth*(y + rect.top)) + rect.left + start_x);

              for (int x = start_x; x < end_x; ++x)
                { 
                  if (!CL_Endian::is_system_big())
                  {
                    tptr[0] = 255;
                    tptr[1] = color.get_blue();
                    tptr[2] = color.get_green();
                    tptr[3] = color.get_red();
                  }
                  else
                  {
                    tptr[3] = 255;
                    tptr[2] = color.get_blue();
                    tptr[1] = color.get_green();
                    tptr[0] = color.get_red();
                  }
                  tptr += 4;
                }
            }
        }
      else
        {
          for (int y = start_y; y < end_y; ++y)
            {
              cl_uint8* tptr = target_buf + 4*((twidth*(y + rect.top)) + rect.left + start_x);

              for (int x = start_x; x < end_x; ++x)
                { 
                  float a = color.get_alpha()/255.0f;

                  if (!CL_Endian::is_system_big())
                  {
                    tptr[0] = Math::mid(0, int(tptr[0] + a * color.get_alpha()), 255);
                    tptr[1] = Math::mid(0, int((1.0f - a) * tptr[1] + a * color.get_blue()) , 255);
                    tptr[2] = Math::mid(0, int((1.0f - a) * tptr[2] + a * color.get_green()), 255);
                    tptr[3] = Math::mid(0, int((1.0f - a) * tptr[3] + a * color.get_red())  , 255);
                  }
                  else
                  {
                    tptr[3] = Math::mid(0, int(tptr[3] + a * color.get_alpha()), 255);
                    tptr[2] = Math::mid(0, int((1.0f - a) * tptr[2] + a * color.get_blue()) , 255);
                    tptr[1] = Math::mid(0, int((1.0f - a) * tptr[1] + a * color.get_green()), 255);
                    tptr[0] = Math::mid(0, int((1.0f - a) * tptr[0] + a * color.get_red())  , 255);
                  }
                }
            }
        }
    }
  
  target.unlock();
}

void
Blitter::clear_canvas(PixelBuffer canvas, Color color)
{
  unsigned char* buffer;

  canvas.lock();
  buffer = static_cast<unsigned char*>(canvas.get_data());
  memset(buffer, color.color, sizeof(unsigned char) * canvas.get_pitch() * canvas.get_height());
  canvas.unlock();
}

PixelBuffer
Blitter::create_canvas(PixelBuffer prov)
{
  PixelBuffer canvas(prov.get_width(), prov.get_height(), prov.get_width()*4, CL_PixelFormat::rgba8888);

  switch (prov.get_format().get_depth())
    {
      // RGB888
    case 24:
      {
	canvas.lock();
	prov.lock();

	int buffer_size = prov.get_pitch () * prov.get_height ();
	unsigned char* sbuffer = static_cast<unsigned char*>(prov.get_data ());
	unsigned char* tbuffer = static_cast<unsigned char*>(canvas.get_data ());

	for (int si = 0, ti = 0; si < buffer_size; si += 3, ti += 4)
	  {
	    if (!CL_Endian::is_system_big())
        {
          tbuffer[ti + 0] = 255; // Alpha
          tbuffer[ti + 1] = sbuffer[si + 0];
          tbuffer[ti + 2] = sbuffer[si + 1];
          tbuffer[ti + 3] = sbuffer[si + 2];
        }
        else
        {
          tbuffer[ti + 3] = 255; // Alpha
          tbuffer[ti + 2] = sbuffer[si + 0];
          tbuffer[ti + 1] = sbuffer[si + 1];
          tbuffer[ti + 0] = sbuffer[si + 2];
        }
	  }

	// -FIXME: memory hole
	prov.unlock();
	canvas.unlock();
      }
      break;

      // RGBA8888
    case 32:
      canvas.lock();
      prov.lock();
      memcpy(canvas.get_data(), prov.get_data(),
 	     sizeof(unsigned char) * prov.get_height() * prov.get_pitch());
      prov.unlock();
      canvas.unlock();
      break;

    default:
      put_surface(canvas, prov, 0, 0);
      break;
    }
  return canvas;
}
#endif

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
