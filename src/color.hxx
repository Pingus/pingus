//  $Id: color.hxx,v 1.8 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_COLOR_HXX
#define HEADER_PINGUS_COLOR_HXX

#include "pingus.hxx"
#include <iostream>

namespace Pingus {

class Color
{
public:
  float red;
  float green;
  float blue;
  float alpha;

  Color () : red(0.0), green(0.0), blue(0.0), alpha(1.0)
  {
  }

  Color (float red_, float green_, float blue_, float alpha_ = 1.0) : red(red_), green(green_), blue(blue_), alpha(alpha_)
  {
  }

  Color (const Color& old): red(old.red), green(old.green), blue(old.blue), alpha(old.alpha)
  {
  }

  Color& operator= (const Color& old)
  {
    if (this == &old)
      return *this;

    red   = old.red;
    green = old.green;
    blue  = old.blue;
    alpha = old.alpha;

    return *this;
  }

  bool operator!= (const Color& color) const {
    return !operator==(color);
  }

  bool operator== (const Color& color) const {
    return red == color.red && green == color.green && blue == color.blue && alpha == color.alpha;
  }

};

} // namespace Pingus

std::ostream& operator<<(std::ostream& s, const Pingus::Color& color);

#endif

/* EOF */
