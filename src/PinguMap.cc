//  $Id: PinguMap.cc,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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

#include "PinguMap.hh"

void 
PinguMap::mark_dirty(int x,int y,int w,int h)
{
  
}

// Return '0' to indicate that the colmap can not be generated
// automaticly.
ColMap*
PinguMap::get_colmap()
{
  return 0;
}

void
PinguMap::remove(CL_Surface* sur, int x, int y)
{
  remove(sur->get_provider(), x, y);
}

void
PinguMap::remove(CL_SurfaceProvider*, int, int)
{
  
}

void 
PinguMap::put(CL_SurfaceProvider*, int, int)
{
  
}

/* EOF */
