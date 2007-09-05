//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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

#include "../gui/gui_screen.hpp"
#include "../file_dialog_listener.hpp"


class DrawingContext;
class FileDialog;
class GUI::GUIManager;

namespace Editor {

class EditorLevel;
class LevelObj;
class Panel;
class EditorViewport;

/** This class is the screen that contains all of the
    editor objects */
class EditorScreen : public GUIScreen, 
                     public FileDialogListener
{
private:
  /** The level currently being edited */
  EditorLevel* plf;

  /** Panel which contains all of the buttons for each action */
  Panel* panel;

  /** Viewport which holds all of the level images and data */
  EditorViewport* viewport;

  /** File Dialog box */
  FileDialog* filedialog;

  bool close_dialog;

public:
  /** Default constructor */
  EditorScreen();

  /** Destructor */
  ~EditorScreen();

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
  EditorLevel* get_level() const { return plf; }

  /** Add an object to both the EditorLevel and the EditorViewport */
  void add_object(LevelObj* obj);
  void add_objects(std::vector<LevelObj*> objs);

  /** Show a file dialog box */
  void show_file_dialog(bool for_loading);

  /** Closes the file dialog box */
  void cancel();

  /** Saves the currently loaded level */
  void save(const std::string &file);

  /** Load a new level */
  void load(const std::string &file);

  void load_level(const std::string& file);

  // Functions accessible from the GUI
  void level_new();
  void level_load();
  void level_save();
  void level_save_as();
  void level_play();

  void show_level_properties();
  void show_action_properties();
  void show_object_properties();

  void objects_delete();

  void objects_raise_to_top();
  void objects_raise();
  void objects_lower();
  void objects_lower_to_bottom();

  void objects_flip_vertical();
  void objects_flip_horizontal();

  void objects_rotate_left();
  void objects_rotate_right();

  void toggle_grid_snap();

  void toggle_object_selector();

  void toggle_help();

private:
  EditorScreen (const EditorScreen&);
  EditorScreen& operator= (const EditorScreen&);

};	// EditorScreen
}  // Editor namespace

#endif

/* EOF */
