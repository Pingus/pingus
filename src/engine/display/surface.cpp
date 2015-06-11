// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/display/surface.hpp"

#include <SDL_image.h>
#include <boost/format.hpp>
#include <stdexcept>

#include "engine/display/blitter.hpp"
#include "math/rect.hpp"
#include "util/log.hpp"
#include "util/raise_exception.hpp"

class SurfaceImpl
{
public:
  SDL_Surface* surface;

  SurfaceImpl() :
    surface()
  {
  }

  SurfaceImpl(SDL_Surface* surface_) :
    surface(surface_)
  {
  }

  ~SurfaceImpl()
  {
    if (surface)
    {
      SDL_FreeSurface(surface);
    }
  }

private:
  SurfaceImpl(const SurfaceImpl&);
  SurfaceImpl& operator=(const SurfaceImpl&);
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
  if (!surface)
  {
    raise_exception(std::runtime_error, "couldn't load " << pathname.get_sys_path() << "\n  IMG_GetError: " << IMG_GetError());
  }
  else
  {
    impl.reset(new SurfaceImpl(surface));
  }
}

Surface::Surface(int width, int height, SDL_Palette* palette, int colorkey) :
  impl(new SurfaceImpl)
{
  if (colorkey == -1)
  {
    impl->surface = SDL_CreateRGBSurface(0, width, height, 8,
                                         0, 0, 0 ,0);
  }
  else
  {
    impl->surface = SDL_CreateRGBSurface(0, width, height, 8,
                                         0, 0, 0 ,0);
    SDL_SetColorKey(impl->surface, SDL_TRUE, static_cast<Uint32>(colorkey));
  }

  SDL_SetSurfacePalette(impl->surface, palette);
}

