//  $Id: checkbox.cxx,v 1.00 2006/1/15 23:41:12 Jave27 Exp $
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

#include <ClanLib/Display/sprite.h>
#include "checkbox.hxx"
#include "../fonts.hxx"
#include "../vector.hxx"
#include "../resource.hxx"
#include "../display/drawing_context.hxx"
#include "../gui/checkbox_listener.hxx"

namespace Pingus {

namespace GUI {
	
Checkbox::Checkbox(Vector p, std::string label_, CheckboxListener* l) :
	checkmark(Resource::load_sprite("core/misc/checkbox_clicked")),
	is_checked(false),
	pos(p),
	width(20),
	height(20),
	listener(l),
	label(label_)
{
}
	
void 
Checkbox::draw(DrawingContext& gc)
{
	gc.draw_rect(pos.x, pos.y, pos.x + width, pos.y + height, 
		CL_Color::black);
	if (is_checked)
		gc.draw(checkmark, pos);
	
	gc.print_right(Fonts::smallfont, (int)pos.x, (int)pos.y, label);
}

bool
Checkbox::is_at(int x, int y)
{
	return (x > pos.x && x < pos.x + width &&
		y > pos.y && y < pos.y + height);
}

void 
Checkbox::on_primary_button_click(int x, int y)
{
	is_checked = !is_checked;
	listener->checkbox_changed(is_checked, this);
}

}	// GUI namespace
}	// Pingus namespace

/* EOF */
