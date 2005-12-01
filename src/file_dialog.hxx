//  $Id: file_dialog.hxx,v 1.0 2005/11/18 00:30:04 Jave27 Exp $
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

#ifndef HEADER_PINGUS_FILE_DIALOG_HXX
#define HEADER_PINGUS_FILE_DIALOG_HXX

#include <ClanLib/Display/sprite.h>
#include <vector>
#include <map>
#include <string>
#include "pingus_sub_menu.hxx"

namespace Pingus {

	class Vector;
	class FileDialogItem;
	class FileDialogOkButton;
	class PingusMenuManager;

class FileDialog : public PingusSubMenu
{
private:
  CL_Sprite sprite;

	/** The OK button - need to be able to hide and show it */
	FileDialogOkButton* ok_button;

	/** Should this dialog box be used for loading?  True for load, false for saving */
	bool for_loading;

	/** Mask for which files to display (*.pingus, *.xml, etc.) */
	std::string file_mask;

	/** Current path that is being displayed */
	std::string current_path;

	/** Current file that is selected */
	std::string current_file;

	/** Offset in the file_list that is the index of the first file/folder shown */
	unsigned current_offset;

	/** List of directories & files in the current folder */
	std::map<std::string, bool> file_list;

	/** List of files in the directory */
	std::vector<FileDialogItem*> file_dialog_items;

	/** Scroll the displayed file list */
	void offset_changed();

public:
	FileDialog (PingusMenuManager* manager, const std::string filemask_, 
		const std::string searchpath_, const bool for_load = true);
  ~FileDialog ();

  bool draw (DrawingContext& gc);

	void preload();

	/** Refresh the file list */
	void refresh();

	/** Return the current path in the dialog */
	std::string get_path() const { return current_path; }

	/** Return the file mask */
	std::string get_file_mask() const { return file_mask; }

	/** Sets the currently selected file name */
	void set_selected_file(std::string f);

	/** The Ok button has been pressed - either Save or Load this file */
	void ok_pressed();

	/** Need to scroll the list of files if possible */
	void scroll(int direction);

private:
  FileDialog (const FileDialog&);
  FileDialog& operator= (const FileDialog&);
};

} // namespace Pingus

#endif

/* EOF */
