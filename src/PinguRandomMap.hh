//  $Id: PinguRandomMap.hh,v 1.3 2000/06/18 17:01:50 grumbel Exp $
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

#ifndef PINGURANDOMMAP_HH
#define PINGURANDOMMAP_HH

#include <vector>
#include <fstream>

#include "PLF.hh"
#include "ColMap.hh"
#include "PinguMap.hh"

///
class PinguRandomMap : public PinguMap 
{
private:
  ///
  bool need_reload;
  ///
  ColMap* colmap;
  ///
  CL_Surface* sub_surface[100][100];
  ///
  CL_Surface* surface;
  ///
  int tiles_x;
  ///
  int tiles_y;
  ///
  CL_SurfaceProvider* sur_provider;
public:
  ///
  PinguRandomMap();
  ///
  virtual ~PinguRandomMap();

  ///
  ColMap* get_colmap();
  ///
  void draw_offset(int of_x, int of_y, float s = 1.0);
  ///
  int  get_width();
  ///
  int  get_height();
  ///
  void remove(int, int);
  ///
  void remove(CL_SurfaceProvider*, int, int) {}///
;
  ///
  void put(CL_SurfaceProvider*, int, int) {}///
;
}///
;

#endif

/* EOF */

