//  $Id$
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

#include "SDL.h"
#include "pingus.hpp"
#include "math/color.hpp"
#include "math/rect.hpp"

class Surface;

/** A bunch of blitting and creation functions to operate on
    Surface.  Some of them a similar to the ones in ClanLib, but
    this are slower and work. */
class Blitter
{
public:
  static SDL_Surface* create_surface_rgba(int w, int h);
  static SDL_Surface* create_surface_rgb(int w, int h);

  /** Flip a surface horizontal */
  static Surface flip_horizontal (Surface sur);

  /** Flip a surface vertical */
  static Surface flip_vertical (Surface sur);

  /** Rotate a surface 90 degrees */
  static Surface rotate_90 (Surface sur);

  static Surface rotate_180 (Surface sur);

  static Surface rotate_270 (Surface sur);

  static Surface rotate_90_flip (Surface sur);

  static Surface rotate_180_flip (Surface sur);

  static Surface rotate_270_flip (Surface sur);

  /** Creates a new surface with the given width and height and
      stretches the source surface onto it, the caller is responsible
      to delete the returned Surface.

      @param surface The source surface
      @param width The new width of the surface.
      @param height The new height of the surface.
      @return A newly created surface, the caller is responsible to delete it. */
  static SDL_Surface* scale_surface(SDL_Surface* surface, int width, int height);

  static Surface scale_surface_to_canvas(Surface provider, int width, int height);

private:
  Blitter (const Blitter&);
  Blitter& operator= (const Blitter&);
};

#endif

/* EOF */

