//  $Id: ScrollMap.cc,v 1.1 2000/08/10 15:48:02 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "ScrollMap.hh"
#include "../Display.hh"

Editor* ScrollMap::editor;

ScrollMap::ScrollMap()
{
  x_pos = CL_Display::get_width() - 200 - 1;
  y_pos = CL_Display::get_height() - 150 - 1;
  
  width = 200;
  height = 150;
}

ScrollMap::~ScrollMap()
{
}

void 
ScrollMap::draw()
{
  Display::draw_rect(x_pos, y_pos,
		     x_pos + width
		     y_pos + height);
}

/* EOF */
