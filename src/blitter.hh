//  $Id: blitter.hh,v 1.6 2000/06/18 17:01:50 grumbel Exp $
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

#include <ClanLib/core.h>

///
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
  /**void put_surface(CL_LockableSurface* provider, CL_Surface* surface,
    int x, int y);
    void put_surface(CL_LockableSurface* provider, CL_SurfaceProvider* surface,
    int x, int y);
  */
  static void put_surface(CL_Canvas*, CL_Surface*,
			  int x, int y);

  ///
  static void put_surface(CL_Canvas*, CL_SurfaceProvider*,
			  int x, int y);

  ///
  static void put_alpha_surface(CL_Canvas*, CL_SurfaceProvider*,
				int x, int y);

  ///
  static CL_Canvas* create_canvas(CL_Surface*);
  ///
  static CL_Canvas* create_canvas(CL_SurfaceProvider*);
  ///
  static CL_Canvas* clear_canvas(CL_Canvas*);
  
  //static CL_Surface* convert_to_emptyprovider(CL_Surface*);
  ///static CL_Canvas* convert_to_emptyprovider(CL_SurfaceProvider*);
  
};

#endif 

/* EOF */
