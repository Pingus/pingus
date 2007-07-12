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

#include "pingus_error.hpp"
#include "file_dialog_item.hpp"
#include "file_dialog.hpp"
#include "math/vector3f.hpp"
#include "fonts.hpp"
#include "resource.hpp"
#include "gettext.h"
#include "stat_manager.hpp"
#include "savegame_manager.hpp"
#include "savegame.hpp"
#include "worldmap/manager.hpp"
#include "worldmap/metamap.hpp"

	// Only Constructor
	FileDialogItem::FileDialogItem(FileDialog* f, Vector3f p) 
		: mouse_over(false), 
			is_hidden(true), 
			file_dialog(f),
			pos(p), 
			sprite(Resource::load_sprite("core/misc/404sprite"))
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
		{
			file_item.is_directory = true;
			sprite = Resource::load_sprite("core/menu/folder");
		}
		else
		{
			// FIXME: Load thumbnail specific to this level
			sprite = Resource::load_sprite("core/menu/default_level");
			file_item.is_accessible = true;
			file_item.is_finished = false;
#if 0

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
							file_item.is_accessible = true;
							file_item.friendly_name = _(file_item.friendly_name);
							file_info = _("Difficulty: ") + file_info;
							Savegame* sg = SavegameManager::instance()->get(file_item.friendly_name);
							if (sg)
							{
								status = _("Finished!");
								file_item.is_finished = (sg->status == Savegame::FINISHED);
							}
							else
							{
								status = _("Not finished");
								file_item.is_finished = false;
							}
						}
						else
						{
							// It's a Worldmap
							reader.read_string("name",        file_item.friendly_name);
							reader.read_string("short-name",  file_item.short_name);
							reader.read_string("description", file_info);

							// Get player's status for this worldmap
							if (!StatManager::instance()->get_bool(
								file_item.short_name + "-accessible", file_item.is_accessible))
							{
								status = _("Not Accessible");
								file_item.is_accessible = false;
							}
							else if (!StatManager::instance()->get_bool(
								file_item.short_name + "-finished", file_item.is_finished))
							{
								status = _("Not Finished");
								file_item.is_finished = false;
							}
							else
							{
								status = _("Finished!");
								file_item.is_finished = true;
							}
							file_item.friendly_name = _(file_item.friendly_name);
							file_info = _(file_info);

						}
						// Have to limit the size of the printed name
						file_item.friendly_name = file_item.friendly_name.substr(0, 23);
						break;
					}	// if XML Node == "head"
				}	// for loop
			}	// if XML tag_name was recognized
#endif
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

			// If mouse over, draw a quick info box about the file item
			if (mouse_over)
			{
				if (file_item.is_directory)
					gc.draw_rect(pos.x, pos.y, pos.x + sprite.get_width(), 
						pos.y + sprite.get_height(), Color(255,255,255,150));
				else		// It's a file
				{
					// Determine which color to draw the background rectangle.
					Color color;
					if (file_item.is_accessible)
						if (file_item.is_finished)
                                                  color = Color(123,123,0);////Color::azure;
						else
                                                  color = Color(123,0,222); ////Color::aqua;
					else
                                          color = Color(255,0,0);
					gc.draw_fillrect(pos.x+50, pos.y, pos.x+270, pos.y+75, color);

					// Draw level or worldmap information.
					gc.print_left(Fonts::pingus_small, pos.x+50, pos.y, file_item.friendly_name);
					gc.print_left(Fonts::pingus_small, pos.x+50, pos.y+25, file_info);
					gc.print_left(Fonts::pingus_small, pos.x+50, pos.y+50, status);
				}
			}
		}
	}

	void
	FileDialogItem::on_primary_button_click (int x, int y)
	{
		if (file_item.is_accessible)
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

/* EOF */
