//  $Id: indexed_canvas.cxx,v 1.1 2002/10/12 00:24:26 grumbel Exp $
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

#include "indexed_canvas.hxx"

IndexedCanvas::IndexedCanvas(int width, int height)
{
  data = new unsigned char [width * height];
  new IndexedCanvas(10, 10);
}

IndexedCanvas::~IndexedCanvas()
{
  delete data;
}

void
IndexedCanvas::set_palette(CL_Palette* p)
{
  palette = *p;
}

CL_Palette*
IndexedCanvas::get_palette() const
{
  return const_cast<CL_Palette*>(&palette);
}

/* EOF */
