//  $Id: blitter_impl.hxx,v 1.16 2003/10/21 21:37:05 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_BLITTER_IMPL_HXX
#define HEADER_PINGUS_BLITTER_IMPL_HXX

#include <iostream>
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/surface.h>
#include "pingus.hxx"

namespace Pingus {

/** A collection of helper functions for the blitter class */
namespace BlitterImpl
{

/** Rotate a surface 90 degree */
struct transform_rot90
{
  static inline int get_index(int width, int height, int x, int y) {
    return (x * height) + (height - y - 1);
  }

  static inline int get_x(int width, int height, int x, int y) {
    return (height - y - 1);
  }

  static inline int get_y(int width, int height, int x, int y) {
    return x;
  }

  static inline int get_width(int width, int height) { return height; }
  static inline int get_height(int width, int height) { return width; }
};

/** Rotate a surface 180 degree */
struct transform_rot180
{
  static inline int get_index(int width, int height, int x, int y) {
    return (width * height) - (y * width + x) - 1;
  }

  static inline int get_x(int width, int height, int x, int y) { UNUSED_ARG(height); UNUSED_ARG(y);
    return width - x - 1;
  }

  static inline int get_y(int width, int height, int x, int y) { UNUSED_ARG(width);  UNUSED_ARG(x);
    return height - y - 1;
  }

  static inline int get_width (int width, int height) { UNUSED_ARG(height); return width; }
  static inline int get_height(int width, int height) { UNUSED_ARG(width);  return height; }
};

/** Rotate a surface 270 degree */
struct transform_rot270
{
  static inline int get_index(int width, int height, int x, int y) {
    return ((width - x - 1) * height) + y;
  }

  static inline int get_x(int width, int height, int x, int y) {
    UNUSED_ARG(width); UNUSED_ARG(height); UNUSED_ARG(x);
    return y;
  }

  static inline int get_y(int width, int height, int x, int y) {
    UNUSED_ARG(height);
    UNUSED_ARG(y);

    return width - x - 1;
  }

  static inline int get_width (int width, int height) { UNUSED_ARG(width);  return height; }
  static inline int get_height(int width, int height) { UNUSED_ARG(height); return width; }
};

/** flip a surface  */
struct transform_flip
{
  static inline int get_index(int width, int height, int x, int y) {
    UNUSED_ARG(height);
    return (y * width) + (width - x - 1);
  }

  static inline int get_x(int width, int height, int x, int y) {
    UNUSED_ARG(height); UNUSED_ARG(y);
    return width - x - 1;
  }

  static inline int get_y(int width, int height, int x, int y) {
    UNUSED_ARG(width); UNUSED_ARG(height); UNUSED_ARG(x);
    return y;
  }

  static inline int get_width (int width, int height) { UNUSED_ARG(height);  return width; }
  static inline int get_height(int width, int height) { UNUSED_ARG(width); return height; }
};

/** Rotate a surface 90 degree and then flip it */
struct transform_rot90_flip
{
  static inline int get_index(int width, int height, int x, int y) {
    UNUSED_ARG(width);
    return (x * height) + y;
  }

  static inline int get_x(int width, int height, int x, int y) {
    UNUSED_ARG(width); UNUSED_ARG(height); UNUSED_ARG(x);
    return y;
  }

  static inline int get_y(int width, int height, int x, int y) {
    UNUSED_ARG(width); UNUSED_ARG(height); UNUSED_ARG(y);
    return x;
  }

  static inline int get_width (int width, int height) { UNUSED_ARG(width);  return height; }
  static inline int get_height(int width, int height) { UNUSED_ARG(height); return width; }
};

/** Rotate a surface 180 degree and then flip it */
struct transform_rot180_flip
{
  static inline int get_index(int width, int height, int x, int y) {
    return ((height - y - 1) * width) + x;
  }

  static inline int get_x(int width, int height, int x, int y) {
    UNUSED_ARG(width); UNUSED_ARG(height); UNUSED_ARG(y);
    return x;
  }

  static inline int get_y(int width, int height, int x, int y) {
    UNUSED_ARG(width); UNUSED_ARG(x);
    return height - y - 1;
  }

  static inline int get_width (int width, int height) { UNUSED_ARG(height);  return width; }
  static inline int get_height(int width, int height) { UNUSED_ARG(width); return height; }
};

/** Rotate a surface 270 degree and then flip it */
struct transform_rot270_flip
{
  static inline int get_index(int width, int height, int x, int y) {
    return ((width - x - 1) * height) + height - y - 1;
  }

  static inline int get_x(int width, int height, int x, int y) {
    UNUSED_ARG(width); UNUSED_ARG(x);
    return height - y - 1;
  }

  static inline int get_y(int width, int height, int x, int y) {
    UNUSED_ARG(height); UNUSED_ARG(y);
    return width - x - 1;
  }

  static inline int get_width (int width, int height) { UNUSED_ARG(width);  return height; }
  static inline int get_height(int width, int height) { UNUSED_ARG(height); return width; }
};

template<class TransF>
inline
CL_PixelBuffer modify(CL_PixelBuffer prov, const TransF&)
{
  if (prov.get_format().get_type() ==  pixelformat_index)
    {
      CL_PixelFormat format(8, 0, 0, 0, 0, 
                            prov.get_format().has_colorkey(), prov.get_format().get_colorkey(),
                            pixelformat_index);
      
      CL_PixelBuffer canvas(TransF::get_width (prov.get_width(), prov.get_height()), 
                            TransF::get_height(prov.get_width(), prov.get_height()),
                            TransF::get_width (prov.get_width(), prov.get_height()),
                            format, prov.get_palette());

      prov.lock ();
      canvas.lock ();

      unsigned char* source_buf = static_cast<unsigned char*>(prov.get_data());
      unsigned char* target_buf = static_cast<unsigned char*>(canvas.get_data());

      int pwidth  = prov.get_width();
      int pheight = prov.get_height();

      for (int y = 0; y < pheight; ++y)
        for (int x = 0; x < pwidth; ++x)
          {
            target_buf[TransF::get_index(pwidth, pheight, x, y)] = source_buf[y * pwidth + x];
          }

      canvas.unlock ();
      prov.unlock ();
      
      return canvas;
    }
  else
    {
      int pwidth  = prov.get_width();
      int pheight = prov.get_height();
			
			CL_PixelBuffer canvas(prov.get_height(), pwidth, pheight*4, CL_PixelFormat::rgba8888);

      prov.lock();
      canvas.lock();

      for (int y = 0; y < pheight; ++y)
        for (int x = 0; x < pwidth; ++x)
          {
            CL_Color color = prov.get_pixel(x, y);
            canvas.draw_pixel(TransF::get_x(pwidth, pheight, x, y),
                              TransF::get_y(pwidth, pheight, x, y),
                              color);
          }

      canvas.unlock ();
      prov.unlock ();

      return canvas;
    }
}

} // namespace BlitterImpl
} // namespace Pingus

#endif

/* EOF */
