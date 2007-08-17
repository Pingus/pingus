//  $Id$
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

#ifndef HEADER_PINGUS_FILE_DIALOG_ITEM_HXX
#define HEADER_PINGUS_FILE_DIALOG_ITEM_HXX

#include "gui/component.hpp"
#include "math/vector3f.hpp"
#include "sprite.hpp"
#include "file_dialog.hpp"


/** Class representing a clickable object in a File Dialog (a Directory, Level,
	or Worldmap */
class FileDialogItem : GUI::Component
{
private:
	bool mouse_over;
	bool is_hidden;

	/** Extra info about this file - differs based on file type (difficulty, etc.) */
	std::string file_info;
	std::string status;

	FileDialog* file_dialog;
	Vector3f pos;

	/** This file_item name will change based on the current file list */
	FileItem file_item;
	
	/** This sprite will change based on the currently displayed file */
	Sprite sprite;

public:
	// Only Constructor
	FileDialogItem(FileDialog* f, Vector3f p) ;

	/** Set the current file assigned to this button */
	void set_file(FileItem f);

	/** Get the cleaned up file name (no extension) */
	std::string get_filename() const;

	bool is_at(int x, int y);

	void draw (DrawingContext& gc);

	void hide ();

	void on_primary_button_click (int x, int y);

	/** Emmitted when pointer enters the region of the component */
	void on_pointer_enter () { mouse_over = true; }

	/** Emmitted when pointer leaves the region of the component */
	void on_pointer_leave () { mouse_over = false; }

private:
	FileDialogItem();
	FileDialogItem (const FileDialogItem&);
	FileDialogItem& operator= (const FileDialogItem&);
};	// FileDialogItem class

#endif

/* EOF */
