//  $Id: editor.hxx,v 1.21 2002/11/05 03:02:48 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_EDITOR_HXX
#define HEADER_PINGUS_EDITOR_EDITOR_HXX

#include <string>
#include <vector>
#include <ClanLib/Signals/slot.h>

#include "../screen.hxx"
#include "editor_help_screen.hxx"

class CL_StyleManager_Default;
class CL_GUIManager;

class EditorObj;

namespace EditorNS {

class EditorView;
class EditorEvent;
class Panel;
class ScrollMap;
class ObjectManager;
class Selection;
class ObjectSelector;
class StatusLine;
class ActionWindow;
class PropertyWindow;
class LevelPropertyWindow;

class Editor : public Screen
{
private:
  EditorEvent* event;
  friend class EditorEvent;

  CL_Slot on_button_press_slot;
  CL_Slot on_button_release_slot;
  
  int event_handler_ref_counter;

  CL_Font* font;

  int move_x;
  int move_y;
  std::string checkpoint;
  std::string last_level;
  bool always_animate;

  bool show_help_screen;
  EditorHelpScreen help_screen;

  Panel*  panel;
  ScrollMap* scroll_map;
  ObjectManager* object_manager;
  Selection* selection;
  EditorView* view; 
  StatusLine* status_line;
  ObjectSelector* object_selector;
  
  // GUI Stuff
  CL_StyleManager_Default* style_manager;
  CL_GUIManager* gui;

  ActionWindow*   action_window;

  EditorNS::LevelPropertyWindow* level_property_window;
  EditorNS::PropertyWindow* property_window;

  enum { SELECTOR_TOOL, ZOOM_TOOL } tool;

  static Editor* instance_;

  std::vector<EditorObj*> tmp_selection;

  Editor ();
public:
  virtual ~Editor ();
  
  static Editor* instance ();
  static void init();
  static void deinit();

  std::string read_string (const std::string & prefix = "", const std::string & default_str = "");

  void draw_noflip();
  void interactive_move_object();
  void move_objects();

  void rect_get_current_objs();

  bool mouse_moved();
  void scroll();
  void zoom_mode ();
  void load_level (const std::string& str);

  void toggle_help_screen ();

  ObjectManager* get_object_manager() { return object_manager; }
  std::string save_tmp_level ();
  EditorEvent* get_event() { return event; }

  CL_GUIManager* get_gui_manager ();

  // overloaded Screen stuff
  bool draw (GraphicContext& gc);
  void draw ();
  void update (const GameDelta& delta);

  void on_startup ();
  void on_shutdown ();
  
private:
  Editor (const Editor&);
  Editor& operator= (const Editor&);
};

} // namespace EditorNS

#endif

/* EOF */