Surface::Surface(int width, int height) :
  impl(new SurfaceImpl())
{
  impl->surface = SDL_CreateRGBSurface(0, width, height, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                                       0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
                                       0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
    );
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
Surface::blit(Surface src, int x_pos, int y_pos)
{
  if (!get_surface())
  {
    log_error("trying to blit to empty surface");
  }
  else if (!src.get_surface())
  {
    log_error("trying to blit with an empty surface");
  }
  else if (get_surface()->format->BytesPerPixel == 4 &&
           src.get_surface()->format->BytesPerPixel == 4 &&
           src.get_surface()->format->Amask)
  {
    // RGBA to RGBA blit
    int start_x = std::max(0, -x_pos);
    int start_y = std::max(0, -y_pos);

    int end_x = std::min(src.get_width(),  get_width()  - x_pos);
    int end_y = std::min(src.get_height(), get_height() - y_pos);

    // empty blit range
    if (end_x - start_x <= 0 || end_y - start_y <= 0)
      return;

    lock();
    src.lock();

    int tpitch  = get_pitch();
    uint8_t* tdata = get_data();

    int spitch = src.get_pitch();
    uint8_t* sdata = src.get_data();

    const uint32_t srmask = src.get_surface()->format->Rmask;
    const uint32_t sgmask = src.get_surface()->format->Gmask;
    const uint32_t sbmask = src.get_surface()->format->Bmask;
    const uint32_t samask = src.get_surface()->format->Amask;

    const uint8_t srshift = src.get_surface()->format->Rshift;
    const uint8_t sgshift = src.get_surface()->format->Gshift;
    const uint8_t sbshift = src.get_surface()->format->Bshift;
    const uint8_t sashift = src.get_surface()->format->Ashift;

    for(int y = start_y; y < end_y; ++y)
    {
      uint8_t*  tptr = tdata + tpitch * (y + y_pos) + 4 * (x_pos + start_x);
      uint32_t* sptr = reinterpret_cast<uint32_t*>(sdata + spitch * y + 4 * start_x);

      for(int x = start_x; x < end_x; ++x)
      {
        const uint8_t red   = static_cast<uint8_t>((*sptr & srmask) >> srshift);
        const uint8_t green = static_cast<uint8_t>((*sptr & sgmask) >> sgshift);
        const uint8_t blue  = static_cast<uint8_t>((*sptr & sbmask) >> sbshift);
        const uint8_t alpha = static_cast<uint8_t>((*sptr & samask) >> sashift);

        if (alpha == 255)
        {
          // opaque blit
          tptr[0] = red;
          tptr[1] = green;
          tptr[2] = blue;
          tptr[3] = alpha;
        }
        else if (sptr[3] == 0)
        {
          // do nothing
        }
        else
        {
          // alpha blend
          uint8_t outa = static_cast<uint8_t>((alpha + (tptr[3] * (255 - alpha)) / 255));

          if (outa == 0)
          {
            tptr[0] = 0;
            tptr[1] = 0;
            tptr[2] = 0;
            tptr[3] = 0;
          }
          else
          {
            tptr[0] = static_cast<uint8_t>(((red   * alpha + tptr[0] * tptr[3] * (255 - alpha) / 255) / outa));
            tptr[1] = static_cast<uint8_t>(((green * alpha + tptr[1] * tptr[3] * (255 - alpha) / 255) / outa));
            tptr[2] = static_cast<uint8_t>(((blue  * alpha + tptr[2] * tptr[3] * (255 - alpha) / 255) / outa));

            tptr[3] = outa;
          }
        }

        tptr += 4;
        sptr += 1;
      }
    }

    src.unlock();
    unlock();
  }
  else
  {
    SDL_Rect dstrect;

    dstrect.x = static_cast<Sint16>(x_pos);
    dstrect.y = static_cast<Sint16>(y_pos);

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
      {
        pixel = *p;

        if (impl->surface->format->palette)
        {
          Uint32 colorkey;
          if (SDL_GetColorKey(impl->surface, &colorkey) == 0 &&
              pixel == colorkey)
          {
            return Color(0,0,0,0);
          }
          else
          {
            SDL_Color c = impl->surface->format->palette->colors[pixel];
            return Color(c.r, c.g, c.b, 255);
          }
        }
      }
      break;

    case 2: /* This will cause some problems ... */
      pixel = *reinterpret_cast<Uint16*>(p);
      break;

    case 3:
      pixel = static_cast<Uint32>(p[0] | p[1] << 8 | p[2] << 16);
      break;

    case 4:
      pixel = *reinterpret_cast<Uint32*>(p);
      break;

    default:
      pixel = 0;       /* shouldn't happen, but avoids warnings */
      break;
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
      log_error("Surface: unhandled modifier: %1%", modifier);
      return *this;
  }
}

Surface
Surface::scale(int w, int h)
{
  return Surface(std::make_shared<SurfaceImpl>(Blitter::scale_surface(impl->surface, w, h)));
}

Surface
Surface::clone() const
{
  SDL_Surface* new_surface = Blitter::create_surface_from_format(impl->surface,
                                                                 impl->surface->w, impl->surface->h);
  SDL_BlendMode blend_mode;
  SDL_GetSurfaceBlendMode(impl->surface, &blend_mode);

  SDL_SetSurfaceBlendMode(impl->surface, SDL_BLENDMODE_NONE);
  SDL_BlitSurface(impl->surface, NULL, new_surface, NULL);

  SDL_SetSurfaceBlendMode(impl->surface, blend_mode);

  return Surface(std::shared_ptr<SurfaceImpl>(new SurfaceImpl(new_surface)));
}

Surface
Surface::subsection(const Rect& rect) const
{
  assert(rect.left  >= 0);
  assert(rect.top   >= 0);
  assert(rect.right <= impl->surface->w);
  assert(rect.top   <= impl->surface->h);

  SDL_Surface* new_surface
    = Blitter::create_surface_from_format(impl->surface,
                                          rect.get_width(),
                                          rect.get_height());

  SDL_LockSurface(impl->surface);
  SDL_LockSurface(new_surface);
  for(int y = 0; y < new_surface->h; ++y)
  {
    memcpy(static_cast<uint8_t*>(new_surface->pixels)
           + (y * new_surface->pitch),
           static_cast<uint8_t*>(impl->surface->pixels)
           + (impl->surface->pitch * (y + rect.top))
           + rect.left * impl->surface->format->BytesPerPixel,
           static_cast<size_t>(new_surface->pitch));
  }
  SDL_UnlockSurface(new_surface);
  SDL_UnlockSurface(impl->surface);

  return Surface(std::shared_ptr<SurfaceImpl>(new SurfaceImpl(new_surface)));
}

void
Surface::fill(const Color& color)
{
  if (color.a == 0)
  {
    // fill wouldn't change the surface, so ignore it
  }
  else
  {
    if (impl->surface->format->BytesPerPixel == 4)
    {
      SDL_LockSurface(impl->surface);
      uint8_t* pixels = static_cast<uint8_t*>(impl->surface->pixels);
      for(int y = 0; y < impl->surface->h; ++y)
      {
        for(int x = 0; x < impl->surface->w; ++x)
        {
          uint8_t* p = pixels + y * impl->surface->pitch + 4*x;

          p[0] = static_cast<uint8_t>(std::min(255, ((255 - color.a) * p[0] + color.a * color.r) / 255));
          p[1] = static_cast<uint8_t>(std::min(255, ((255 - color.a) * p[1] + color.a * color.g) / 255));
          p[2] = static_cast<uint8_t>(std::min(255, ((255 - color.a) * p[2] + color.a * color.b) / 255));
        }
      }
      SDL_UnlockSurface(impl->surface);
    }
    else if (impl->surface->format->BytesPerPixel == 3)
    {
      SDL_LockSurface(impl->surface);
      uint8_t* pixels = static_cast<uint8_t*>(impl->surface->pixels);
      for(int y = 0; y < impl->surface->h; ++y)
      {
        for(int x = 0; x < impl->surface->w; ++x)
        {
          uint8_t* p = pixels + y * impl->surface->pitch + 3*x;

          p[0] = static_cast<uint8_t>(std::min(255, ((255 - color.a) * p[0] + color.a * color.r) / 255));
          p[1] = static_cast<uint8_t>(std::min(255, ((255 - color.a) * p[1] + color.a * color.g) / 255));
          p[2] = static_cast<uint8_t>(std::min(255, ((255 - color.a) * p[2] + color.a * color.b) / 255));
        }
      }
      SDL_UnlockSurface(impl->surface);
    }
    else
    {
      log_error("unsupported BytesPerPixel format: %1%", impl->surface->format->BytesPerPixel);
    }
  }
}

Surface
Surface::convert_to_rgba() const
{
  SDL_Surface* surface = Blitter::create_surface_rgba(impl->surface->w, impl->surface->h);
  SDL_BlitSurface(impl->surface, NULL, surface, NULL);
  return Surface(surface);
}

Surface
Surface::convert_to_rgb() const
{
  SDL_Surface* surface = Blitter::create_surface_rgb(impl->surface->w, impl->surface->h);
  SDL_BlitSurface(impl->surface, NULL, surface, NULL);
  return Surface(surface);
}

bool
Surface::has_colorkey() const
{
  Uint32 colorkey;
  return SDL_GetColorKey(impl->surface, &colorkey) == 0;
}

bool
Surface::is_indexed() const
{
  return impl->surface->format->palette;
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
    % impl->surface->format->palette
    % static_cast<int>(impl->surface->format->BitsPerPixel);

  Uint32 colorkey;
  if (SDL_GetColorKey(impl->surface, &colorkey) == 0)
  {
    out << "Colorkey: " << boost::format("0x%08x") % colorkey << std::endl;
  }
  else
  {
    out << "Colorkey: <none>" << std::endl;
  }

  Uint8 alpha;
  if (SDL_GetSurfaceAlphaMod(impl->surface, &alpha) == 0)
  {
    out << "Alpha: " << static_cast<int>(alpha) << std::endl;
  }
  else
  {
    out << "Alpha: <none>" << std::endl;
  }

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
