//  $Id: alpha_button.cxx,v 1.7 2003/10/21 11:01:52 grumbel Exp $
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

#include <ClanLib/Display/display.h>
#include <ClanLib/Display/font.h>
#include <ClanLib/Display/mouse.h>
#include <ClanLib/Display/keys.h>
#include "fonts.hxx"
#include "alpha_button.hxx"

namespace Pingus {

AlphaButton::AlphaButton(std::string str, int x1, int y1, int w, int h) :
    pressed(false),
    font  (Fonts::smallfont_h),
    font_h(Fonts::smallfont),
    name  (str),
    x1_pos(x1),
    y1_pos(y1),
    x2_pos(x1 + w - 1),
    y2_pos(y1 + h - 1)
{
}

void
AlphaButton::draw(void)
{
  CL_Font font = font_h;

  font.set_alignment(origin_top_center);

  if (is_pressed()) {
    CL_Display::fill_rect(CL_Rect(x1_pos, y1_pos, x2_pos, y2_pos),
			  CL_Color(255, 255, 255, 128));
    font.draw(x1_pos + (x2_pos - x1_pos)/2, y1_pos + (y2_pos - y1_pos)/2,
			 name);
  } else {
    CL_Display::fill_rect(CL_Rect(x1_pos, y1_pos, x2_pos, y2_pos),
			  CL_Color(0, 0, 0, 128));
    font.draw(x1_pos + (x2_pos - x1_pos)/2, y1_pos + (y2_pos - y1_pos)/2,
              name);
  }
}

bool
AlphaButton::is_pressed()
{
  if (mouse_over() && CL_Mouse::get_keycode(CL_MOUSE_LEFT)) {
    return true;
  } else {
    return false;
  }
}

bool
AlphaButton::mouse_over(void)
{
  if (   CL_Mouse::get_x() > x1_pos
      && CL_Mouse::get_x() < x2_pos
      && CL_Mouse::get_y() > y1_pos
      && CL_Mouse::get_y() < y2_pos)
    {
      return true;
    }
  else
    {
      return false;
    }
}

} // namespace Pingus

/* EOF */
