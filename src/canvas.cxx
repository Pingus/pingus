//  $Id: canvas.cxx,v 1.1 2004/04/02 18:08:56 grumbel Exp $
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

#include <ClanLib/Display/pixel_format.h>
#include "canvas.hxx"

namespace Pingus {

CL_PixelBuffer* 
Canvas::create_rgba8888(int w, int h)
{
  return new CL_PixelBuffer(w, h, w*4,
                            CL_PixelFormat::rgba8888);
}

} // namespace Pingus

/* EOF */
