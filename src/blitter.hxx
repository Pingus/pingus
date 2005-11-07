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
#include <ClanLib/Display/color.h>

class CL_Surface;
class CL_PixelBuffer;

namespace Pingus {

/** A bunch of blitting and creation functions to operate on
    CL_PixelBuffer.  Some of them a similar to the ones in ClanLib, but
    this are slower and work. */
class Blitter
{
private:
  ///
  static void put_surface_8bit(CL_PixelBuffer target, CL_PixelBuffer source,
                                   int x, int y);
  ///
  static void put_surface_32bit(CL_PixelBuffer target, CL_PixelBuffer source,
			       int x, int y);
public:
  /*void put_surface(CL_LockableSurface* provider, CL_Surface surface,
    int x, int y);
    void put_surface(CL_LockableSurface* provider, CL_PixelBuffer* surface,
    int x, int y);
  */

  /// Puts a given surface to a given canvas at position x, y.
  static void put_surface(CL_PixelBuffer target, const CL_Surface& source,
			  int x, int y);

  /// Puts a given surface provider to a given canvas at position x, y.
  static void put_surface(CL_PixelBuffer target, CL_PixelBuffer source,
			  int x, int y);

  /** Returns a newly allocated canvas. The canvas contains the same
      image as the given surface. */
  static CL_PixelBuffer create_canvas(const CL_Surface&);

  /** Returns a newly allocated canvas. The canvas contains the same
      image as the given surface provider */
  static CL_PixelBuffer create_canvas(CL_PixelBuffer );

  /** Sets all pixels of a canvas to zero */
  static void clear_canvas(CL_PixelBuffer, CL_Color color = CL_Color(0, 0, 0, 0));

  static void fill_rect(CL_PixelBuffer target, const CL_Rect& rect, const CL_Color& color);

  /** Creates a new surface (based on a canvas) with the given width
      and height and stretches the source surface onto it

      @param sur The source surface
      @param width The new width of the surface.
      @param height The new height of the surface.
      @return A newly created surface, the caller is responsible to delete it. */
  static CL_Surface scale_surface (const CL_Surface& sur, int width, int height);

  /** Flip a surface horizontal */
  static CL_PixelBuffer flip_horizontal (CL_PixelBuffer sur);

  /** Flip a surface vertical */
  static CL_PixelBuffer flip_vertical (CL_PixelBuffer sur);

  /** Rotate a surface 90 degrees */
  static CL_PixelBuffer rotate_90 (CL_PixelBuffer sur);

  static CL_PixelBuffer rotate_180 (CL_PixelBuffer sur);

  static CL_PixelBuffer rotate_270 (CL_PixelBuffer sur);

  static CL_PixelBuffer rotate_90_flip (CL_PixelBuffer sur);

  static CL_PixelBuffer rotate_180_flip (CL_PixelBuffer sur);

  static CL_PixelBuffer rotate_270_flip (CL_PixelBuffer sur);

  /** Creates a new canvas with the given width and height and
      stretches the source surface onto it, the caller is responsible
      to delete the returned CL_PixelBuffer.

      @param sur The source surface
      @param width The new width of the surface.
      @param height The new height of the surface.
      @return A newly created surface, the caller is responsible to delete it. */
  static CL_PixelBuffer scale_surface_to_canvas (const CL_Surface& sur, int width, int height);

  static CL_PixelBuffer scale_surface_to_canvas (CL_PixelBuffer sur, int width, int height);

private:
  Blitter (const Blitter&);
  Blitter& operator= (const Blitter&);
};

} // namespace Pingus

#endif

/* EOF */

