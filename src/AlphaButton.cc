//  $Id: AlphaButton.cc,v 1.6 2002/06/10 11:00:25 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Input/mouse.h>
#include "PingusResource.hh"

#include "AlphaButton.hh"

AlphaButton::AlphaButton(std::string str, int x1, int y1, int w, int h)
{
  name = str;
  x1_pos = x1;
  y1_pos = y1;
  x2_pos = x1 + w - 1;
  y2_pos = y1 + h - 1;
 
  font_h = PingusResource::load_font("Fonts/smallfont", "fonts");
  font   = PingusResource::load_font("Fonts/smallfont_h", "fonts");
}

void
AlphaButton::draw(void)
{
  if (is_pressed()) {
    CL_Display::fill_rect(x1_pos, y1_pos, x2_pos, y2_pos,
			  1.0, 1.0, 1.0, 0.5);
    font_h->print_center(x1_pos + (x2_pos - x1_pos)/2, y1_pos + (y2_pos - y1_pos)/2,
			 name.c_str());
  } else {
    CL_Display::fill_rect(x1_pos, y1_pos, x2_pos, y2_pos,
			  0.0, 0.0, 0.0, 0.5);
    font->print_center(x1_pos + (x2_pos - x1_pos)/2, y1_pos + (y2_pos - y1_pos)/2,
		       name.c_str());
  }
}

bool 
AlphaButton::is_pressed()
{
  if (mouse_over() && CL_Mouse::left_pressed()) {
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

/* EOF */
