//  $Id: blitter_impl.hxx,v 1.8 2003/04/02 20:47:45 grumbel Exp $
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
#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include <ClanLib/Display/Display/surface.h>
#include "indexed_canvas.hxx"
#include "pingus.hxx"


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
CL_Surface modify(const CL_Surface& sur)
{
  CL_SurfaceProvider* prov = sur.get_provider ();
  int pwidth  = prov->get_width();
  int pheight = prov->get_height();

  if (prov->is_indexed())
    {
      std::cout << "Using indexed blitter" << std::endl;

      IndexedCanvas* canvas = new IndexedCanvas(TransF::get_width (pwidth, pheight),
                                                TransF::get_height(pwidth, pheight));
      if (prov->uses_src_colorkey())
        canvas->set_src_colorkey(prov->get_src_colorkey());
      
      prov->lock ();
      canvas->lock ();

      canvas->set_palette(prov->get_palette());

      unsigned char* source_buf = static_cast<unsigned char*>(prov->get_data());
      unsigned char* target_buf = static_cast<unsigned char*>(canvas->get_data());

      for (int y = 0; y < pheight; ++y)
        for (int x = 0; x < pwidth; ++x)
          {
#ifdef PINGUS_DO_ROTBLITTER_BOUNDCHECK 
            int i = TransF::get_index(pwidth, pheight, x, y);
            if (i < 0 || i >= pwidth * pheight)
              {
                std::cout << "Target: Out of bounce: " << i << " " << pwidth << "x" << pheight
                          << " " <<  typeid(TransF()).name() << std::endl;
              }
            
            if (y * pwidth + x < 0
                || y * pwidth + x >= pwidth * pheight)
              {
                std::cout << "Source: Out of bounce: " << i << " " << pwidth << "x" << pheight
                          << " " <<  typeid(TransF()).name() << std::endl;
              }
#endif
            target_buf[TransF::get_index(pwidth, pheight, x, y)] = source_buf[y * pwidth + x];
          }

      canvas->unlock ();
      prov->unlock ();
      return CL_Surface(canvas, true);     
    }
  else
    {
      CL_Canvas* canvas = new CL_Canvas (sur.get_height (), sur.get_width ());

      prov->lock ();
      canvas->lock ();

      float r, b, g, a;
      for (unsigned int y = 0; y < sur.get_height (); ++y)
        for (unsigned int x = 0; x < sur.get_width (); ++x)
          {
            prov->get_pixel (x, y, &r, &g, &b, &a);
            canvas->draw_pixel (TransF::get_x(pwidth, pheight, x, y),
                                TransF::get_y(pwidth, pheight, x, y),
                                r, g, b, a);
          }

      canvas->unlock ();
      prov->unlock ();
      return CL_Surface(canvas, true);
    }
}

} // namespace BlitterImpl

#endif

/* EOF */
