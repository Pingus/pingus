//  $Id: editor_screen.hxx,v 1.00 2005/11/07 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOR_SCREEN_HXX
#define HEADER_PINGUS_EDITOR_SCREEN_HXX

#include "../gui/gui_screen.hxx"
#include "../file_dialog_listener.hxx"

namespace Pingus {

class DrawingContext;
class FileDialog;
class GUI::GUIManager;

namespace Editor {

class XMLLevel;
class LevelObj;
class EditorPanel;
class EditorViewport;

/** This class is the screen that contains all of the
	editor objects */
class EditorScreen : public GUIScreen, public FileDialogListener
{
private:
	/** The level currently being edited */
	XMLLevel* plf;

	/** Panel which contains all of the buttons for each action */
	EditorPanel* panel;

	/** Viewport which holds all of the level images and data */
	EditorViewport* viewport;

	/** File Dialog box */
	FileDialog* filedialog;

	bool close_dialog;

public:
	/** Default constructor */
  EditorScreen(XMLLevel* level = 0);

	/** Destructor */
	~EditorScreen();

	/** Code that runs when the screen first opens */
  void on_startup();

	/** Closes the current screen */
  void close_screen();

	/** Code that runs when the Escape button is pressed */
  void on_escape_press();

	/** Draw the items in the screen */
	bool draw (DrawingContext& gc);

	/** Update the GUI objects */
	void update (const GameDelta& delta);

	/** Return the gui_manager */
	GUI::GUIManager* get_gui_manager() const { return gui_manager; }
	
	/** Return the viewport */
	EditorViewport* get_viewport() const { return viewport; }

	/** Return a pointer to the current level */
	XMLLevel* get_level() const { return plf; }

	/** Add an object to both the XMLLevel and the EditorViewport */
	void add_object(LevelObj* obj);
	void add_objects(std::vector<LevelObj*> objs);

	/** Show a file dialog box */
	void show_file_dialog(bool for_loading);

	/** Closes the file dialog box */
	void cancel();

	/** Saves the currently loaded level */
	void save(const std::string &file, const std::string &filemask);

	/** Load a new level */
	void load(const std::string &file, const std::string &filemask);

	/** Plays the currently loaded level */
	void play_level();

private:
  EditorScreen (const EditorScreen&);
  EditorScreen& operator= (const EditorScreen&);

};	// EditorScreen
}		// Editor namespace
}		// Pingus namespace

#endif

/* EOF */
