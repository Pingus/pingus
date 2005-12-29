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

#include <ClanLib/Display/sprite.h>
#include <ClanLib/Core/IOData/directory_scanner.h>
#include <ClanLib/Core/IOData/inputsource_provider_file.h>
#include <ClanLib/Core/System/clanstring.h>
#include <ClanLib/Core/XML/dom_node.h>
#include <ClanLib/Core/XML/dom_node_list.h>
#include <ClanLib/Core/XML/dom_document.h>
#include <ClanLib/Core/XML/dom_element.h>
#include "pingus_error.hxx"
#include "xml_file_reader.hxx"
#include "file_dialog_item.hxx"
#include "file_dialog.hxx"
#include "vector.hxx"
#include "fonts.hxx"
#include "resource.hxx"
#include "gettext.h"

namespace Pingus {


	// Only Constructor
	FileDialogItem::FileDialogItem(FileDialog* f, Vector p) 
		: mouse_over(false), is_hidden(true), file_dialog(f),
			pos(p), sprite(Resource::load_sprite("core/misc/404sprite"))
	{
	}

	/** Set the current file assigned to this button */
	void 
	FileDialogItem::set_file(FileItem f) 
	{ 
		file_item = f;
		is_hidden = false;
		// Load sprite based on file  (folder icon, level screenshot, or generic)
		if (file_item.is_directory)
			sprite = Resource::load_sprite("core/menu/folder");
		else
		{
			// FIXME: Load thumbnail specific to this level
			sprite = Resource::load_sprite("core/menu/default_level");

			// Load information about this file if possible.
			CL_InputSourceProvider_File provider(".");
			CL_DomDocument doc(provider.open_source(file_dialog->get_path() 
				+ file_item.name), true);

			CL_DomElement root = doc.get_document_element();

			if (root.get_tag_name() == "pingus-level" || root.get_tag_name() == "pingus-worldmap")
			{
				CL_DomNodeList lst = root.get_child_nodes();

				for(int i = 0; i < lst.get_length(); ++i)
				{
					CL_DomElement node = lst.item(i).to_element();

					// Get information about this file
					if (node.get_tag_name() == "head")
					{
						XMLFileReader reader(node);
						if (root.get_tag_name() == "pingus-level")
						{
							// It's a level file
							reader.read_string("levelname",   file_item.friendly_name);
							reader.read_string("difficulty",  file_info);
							file_item.friendly_name = _(file_item.friendly_name);
							file_info = _("Difficulty: ") + file_info;
						}
						else
						{
							// It's a Worldmap
							// FIXME: Add more types eventually
							reader.read_string("name",        file_item.friendly_name);
							reader.read_string("description", file_info);
							file_item.friendly_name = _(file_item.friendly_name);
							file_info = _(file_info);

						}
						// Have to limit the size of the printed name
						file_item.friendly_name = file_item.friendly_name.substr(0, 23);
						break;
					}	// if XML Node == "head"
				}	// for loop
			}	// if XML tag_name was recognized
		}	// else it's a file, not a directory
	}	//set_file()

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

			// FIXME: If mouse over, draw a quick info box about the file item
			if (mouse_over)
			{
				if (file_item.is_directory)
					gc.draw_rect(pos.x, pos.y, pos.x + sprite.get_width(), 
						pos.y + sprite.get_height(), CL_Color(255,255,255,150));
				else		// It's a file
				{
					gc.draw_fillrect(pos.x+50, pos.y, pos.x+300, pos.y+50, CL_Color::azure);
					gc.print_left(Fonts::pingus_small, pos.x+50, pos.y, file_item.friendly_name);
					gc.print_left(Fonts::pingus_small, pos.x+50, pos.y+25, file_info);
				}
			}
		}
	}

	void
	FileDialogItem::on_primary_button_click (int x, int y)
	{
		file_dialog->set_selected_file(file_item);
	}

	void
	FileDialogItem::hide()
	{
		file_item.name = "";
		is_hidden = true;
	}

	std::string 
	FileDialogItem::get_filename() const 
	{
		if (file_item.is_directory)
			return file_item.name;
		else
			return file_item.name.substr(0, file_item.name.size() - file_dialog->get_file_mask().size());
	}
}

/* EOF */
