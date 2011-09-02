//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/display/surface.hpp"

#include <SDL_image.h>
#include <boost/format.hpp>

#include "engine/display/blitter.hpp"
#include "math/rect.hpp"
#include "pingus/debug.hpp"

class SurfaceImpl
{
public:
  SDL_Surface* surface;

  SurfaceImpl()
    : surface(0)
  {}

  SurfaceImpl(SDL_Surface* surface_) :
    surface(surface_)
  {}
  
  ~SurfaceImpl() 
  {
    if (surface)
      SDL_FreeSurface(surface);
  }

private:
  SurfaceImpl(const SurfaceImpl&);
  SurfaceImpl & operator=(const SurfaceImpl&);
};

Surface::Surface() :
  impl()
{
}

Surface::Surface(std::shared_ptr<SurfaceImpl> impl_) :
  impl(impl_)
{
}

Surface::Surface(const Pathname& pathname) :
  impl()
{
  SDL_Surface* surface = IMG_Load(pathname.get_sys_path().c_str());
  if (surface)
  {
    impl = std::shared_ptr<SurfaceImpl>(new SurfaceImpl(surface));
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
  impl->surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, width, height, 32,
                                       0x000000ff,
                                       0x0000ff00,
                                       0x00ff0000,
                                       0xff000000);
  //SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
}

Surface::Surface(SDL_Surface* surface)
  : impl(new SurfaceImpl(surface))
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

    dstrect.x = static_cast<Sint16>(x);
    dstrect.y = static_cast<Sint16>(y);

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
  Uint8 *p = static_cast<Uint8 *>(get_surface()->pixels) + y * get_surface()->pitch + x * get_surface()->format->BytesPerPixel;
  Uint32 pixel;

  switch(get_surface()->format->BytesPerPixel)
  {
    case 1:
      pixel = *p;
    case 2: /* This will cause some problems ... */
      pixel = *reinterpret_cast<Uint16*>(p);
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        pixel = p[0] << 16 | p[1] << 8 | p[2];
      else
        pixel = p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      pixel = *reinterpret_cast<Uint32*>(p);
    default:
      pixel = 0;       /* shouldn't happen, but avoids warnings */
  } 

  Color color;
  SDL_GetRGBA(pixel, get_surface()->format, &color.r, &color.g, &color.b, &color.a);
  return color;
}

Surface
Surface::mod(ResourceModifier::Enum modifier)
{
  switch(modifier)
  {
    case ResourceModifier::ROT0:
      return this->clone();

    case ResourceModifier::ROT90:
      return Blitter::rotate_90(*this);

    case ResourceModifier::ROT180:
      return Blitter::rotate_180(*this);

    case ResourceModifier::ROT270:
      return Blitter::rotate_270(*this);

    case ResourceModifier::ROT0FLIP:
      return Blitter::flip_horizontal(*this);

    case ResourceModifier::ROT90FLIP:
      return Blitter::rotate_90_flip(*this);

    case ResourceModifier::ROT180FLIP:
      return Blitter::rotate_180_flip(*this);

    case ResourceModifier::ROT270FLIP:
      return Blitter::rotate_270_flip(*this);

    default:
      perr << "Surface: unhandled modifier: " << modifier << std::endl;
      return *this;
  }
}

Surface
Surface::scale(int w, int h)
{
  return Surface(std::shared_ptr<SurfaceImpl>
                 (new SurfaceImpl(Blitter::scale_surface(impl->surface, w, h))));
}

Surface
Surface::clone() const
{
  SDL_Surface* new_surface = Blitter::create_surface_from_format(impl->surface, 
                                                                 impl->surface->w, impl->surface->h);
  if (impl->surface->flags & SDL_SRCALPHA)
  {
    Uint8 alpha = impl->surface->format->alpha;
    SDL_SetAlpha(impl->surface, 0, 0);
    SDL_BlitSurface(impl->surface, NULL, new_surface, NULL);
    SDL_SetAlpha(impl->surface, SDL_SRCALPHA, alpha);
  }
  else
  {
    SDL_BlitSurface(impl->surface, NULL, new_surface, NULL);
  }
 
  return Surface(std::shared_ptr<SurfaceImpl>(new SurfaceImpl(new_surface)));
}

Surface
Surface::subsection(const Rect& rect) const
{
  SDL_Surface* new_surface;
  new_surface = Blitter::create_surface_from_format(impl->surface,
                                                    rect.get_width(), rect.get_height());
  SDL_Rect dst_rect;
  dst_rect.x = static_cast<Sint16>(rect.left);
  dst_rect.y = static_cast<Sint16>(rect.top);

  if (impl->surface->format->palette)
    SDL_SetPalette(new_surface, SDL_LOGPAL, impl->surface->format->palette->colors, 
                   0, impl->surface->format->palette->ncolors);

  if (impl->surface->flags & SDL_SRCCOLORKEY)
    SDL_SetColorKey(new_surface, SDL_SRCCOLORKEY, impl->surface->format->colorkey);

  SDL_BlitSurface(impl->surface, NULL, new_surface, &dst_rect);

  return Surface(std::shared_ptr<SurfaceImpl>(new SurfaceImpl(new_surface)));
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

void
Surface::print(std::ostream& out)
{
  out << boost::format("Pointer: 0x%p\n"
                       "Rmask:   0x%08x\n"
                       "Gmask:   0x%08x\n"
                       "Bmask:   0x%08x\n"
                       "Amask:   0x%08x\n"
                       "Flags:   0x%08x -> %s%s%s%s\n"
                       "Palette: 0x%08x\n"
                       "BitsPerPixel: %d\n"
    )
    % impl->surface
    % impl->surface->format->Rmask
    % impl->surface->format->Gmask
    % impl->surface->format->Bmask
    % impl->surface->format->Amask
    % impl->surface->flags
    % ((impl->surface->flags & SDL_HWSURFACE) ? "HWSURFACE " : "")
    % ((impl->surface->flags & SDL_SWSURFACE) ? "SWSURFACE " : "")
    % ((impl->surface->flags & SDL_SRCCOLORKEY) ? "SRCCOLORKEY " : "")
    % ((impl->surface->flags & SDL_SRCALPHA) ? "SRCALPHA " : "")
    % impl->surface->format->palette
    % static_cast<int>(impl->surface->format->BitsPerPixel);

  if (impl->surface->flags & SDL_SRCCOLORKEY)
    out << "Colorkey: " << static_cast<int>(impl->surface->format->colorkey) << std::endl;

  if (impl->surface->flags & SDL_SRCALPHA)
    out << "Alpha: " << static_cast<int>(impl->surface->format->alpha) << std::endl;

  if (0)
  {
    SDL_LockSurface(impl->surface);
    Uint8* pixels = static_cast<Uint8*>(impl->surface->pixels);
    for(int i = 0; i < impl->surface->pitch * impl->surface->h; i += 4)
      out << boost::format("(%3d %3d %3d %3d) ")
        % static_cast<int>(pixels[i+0])
        % static_cast<int>(pixels[i+1])
        % static_cast<int>(pixels[i+2])
        % static_cast<int>(pixels[i+3]);
    out << std::endl;
    SDL_UnlockSurface(impl->surface);
  }
}

/* EOF */
