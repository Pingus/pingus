//  $Id: pingu_map.cxx,v 1.9 2003/10/19 12:25:47 grumbel Exp $
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

#include <ClanLib/Display/surface.h>
#include "pingu_map.hxx"

namespace Pingus {

PinguMap::PinguMap()
{
}

PinguMap::~PinguMap()
{
  //std::cout << "PinguMap:~PinguMap()" << std::endl;
}

void
PinguMap::mark_dirty(int x,int y,int w,int h)
{
	UNUSED_ARG(x);
	UNUSED_ARG(y);
	UNUSED_ARG(w);
	UNUSED_ARG(h);
}

// Return '0' to indicate that the colmap can not be generated
// automaticly.
ColMap*
PinguMap::get_colmap()
{
  return 0;
}

void
PinguMap::remove(const CL_Surface& sur, int x, int y)
{
  remove(sur.get_provider(), x, y);
}

void
PinguMap::remove(const CL_PixelBuffer&, int, int)
{

}

void
PinguMap::put(const CL_PixelBuffer&, int, int)
{

}

void
PinguMap::put(const CL_Surface& sur, int x, int y)
{
  put (sur.get_provider (), x, y);
}

} // namespace Pingus

/* EOF */
