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

#ifndef HEADER_PINGUS_FILE_DIALOG_ITEM_HXX
#define HEADER_PINGUS_FILE_DIALOG_ITEM_HXX

#include <ClanLib/Display/sprite.h>
#include "gui/component.hxx"
#include "vector.hxx"
#include "file_dialog.hxx"

namespace Pingus {

/** Class representing a clickable object in a File Dialog (a Directory, Level,
	or Worldmap */
class FileDialogItem : GUI::Component
{
private:
	bool mouse_over;
	bool is_hidden;

	std::string friendly_name;
	std::string file_info;

	FileDialog* file_dialog;
	Vector pos;

	/** This file_item name will change based on the current file list */
	FileItem file_item;
	
	/** This sprite will change based on the currently displayed file */
	CL_Sprite sprite;

public:
	// Only Constructor
	FileDialogItem(FileDialog* f, Vector p) ;

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

}		// Pingus namespace

#endif

/* EOF */
