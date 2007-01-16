//  $Id: blitter.hxx,v 1.12 2003/10/19 12:25:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_BLITTER_HXX
#define HEADER_PINGUS_BLITTER_HXX

#include "pingus.hxx"
////#include <ClanLib/Display/color.h>

class CL_Surface;
class PixelBuffer;

#if 0
/** A bunch of blitting and creation functions to operate on
    PixelBuffer.  Some of them a similar to the ones in ClanLib, but
    this are slower and work. */
class Blitter
{
private:
  ///
  static void put_surface_8bit(PixelBuffer target, PixelBuffer source,
                                   int x, int y);
  ///
  static void put_surface_32bit(PixelBuffer target, PixelBuffer source,
			       int x, int y);
public:
  /** Puts a given surface to a given canvas at position x, y. */
  static void put_surface(PixelBuffer target, const CL_Surface& source,
			  int x, int y);

  /** Puts a given surface provider to a given canvas at position x, y. */
  static void put_surface(PixelBuffer target, PixelBuffer source,
			  int x, int y);

  /** Returns a newly allocated canvas. The canvas contains the same
      image as the given surface. */
  static PixelBuffer create_canvas(const CL_Surface&);

  /** Returns a newly allocated canvas. The canvas contains the same
      image as the given surface provider */
  static PixelBuffer create_canvas(PixelBuffer );

  /** Sets all pixels of a canvas to zero */
  static void clear_canvas(PixelBuffer, Color color = Color(0, 0, 0, 0));

  static void fill_rect(PixelBuffer target, const CL_Rect& rect, const Color& color);

  /** Creates a new surface (based on a canvas) with the given width
      and height and stretches the source surface onto it

      @param sur The source surface
      @param width The new width of the surface.
      @param height The new height of the surface.
      @return A newly created surface, the caller is responsible to delete it. */
  static CL_Surface scale_surface (const CL_Surface& sur, int width, int height);

  /** Flip a surface horizontal */
  static PixelBuffer flip_horizontal (PixelBuffer sur);

  /** Flip a surface vertical */
  static PixelBuffer flip_vertical (PixelBuffer sur);

  /** Rotate a surface 90 degrees */
  static PixelBuffer rotate_90 (PixelBuffer sur);

  static PixelBuffer rotate_180 (PixelBuffer sur);

  static PixelBuffer rotate_270 (PixelBuffer sur);

  static PixelBuffer rotate_90_flip (PixelBuffer sur);

  static PixelBuffer rotate_180_flip (PixelBuffer sur);

  static PixelBuffer rotate_270_flip (PixelBuffer sur);

  /** Creates a new canvas with the given width and height and
      stretches the source surface onto it, the caller is responsible
      to delete the returned PixelBuffer.

      @param sur The source surface
      @param width The new width of the surface.
      @param height The new height of the surface.
      @return A newly created surface, the caller is responsible to delete it. */
  static PixelBuffer scale_surface_to_canvas (const CL_Surface& sur, int width, int height);

  static PixelBuffer scale_surface_to_canvas (PixelBuffer sur, int width, int height);

private:
  Blitter (const Blitter&);
  Blitter& operator= (const Blitter&);
};
#endif

#endif

/* EOF */

