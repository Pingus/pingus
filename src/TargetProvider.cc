//  $Id: TargetProvider.cc,v 1.1 2000/09/25 16:23:09 grumbel Exp $
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

#include "TargetProvider.hh"

CL_Canvas* 
TargetProvider::create_canvas (CL_Target* target)
{
  CL_Canvas* canvas;
  float r, g, b, a;

  canvas = new CL_Canvas (target->get_width (), target->get_height ());

  for (int y_pos = 0; y_pos < canvas->get_height (); y_pos++)
    for (int x_pos = 0; x_pos < canvas->get_width (); x_pos++)
      {
	target->get_pixel (x_pos, y_pos, &r, &g, &b, &a);
	canvas->draw_pixel (x_pos, y_pos, r, g, b);
      }
  
  return canvas;
}

/* EOF */
