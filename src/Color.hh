//  $Id: Color.hh,v 1.1 2000/08/04 19:31:27 grumbel Exp $
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

#ifndef COLOR_HH
#define COLOR_HH

class Color
{
public:
  float red;
  float green;
  float blue;
  float alpha;
 
  Color() {
    red = green = blue = 0.0;
      alpha = 1.0;
  }
 
  Color(float red, float green, float blue, float alpha = 1.0) {
    this->red   = red;
    this->green = green;
    this->blue  = blue;
    this->alpha = alpha;
  }
};

#endif

/* EOF */
