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

#include "pingus/collision_mask.hpp"

#include <boost/format.hpp>

#include "pingus/resource.hpp"
#include "util/log.hpp"

CollisionMask::CollisionMask() :
  surface(),
  buffer(),
  width(),
  height()
{
}

CollisionMask::CollisionMask(const std::string& gfx_name, const std::string& col_name) :
  surface(),
  buffer(),
  width(),
  height()
{
  surface = Resource::load_surface(gfx_name);
  init_colmap(Resource::load_surface(col_name), col_name);
}

CollisionMask::CollisionMask(const std::string& name) :
  surface(),
  buffer(),
  width(),
  height()
{
  surface = Resource::load_surface(name);
  init_colmap(surface, name);
}

CollisionMask::CollisionMask(const ResDescriptor& res_desc) :
  surface(),
  buffer(),
  width(),
  height()
{
  surface = Resource::load_surface(res_desc);
  init_colmap(surface, res_desc.res_name);
}

void
CollisionMask::init_colmap(const Surface& surf, const std::string& surface_res)
{
  int pitch = surf.get_pitch();
  width  = surf.get_width();
  height = surf.get_height();

  buffer.reset(new uint8_t[static_cast<size_t>(width * height)]);

  SDL_Surface* sdl_surface = surf.get_surface();
  SDL_LockSurface(sdl_surface);

  if (sdl_surface->format->palette)
  {
    uint8_t* source = static_cast<uint8_t*>(sdl_surface->pixels);
    Uint32 colorkey;
    if (SDL_GetColorKey(sdl_surface, &colorkey) == 0)
    { // surface with transparent areas
      for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x)
        {
          if (source[y*pitch + x] == colorkey)
            buffer[static_cast<size_t>(y*width + x)] = 0;
          else
            buffer[static_cast<size_t>(y*width + x)] = 1;
        }
    }
    else
    { // completly opaque surface
      memset(buffer.get(), 1, static_cast<size_t>(width * height));
    }
  }
  else if (sdl_surface->format->BitsPerPixel == 24)
  {
    // completly opaque surface
    memset(buffer.get(), 1, static_cast<size_t>(width * height));
  }
  else if (sdl_surface->format->BitsPerPixel == 32)
  {
    uint8_t* source = static_cast<uint8_t*>(sdl_surface->pixels);
    for(int y = 0; y < height; ++y)
    {
      for(int x = 0; x < width; ++x)
      {
        buffer[static_cast<size_t>(y * width + x)] = (source[static_cast<size_t>(y * pitch + 4*x + 3)] == 255);
      }
    }
  }
  else
  {
    memset(buffer.get(), 0, static_cast<size_t>(width * height));

    log_error("unsupported image format:\n"
              "  File: %s\n"
              "  BitsPerPixel: %d\n"
              "  BytesPerPixel: %d\n"
              "  rmask: 0x%08x\n"
              "  gmask: 0x%08x\n"
              "  bmask: 0x%08x\n"
              "  amask: 0x%08x\n",
              surface_res.c_str(),
              static_cast<int>(sdl_surface->format->BitsPerPixel),
              static_cast<int>(sdl_surface->format->BytesPerPixel),
              static_cast<unsigned int>(sdl_surface->format->Rmask),
              static_cast<unsigned int>(sdl_surface->format->Gmask),
              static_cast<unsigned int>(sdl_surface->format->Bmask),
              static_cast<unsigned int>(sdl_surface->format->Amask));
  }

  SDL_UnlockSurface(sdl_surface);
}

CollisionMask::~CollisionMask()
{
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

Surface
CollisionMask::get_surface() const
{
  return surface;
}

uint8_t*
CollisionMask::get_data() const
{
  return buffer.get();
}

/* EOF */
