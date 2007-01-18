//  $Id: input_box.cxx,v 1.16 2006/1/21 21:37:06 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
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

#include <algorithm>
#include "input_box.hpp"
#include "../display/drawing_context.hpp"
#include "../math/vector3f.hpp"
#include "../fonts.hpp"

namespace GUI {
	
InputBox::InputBox(float width_, Vector3f p, const std::string& default_value,
                   bool locked, const std::string& label_) 
	:	str(default_value),
	pos(p),
	width(width_),
	height((float)Fonts::pingus_small.get_height()),
	is_locked(locked),
  label(label_) 
{
	
}

void
InputBox::draw(DrawingContext &gc)
{
	// Draw the rectangle and border
	gc.draw_fillrect(pos.x, pos.y, pos.x + width, pos.y + height, 
                         Color(0,0,0));
	gc.draw_rect(pos.x, pos.y, pos.x + width, pos.y + height, Color(255,255,255));	

	// If there is text, draw it:
	if (str != std::string())
		gc.print_left(Fonts::pingus_small, pos.x + 10, pos.y, shrink_string(str));
	
  if (label != std::string())
    gc.print_right(Fonts::pingus_small, pos.x, pos.y, label);
     
	if (has_focus)
		gc.draw_line(pos.x + Fonts::pingus_small.get_size(shrink_string(str)).width + 12,
			pos.y, pos.x + Fonts::pingus_small.get_size(shrink_string(str)).width + 12, 
                             pos.y + height,	Color(0,255,255));
}

bool
InputBox::is_at(int x, int y)
{
	return (x > pos.x && x < pos.x + width &&
		y > pos.y && y < pos.y + height);
}

std::string
InputBox::shrink_string(const std::string& s) const
{
	std::string ret_string;
	int w = (int)width / Fonts::pingus_small.get_width('W');
	
	if ((int)s.length() > w - 1)
		ret_string = s.substr(std::max(0, (int)s.length()-w), w - 1);
	else
		ret_string = s;
	
	return ret_string;
}

void
InputBox::on_key_pressed(const char c)
{
	if (!is_locked)
	{
		// Verify input and escape out bad characters
		if (c == 0x08) 	// backspace
			str = str.substr(0, str.length()-1);
		else if ((c > 0x2c && c < 0x3a)    // - . / 0-9
			|| (c > 0x40 && c < 0x5b)        // capital letters
			|| (c > 0x60 && c < 0x7b)        // lowercase letters
      || (c == 0x7e || c == 0x5f)      // ~ and _
      ||  c == 0x20)                   // space
			str += c;
	}
}

}	// GUI

/* EOF */
