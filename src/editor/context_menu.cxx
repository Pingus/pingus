//  $Id: context_menu.cxx,v 1.00 2005/12/29 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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
#include "context_menu.hxx"
#include "level_objs.hxx"
#include "editor_viewport.hxx"
#include "../fonts.hxx"

namespace Pingus {

namespace Editor {

	// Determine which actions are available for these objects
	ContextMenu::ContextMenu(std::vector<LevelObj*> o, Vector p, EditorViewport* vp)
		: objs(o), 
			viewport(vp),
			pos(p),
			width(200),
			item_height(Fonts::pingus_small.get_height()),
			selected_action_offset(0)
	{
		actions.push_back("Remove");
		actions.push_back("ROT0");
		actions.push_back("ROT90");
		actions.push_back("ROT180");
		actions.push_back("ROT270");

		total_height  = item_height * actions.size();
	}


	ContextMenu::~ContextMenu()
	{
	}


	// Keep track of where the mouse is for highlighting
	void
	ContextMenu::on_pointer_move(int x, int y)
	{
		mouse_at.x = (float)x; 
		mouse_at.y = (float)y;

		selected_action_offset = (unsigned)((mouse_at.y - pos.y) / item_height);
	}

	void
	ContextMenu::draw(DrawingContext &gc)
	{
		// Draw the box
		gc.draw_fillrect(pos.x, pos.y, pos.x + 200, pos.y + total_height, 
			CL_Color(211,211,211,100));
		// Draw the border
		gc.draw_rect(pos.x, pos.y, pos.x + 200, pos.y + total_height, 
			CL_Color::black);
		// Draw the highlighted action if the mouse is in the box
		if (hover)
			gc.draw_fillrect(pos.x, pos.y + selected_action_offset * 
				item_height, pos.x + 200, pos.y + (selected_action_offset + 1) * item_height,
				CL_Color(128,128,128,100));

		// Draw the action names
		for (unsigned i = 0; i < actions.size(); i++)
			gc.print_left(Fonts::pingus_small, pos.x, pos.y + 
				(i * item_height), actions[i]);
	}

	bool
	ContextMenu::is_at(int x, int y)
	{
		return (x > pos.x && x < pos.x + 200 &&
			y > pos.y && y < pos.y + total_height);
	}

	void 
	ContextMenu::on_primary_button_click(int x, int y)
	{
		// FIXME: Call the correct object function based on the selected action.
		// FIXME: This is a temporary hack to test the functionality.
		for (unsigned i = 0; i < objs.size(); i++)
		{
			if (selected_action_offset == 0)
				objs[i]->remove();
			else
				objs[i]->set_modifier(actions[selected_action_offset]);
		}
		viewport->remove_context_menu();
	}

	void
	ContextMenu::on_secondary_button_click(int x, int y)
	{
		// Does the same as the primary button
		on_primary_button_click(x, y);
	}

}	// Editor namespace
} // Pingus namespace

/* EOF */

