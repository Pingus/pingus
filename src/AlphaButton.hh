//  $Id: AlphaButton.hh,v 1.6 2002/06/07 09:42:16 torangan Exp $
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

#ifndef ALPHABUTTON_HH
#define ALPHABUTTON_HH

#include <string>

class CL_Font;

///
class AlphaButton
{
private:
  ///
  bool pressed;
  ///
  CL_Font* font;
  ///
  CL_Font* font_h;
  ///
  std::string name;
  ///
  int x1_pos, y1_pos, x2_pos, y2_pos;
public:
  ///
  AlphaButton(std::string str, int, int, int, int);
  ///
  bool is_pressed();
  ///
  void draw(void);
  ///
  bool mouse_over(void);
}///
;

#endif

/* EOF */
