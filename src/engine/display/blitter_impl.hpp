//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_BLITTER_IMPL_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_BLITTER_IMPL_HPP

#include "engine/display/surface.hpp"
#include "util/log.hpp"

/** A collection of helper functions for the blitter class */
namespace BlitterImpl {

/** Rotate a surface 90 degree */
struct transform_rot90
{
  static inline int get_index(int width, int height, int spitch, int tpitch, int x, int y) {
    return (x * tpitch) + (height - y - 1);
  }

  static inline int get_width(int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

/** Rotate a surface 180 degree */
struct transform_rot180
{
  static inline int get_index(int width, int height, int spitch, int tpitch, int x, int y) 
  {
    return (spitch * height) - (y * spitch + x) - 1;
  }

  static inline int get_width (int width, int height) { return width; }
  static inline int get_height(int width, int height) { return height; }
};

/** Rotate a surface 270 degree */
struct transform_rot270
{
  static inline int get_index(int width, int height, int spitch, int tpitch, int x, int y) {
    return ((width - x - 1) * tpitch) + y;
  }

  static inline int get_width (int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

/** flip a surface  */
struct transform_flip
{
  static inline int get_index(int width, int height, int spitch, int tpitch, int x, int y) 
  {
    return (y * spitch) + (width - x - 1);
  }

  static inline int get_width (int width, int height) { return width; }
  static inline int get_height(int width, int height) { return height; }
};

/** Rotate a surface 90 degree and then flip it */
struct transform_rot90_flip
{
  static inline int get_index(int width, int height, int spitch, int tpitch, int x, int y) 
  {
    return (x * tpitch) + y;
  }

  static inline int get_width (int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

/** Rotate a surface 180 degree and then flip it */
struct transform_rot180_flip
{
  static inline int get_index(int width, int height, int spitch, int tpitch, int x, int y) {
    return ((height - y - 1) * spitch) + x;
  }

  static inline int get_width (int width, int height) { return width; }
  static inline int get_height(int width, int height) { return height; }
};

/** Rotate a surface 270 degree and then flip it */
struct transform_rot270_flip
{
  static inline int get_index(int width, int height, int spitch, int tpitch, int x, int y) {
    return ((width - x - 1) * tpitch) + height - y - 1;
  }

  static inline int get_width (int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

template<class Transform>
inline
Surface modify(Surface source_buffer)
{
  SDL_Surface* source = source_buffer.get_surface();
  SDL_LockSurface(source);

  if (source->format->palette)
  {
    Surface target_buffer(Transform::get_width (source_buffer.get_width(), source_buffer.get_height()), 
                          Transform::get_height(source_buffer.get_width(), source_buffer.get_height()),
                          source->format->palette, 
                          (source->flags & SDL_SRCCOLORKEY) ? source->format->colorkey : -1);
    SDL_Surface* target = target_buffer.get_surface();
    SDL_LockSurface(target);

    uint8_t* source_buf = static_cast<uint8_t*>(source->pixels);
    uint8_t* target_buf = static_cast<uint8_t*>(target->pixels);

    for (int y = 0; y < source->h; ++y)
      for (int x = 0; x < source->w; ++x)
      {
        target_buf[Transform::get_index(source->w, source->h, source->pitch, target->pitch, x, y)] = source_buf[y * source->pitch + x];
      }
     
    SDL_UnlockSurface(source);
    SDL_UnlockSurface(target);
    return target_buffer;
  }
  else
  {
    log_error("Blitter::modify: Unsupported PixelFormat: BytesPerPixel: "
              << int(source->format->BytesPerPixel));
    SDL_UnlockSurface(source);
    return source_buffer.clone();
  }
}

} // namespace BlitterImpl

#endif

/* EOF */
