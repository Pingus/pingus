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

#ifndef HEADER_PINGUS_EDITOR_VIEWPORT_HXX
#define HEADER_PINGUS_EDITOR_VIEWPORT_HXX

#include "../gui/rect_component.hpp"
#include "../graphic_context_state.hpp"
#include <boost/signal.hpp>
#include <vector>
#include <string>

class Vector3f;
class DrawingContext;
class SceneContext;

namespace Editor {

class LevelObj;
class EditorScreen;
class ContextMenu;

/** This class is where the actual level graphics will display in the
    level editor.  Objects can be added, deleted, moved, modified, etc. 
    inside of the EditorViewport */
class EditorViewport : public GUI::RectComponent 
{
private:	
  GraphicContextState state;
  DrawingContext* drawing_context;

  /** The EditorScreen to which this viewport belongs */
  EditorScreen* editor;

  /** Whether or not Autoscrolling is turned on */
  bool autoscroll;

  /** Where the mouse is right now - used for autoscrolling */
  Vector2i mouse_world_pos;
  Vector2i mouse_screen_pos;

  /** Where the mouse started dragging from */
  Vector2i drag_world_pos;
  Vector2i drag_screen_pos;

  /** All objects in the level */
  std::vector<LevelObj*> objs;

  /** The currently selected LevelObjs */
  std::vector<LevelObj*> selected_objs;

  /** The region that is currently highlighted */
  Rect highlighted_area;

  /** Returns the topmost object at this x, y location */
  LevelObj* object_at(int x, int y);

  /** There should only be 0 or 1 context menus on the screen */
  ContextMenu* context_menu;

  /** Whether or not the "snap-to-grid" functionality is on. */
  bool snap_to;

  /** What is the currently selected action that the mouse is doing */
  enum ActionType { NOTHING, HIGHLIGHTING, DRAGGING, SCROLLING } current_action;

public:
  /** Constructor
      @param e The EditorScreen to which this viewport belongs */
  EditorViewport(EditorScreen* e, const Rect& rect);

  /** Destructor */
  ~EditorViewport ();

  /** Draws all of the objects in the viewport */
  void draw(DrawingContext &gc);

  /** Update information about scrolling, etc. */
  void update(float delta);

  /** Returns whether or not the mouse is inside the viewport */
  bool is_at(int x, int y);

  /** Emitted when the pointer moved, x and y are the new pointer
      coordinates */
  void on_pointer_move (int x, int y);

  /** Refresh the list of objects (do when loading or creating a new level) */
  void refresh();
	
  /** Turns the "snap-to-grid" option on or off */
  void set_snap_to(bool s) { snap_to = s; }

  /** Add an object to the currently displayed vector of objects */
  void add_object(LevelObj* obj);

  /** Return a pointer to the EditorScreen object */
  EditorScreen* get_screen() { return editor; }

  /// Mouse actions
  void on_primary_button_press(int x, int y);
  void on_primary_button_release(int x, int y);

  void on_secondary_button_press(int x, int y);
  void on_secondary_button_release(int x, int y);

  void on_secondary_button_click(int x, int y);

  void on_key_pressed(const unsigned short c);

  void delete_selected_objects();
  void duplicate_selected_objects();

  void vflip_selected_objects();
  void hflip_selected_objects();

  void rotate_90_selected_objects();
  void rotate_270_selected_objects();

  void raise_object(LevelObj* obj);
  void lower_object(LevelObj* obj);

  void raise_object_to_top(LevelObj* obj);
  void lower_object_to_bottom(LevelObj* obj);

  void raise_objects();
  void lower_objects();

  void raise_objects_to_top();
  void lower_objects_to_bottom();

  Vector2i screen2world(int x, int y) const;

  void update_layout();

  std::vector<LevelObj*>* get_objects() { return &objs; }

  void clear_selection();
  void clear();

  boost::signal<void (const std::vector<LevelObj*>&)> selection_changed;
private:
  EditorViewport();
  EditorViewport (const EditorViewport&);
  EditorViewport& operator= (const EditorViewport&);
};

} // Editor namespace

#endif

/* EOF */
