//  $Id: blitter.hxx,v 1.1 2002/06/12 19:04:45 grumbel Exp $
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

#ifndef BLITTER_HH
#define BLITTER_HH

class CL_Canvas;
class CL_Surface;
class CL_SurfaceProvider;

/** A bunch of blitting and creation functions to operate on
    CL_Canvas.  Some of them a similar to the ones in ClanLib, but
    this are slower and work. */
class Blitter
{
private:
  ///
  static void put_surface_8bit(CL_Canvas*, CL_SurfaceProvider*,
			       int x, int y);
  ///
  static void put_surface_32bit(CL_Canvas*, CL_SurfaceProvider*,
			       int x, int y);
public:
  /*void put_surface(CL_LockableSurface* provider, CL_Surface surface,
    int x, int y);
    void put_surface(CL_LockableSurface* provider, CL_SurfaceProvider* surface,
    int x, int y);
  */

  /// Puts a given surface to a given canvas at position x, y.
  static void put_surface(CL_Canvas*, const CL_Surface&,
			  int x, int y);

  /// Puts a given surface provider to a given canvas at position x, y.
  static void put_surface(CL_Canvas*, CL_SurfaceProvider*,
			  int x, int y);

  /** Makes all pixels in canvas tranparent, when their indexed value
      in provider is larger than zero.*/
  static void put_alpha_surface(CL_Canvas* canvas, CL_SurfaceProvider* provider,
				int x, int y);

  /** Returns a newly allocated canvas. The canvas contains the same
      image as the given surface. */
  static CL_Canvas* create_canvas(const CL_Surface&);

  /** Returns a newly allocated canvas. The canvas contains the same
      image as the given surface provider */
  static CL_Canvas* create_canvas(CL_SurfaceProvider*);

  /** Sets all pixels of a canvas to zero */
  static CL_Canvas* clear_canvas(CL_Canvas*);
  
  /** Creates a new surface (based on a canvas) with the given width
      and height and stretches the source surface onto it

      @param sur The source surface
      @param width The new width of the surface. 
      @param height The new height of the surface. 
      @return A newly created surface, the caller is responsible to delete it. */
  static CL_Surface scale_surface (const CL_Surface& sur, int width, int height);

  /** Creates a new canvas with the given width and height and
      stretches the source surface onto it

      @param sur The source surface
      @param width The new width of the surface. 
      @param height The new height of the surface. 
      @return A newly created surface, the caller is responsible to delete it. */
  static CL_Canvas* Blitter::scale_surface_to_canvas (const CL_Surface& sur, int width, int height);
};

#endif 

/* EOF */

