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

#include "input_box.hxx"
#include "../display/drawing_context.hxx"
#include "../vector.hxx"
#include "../fonts.hxx"

namespace Pingus {
namespace GUI {
	
InputBox::InputBox(float width_, Vector p, const std::string& default_value) 
	:	str(default_value),
	pos(p),
	width(width_),
	height(Fonts::pingus_small.get_height())
{
	
}

void
InputBox::draw(DrawingContext &gc)
{
	// Draw the rectangle and border
	gc.draw_fillrect(pos.x, pos.y, pos.x + width, pos.y + height, CL_Color::azure);
	gc.draw_rect(pos.x, pos.y, pos.x + width, pos.y + height, CL_Color::black);	

	// If there is text, draw it:
	if (str != std::string())
		gc.print_left(Fonts::pingus_small, pos.x, pos.y, str);
}

bool
InputBox::is_at(int x, int y)
{
	return (x > pos.x && x < pos.x + width &&
		y > pos.y && y < pos.y + height);
}

}	// GUI
}	// Pingus

/* EOF */
