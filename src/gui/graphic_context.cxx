//  $Id: graphic_context.cxx,v 1.5 2004/04/02 18:13:00 grumbel Exp $
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

#include <assert.h>
#include <iostream>
#include <ClanLib/Display/surface.h>
#include <ClanLib/Display/sprite.h>
#include "graphic_context.hxx"

namespace Pingus {

void
GraphicContext::draw (CL_Surface sur, const Vector& pos, int frame)
{
  draw(sur, int(pos.x), int(pos.y), frame);
}


void
GraphicContext::draw (CL_Surface sur, const Vector& pos)
{
  draw(sur, int(pos.x), int(pos.y));
}

} // namespace Pingus

/* EOF */
