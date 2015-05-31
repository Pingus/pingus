// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_BLITTER_IMPL_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_BLITTER_IMPL_HPP

#include "blitter.hpp"
#include "engine/display/surface.hpp"
#include "util/log.hpp"

/** A collection of helper functions for the blitter class */
namespace BlitterImpl {

struct transform_rot90
{
  static inline int get_col(int width, int height, int x, int y) { return height - y - 1; }
  static inline int get_row(int width, int height, int x, int y) { return x; }

  static inline int get_width(int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

struct transform_rot180
{
  static inline int get_col(int width, int height, int x, int y) { return width - x - 1; }
  static inline int get_row(int width, int height, int x, int y) { return height - y - 1; }

  static inline int get_width (int width, int height) { return width; }
  static inline int get_height(int width, int height) { return height; }
};

struct transform_rot270
{
  static inline int get_col(int width, int height, int x, int y) { return y; }
  static inline int get_row(int width, int height, int x, int y) { return width - x - 1; }

  static inline int get_width (int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

struct transform_flip
{
  static inline int get_col(int width, int height, int x, int y) { return width - x - 1; }
  static inline int get_row(int width, int height, int x, int y) { return y; }

  static inline int get_width (int width, int height) { return width; }
  static inline int get_height(int width, int height) { return height; }
};

struct transform_rot90_flip
{
  static inline int get_col(int width, int height, int x, int y) { return y; }
  static inline int get_row(int width, int height, int x, int y) { return x; }

  static inline int get_width (int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

struct transform_rot180_flip
{
  static inline int get_col(int width, int height, int x, int y) { return x; }
  static inline int get_row(int width, int height, int x, int y) { return height - y - 1; }

  static inline int get_width (int width, int height) { return width; }
  static inline int get_height(int width, int height) { return height; }
};

struct transform_rot270_flip
{
  static inline int get_col(int width, int height, int x, int y) { return height - y - 1; }
  static inline int get_row(int width, int height, int x, int y) { return width - x - 1; }

  static inline int get_width (int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

template<class Transform>
inline
Surface modify(Surface source)
{
  SDL_Surface* target_surface
    = Blitter::create_surface_from_format(source.get_surface(),
                                          Transform::get_width (source.get_width(), source.get_height()),
                                          Transform::get_height(source.get_width(), source.get_height()));
  Surface target(target_surface);

  const int bpp = target.get_surface()->format->BytesPerPixel;

  target.lock();
  source.lock();

  uint8_t* source_buf = source.get_data();
  uint8_t* target_buf = target.get_data();

  if (bpp == 1)
  {
    for (int y = 0; y < source.get_height(); ++y)
    {
      for (int x = 0; x < source.get_width(); ++x)
      {
        const int col = Transform::get_col(source.get_width(), source.get_height(), x, y);
        const int row = Transform::get_row(source.get_width(), source.get_height(), x, y);

        const int tidx = row * target.get_pitch() + col;
        const int sidx = y * source.get_pitch() + x;

        target_buf[tidx] = source_buf[sidx];
      }
    }
  }
  else if (bpp == 3)
  {
    for (int y = 0; y < source.get_height(); ++y)
    {
      for (int x = 0; x < source.get_width(); ++x)
      {
        const int col = Transform::get_col(source.get_width(), source.get_height(), x, y);
        const int row = Transform::get_row(source.get_width(), source.get_height(), x, y);

        const int tidx = row * target.get_pitch() + col * 3;
        const int sidx = y * source.get_pitch() + x * 3;

        target_buf[tidx+0] = source_buf[sidx+0];
        target_buf[tidx+1] = source_buf[sidx+1];
        target_buf[tidx+2] = source_buf[sidx+2];
      }
    }
  }
  else if (bpp == 4)
  {
    for (int y = 0; y < source.get_height(); ++y)
    {
      for (int x = 0; x < source.get_width(); ++x)
      {
        const int col = Transform::get_col(source.get_width(), source.get_height(), x, y);
        const int row = Transform::get_row(source.get_width(), source.get_height(), x, y);

        const int tidx = row * target.get_pitch() + col * 4;
        const int sidx = y * source.get_pitch() + x * 4;

        // copy all four subpixels in one go
        *reinterpret_cast<uint32_t*>(target_buf + tidx) = *reinterpret_cast<uint32_t*>(source_buf + sidx);
      }
    }
  }
  else
  {
    log_error("unhandled BytesPerPixel: %1%", bpp);
  }

  source.unlock();
  target.unlock();

  return target;
}

} // namespace BlitterImpl

#endif

/* EOF */
