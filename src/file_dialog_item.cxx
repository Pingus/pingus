//  $Id: file_dialog.hxx,v 1.0 2005/11/28 00:30:04 Jave27 Exp $
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

#include <Clanlib/Display/sprite.h>
#include <ClanLib/Core/IOData/directory_scanner.h>
#include "file_dialog_item.hxx"
#include "file_dialog.hxx"
#include "vector.hxx"
#include "fonts.hxx"
#include "resource.hxx"

namespace Pingus {


	// Only Constructor
	FileDialogItem::FileDialogItem(FileDialog* f, Vector p) 
		: file_dialog(f), mouse_over(false), is_hidden(true),
			pos(p), sprite(Resource::load_sprite("core/misc/404sprite"))
	{
	}

	/** Set the current file assigned to this button */
	void 
	FileDialogItem::set_file(const std::string f, bool is_dir) 
	{ 
		file_item = f;
		is_directory = is_dir;
		is_hidden = false;
		// Load sprite based on file  (folder icon, level screenshot, or generic)
		if (is_directory)
			sprite = Resource::load_sprite("core/menu/folder");
		else
			sprite = Resource::load_sprite("core/menu/default_level");
	}

	bool 
	FileDialogItem::is_at(int x, int y)
	{
		if (is_hidden)
			return false;
		return (x > pos.x && x < pos.x + sprite.get_width()
			&& y > pos.y && y < pos.y + sprite.get_height());
	}

	void 
	FileDialogItem::draw (DrawingContext& gc)
	{
		if (!is_hidden)
		{
			// Draw thumbnail
			gc.draw(sprite, pos);

			// Draw title
			gc.print_left(Fonts::pingus_small, pos.x + (float)sprite.get_width(), 
				pos.y, get_filename());

			// FIXME: If mouse over, draw a quick info box about the level
			if (mouse_over)
			{
			}
		}
	}

	void
	FileDialogItem::on_primary_button_click (int x, int y)
	{
		file_dialog->set_selected_file(file_item);
		// Change immediately to the folder if clicked on one
		if (is_directory)
			file_dialog->ok_pressed();
	}

	void
	FileDialogItem::hide()
	{
		file_item = "";
		is_hidden = true;
	}

	std::string 
	FileDialogItem::get_filename() const 
	{
		if (is_directory)
			return file_item;
		else
			return file_item.substr(0, file_item.size() - file_dialog->get_file_mask().size());
	}
}

/* EOF */