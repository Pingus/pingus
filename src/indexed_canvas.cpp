//  $Id: indexed_canvas.cxx,v 1.3 2002/10/20 18:28:48 torangan Exp $
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

#include "indexed_canvas.hpp"
#include <assert.h>


IndexedCanvas::IndexedCanvas(int w, int h)
  : width(w),
    height(h),
    transcol(-1)
{
  data = new unsigned char [width * height];
}

IndexedCanvas::~IndexedCanvas()
{
  delete[] data;
}

void
IndexedCanvas::set_palette(CL_Palette* p)
{
  assert(p);
  memcpy(palette.palette, p->palette, p->num_colors*3);
}

CL_Palette*
IndexedCanvas::get_palette() const
{
  return const_cast<CL_Palette*>(&palette);
}


/* EOF */
