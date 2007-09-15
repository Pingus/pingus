//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include "SDL_image.h"
#include <sstream>
#include <iostream>
#include "math/rect.hpp"
#include "debug.hpp"
#include "blitter.hpp"
#include "surface.hpp"

class SurfaceImpl
{
public:
  SDL_Surface* surface;
  bool  delete_surface;
  bool       optimized;

  SurfaceImpl(SDL_Surface* surface = NULL, bool delete_surface_ = true) 
    : surface(surface),
      delete_surface(delete_surface_),
      optimized(false)
  {}
  
  ~SurfaceImpl() 
  {
    if (delete_surface)
      SDL_FreeSurface(surface);
  }
};

Surface::Surface()
{
}

Surface::Surface(boost::shared_ptr<SurfaceImpl> impl_)
  : impl(impl_)
{
}

Surface::Surface(const Pathname& pathname)
{
  SDL_Surface* surface = IMG_Load(pathname.get_sys_path().c_str());
  if (surface)
    {
      impl = boost::shared_ptr<SurfaceImpl>(new SurfaceImpl());
      impl->surface = surface;
    }
}

Surface::Surface(int width, int height, SDL_Palette* palette, int colorkey)
  : impl(new SurfaceImpl())
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

Surface::Surface(int width, int height)
  : impl(new SurfaceImpl())
{
  impl->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
                                       0x000000ff,
                                       0x0000ff00,
                                       0x00ff0000,
                                       0xff000000);
  //SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
}

Surface::Surface(SDL_Surface* surface, bool delete_surface)
  : impl(new SurfaceImpl(surface, delete_surface))
{
}

Surface::~Surface()
{
}

void
Surface::blit(const Surface& src, int x, int y)
{
  if (!get_surface())
    {
      std::cout << "Surface: Trying to blit to empty surface" << std::endl;
    }
  else if (!src.get_surface())
    {
      std::cout << "Surface: Trying to blit with an empty surface" << std::endl;
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
Surface::lock()
{
  SDL_LockSurface(get_surface());
}

void
Surface::unlock()
{
  SDL_UnlockSurface(get_surface());
}

uint8_t*
Surface::get_data() const
{
  return static_cast<uint8_t*>(get_surface()->pixels);
}

Size
Surface::get_size()  const
{
  if (get_surface())
    return Size(impl->surface->w, impl->surface->h);
  else
    return Size();
}

int
Surface::get_width()  const
{
  if (get_surface())
    return get_surface()->w;
  else
    return 0;
}

int
Surface::get_height() const
{
  if (get_surface())
    return get_surface()->h;
  else
    return 0;
}

int
Surface::get_pitch() const
{
  if (get_surface())
    return get_surface()->pitch;
  else
    return 0;
}

SDL_Surface* 
Surface::get_surface() const
{
  return impl ? impl->surface : 0;
}

Surface::operator bool() const
{
  return impl ? impl->surface != NULL : false;
}

Color
Surface::get_pixel(int x, int y) const
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

Surface
Surface::mod(ResourceModifierNS::ResourceModifier modifier)
{
  switch(modifier)
    {
      case ResourceModifierNS::ROT0:
        return this->clone();

      case ResourceModifierNS::ROT90:
        return Blitter::rotate_90(*this);

      case ResourceModifierNS::ROT180:
        return Blitter::rotate_180(*this);

      case ResourceModifierNS::ROT270:
        return Blitter::rotate_270(*this);

      case ResourceModifierNS::ROT0FLIP:
        return Blitter::flip_horizontal(*this);

      case ResourceModifierNS::ROT90FLIP:
        return Blitter::rotate_90_flip(*this);

      case ResourceModifierNS::ROT180FLIP:
        return Blitter::rotate_180_flip(*this);

      case ResourceModifierNS::ROT270FLIP:
        return Blitter::rotate_270_flip(*this);

      default:
        perr << "Surface: unhandled modifier: " << modifier << std::endl;
        return *this;
    }
}

void
Surface::optimize()
{
  if (impl.get() && !impl->optimized)
    {
      // FIXME: Could add a check to check if the surface is already optimized
      SDL_Surface* old_surface = impl->surface;

      if (impl->surface->format->Amask != 0 || (impl->surface->flags & SDL_SRCCOLORKEY))
        impl->surface = SDL_DisplayFormatAlpha(old_surface);
      else
        impl->surface = SDL_DisplayFormat(old_surface);
  
      SDL_FreeSurface(old_surface);

      impl->optimized = true;
    }
}

Surface
Surface::scale(int w, int h)
{
  return Surface(boost::shared_ptr<SurfaceImpl>
                 (new SurfaceImpl(Blitter::scale_surface(impl->surface, w, h), true)));
}

Surface
Surface::clone() const
{
  SDL_Surface* new_surface = Blitter::create_surface_from_format(impl->surface, 
                                                                 impl->surface->w, impl->surface->h);
  SDL_BlitSurface(impl->surface, NULL, new_surface, NULL);
 
 return Surface(boost::shared_ptr<SurfaceImpl>(new SurfaceImpl(new_surface, true)));
}

Surface
Surface::subsection(const Rect& rect) const
{
  SDL_Surface* new_surface;
  new_surface = Blitter::create_surface_from_format(impl->surface,
                                                    rect.get_width(), rect.get_height());
  SDL_Rect dst_rect;
  dst_rect.x = rect.left;
  dst_rect.y = rect.top;

  if (impl->surface->format->palette)
    SDL_SetPalette(new_surface, SDL_LOGPAL, impl->surface->format->palette->colors, 
                   0, impl->surface->format->palette->ncolors);

  SDL_BlitSurface(impl->surface, NULL, new_surface, &dst_rect);

  /* FIXME: Need to copy palette and color key?!
    if (impl->surface->format->palette)
    SDL_SetPalette(subsurface, SDL_LOGPAL, impl->surface->format->palette->colors, 
    0, impl->surface->format->palette->ncolors);

    if (impl->surface->flags & SDL_SRCCOLORKEY)
    SDL_SetColorKey(subsurface, SDL_SRCCOLORKEY, impl->surface->format->colorkey);
  */

  return Surface(boost::shared_ptr<SurfaceImpl>(new SurfaceImpl(new_surface, true)));
}

void
Surface::fill(const Color& color)
{
  // FIXME: Couldn't get this to work with a RGBA surface for some
  // reason, something to do with tmp format and impl->surface
  // matching up maybe, anyway with RGB it works and it saves a
  // little bit of space to
  // FIXME: sould/should use a proper RGBA rect_fill function, this is just a work around
  SDL_Surface* tmp = Blitter::create_surface_rgb(impl->surface->w, impl->surface->h);
  SDL_FillRect(tmp, NULL, SDL_MapRGBA(tmp->format, color.r, color.g, color.b, 255));
  SDL_SetAlpha(tmp, SDL_SRCALPHA, color.a);
          
  SDL_BlitSurface(tmp, NULL, impl->surface, NULL);

  SDL_FreeSurface(tmp);
}

/* EOF */
