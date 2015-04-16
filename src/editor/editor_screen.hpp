// Pingus - A free Lemmings clone
// Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                    Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_EDITOR_EDITOR_SCREEN_HPP
#define HEADER_PINGUS_EDITOR_EDITOR_SCREEN_HPP

#include "editor/file_dialog.hpp"
#include "editor/editor_level.hpp"
#include "engine/screen/gui_screen.hpp"
#include "util/pathname.hpp"

class DrawingContext;
class FileDialog;
class Pathname;

namespace Editor {

class ActionProperties;
class EditorLevel;
class LevelObj;
class LevelProperties;
class MessageBox;
class Minimap;
class ObjectProperties;
class ObjectSelector;
class Panel;
class Viewport;

/** This class is the screen that contains all of the
    editor objects */
class EditorScreen : public GUIScreen
{
private:
  std::unique_ptr<EditorLevel> plf;
  Pathname level_pathname;

  Panel* panel;
  Viewport*   viewport;
  ObjectSelector*   object_selector;
  Minimap*          minimap;
  ObjectProperties* object_properties;
  ActionProperties* action_properties;
  LevelProperties*  level_properties;

  FileDialog*       file_load_dialog;
  FileDialog*       file_save_dialog;

  MessageBox* m_level_new_msgbox;

  bool show_help;

public:
  /** Default constructor */
  EditorScreen();

  /** Destructor */
  ~EditorScreen();

  /** Runs when the editor becomes the active screen */
  void on_startup();

  /** Closes the current screen */
  void close_screen();

  /** Code that runs when the Escape button is pressed */
  void on_escape_press();
  void on_action_up_press();
  void on_action_down_press();

  /** Draw the items in the screen */
  void draw (DrawingContext& gc);

  /** Update the GUI objects */
  void update(const Input::Event& event);

  /** Return the gui_manager */
  GUI::GUIManager* get_gui_manager() const { return gui_manager.get(); }

  /** Return the viewport */
  Viewport* get_viewport() const { return viewport; }

  void set_level(std::unique_ptr<EditorLevel> level);

  /** Return a pointer to the current level */
  EditorLevel* get_level() const { return plf.get(); }

  /** Show a file dialog box */
  void show_file_dialog(bool for_loading);

  /** Closes the file dialog box */
  void cancel();

  /** Saves the currently loaded level */
  void save(const Pathname& file);

  /** Load a new level */
  void load(const Pathname& file);

  // Functions accessible from the GUI
  void level_new_without_confirm();
  void level_new();
  void level_load();
  void level_save();
  void level_save_as();
  void level_play();

  void objects_duplicate();
  void objects_delete();

  void objects_raise_to_top();
  void objects_raise();
  void objects_lower();
  void objects_lower_to_bottom();

  void objects_flip_vertical();
  void objects_flip_horizontal();

  void objects_rotate_left();
  void objects_rotate_right();

  void toggle_object_selector();

  void toggle_object_properties();
  void toggle_action_properties();
  void toggle_level_properties();
  void toggle_minimap();

  void toggle_help();

  void exit();

  void resize(const Size&);

  void update_layout();

private:
  EditorScreen (const EditorScreen&);
  EditorScreen& operator= (const EditorScreen&);
};

} // namespace Editor

#endif

/* EOF */
