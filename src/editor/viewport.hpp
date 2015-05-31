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

#ifndef HEADER_PINGUS_EDITOR_VIEWPORT_HPP
#define HEADER_PINGUS_EDITOR_VIEWPORT_HPP

#include <boost/signals2.hpp>
#include <set>

#include "editor/selection.hpp"
#include "editor/editor_level.hpp"
#include "engine/display/graphic_context_state.hpp"
#include "engine/gui/rect_component.hpp"

class DrawingContext;
class SceneContext;

namespace Editor {

class LevelObj;
class EditorScreen;
class ContextMenu;

/** This class is where the actual level graphics will display in the
    level editor.  Objects can be added, deleted, moved, modified, etc.
    inside of the Viewport */
class Viewport : public GUI::RectComponent
{
private:
  GraphicContextState state;
  std::unique_ptr<DrawingContext> drawing_context;

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

  /** The currently selected LevelObjs */
  Selection selection;

  /** The region that is currently highlighted */
  Rect highlighted_area;

  /** There should only be 0 or 1 context menus on the screen */
  ContextMenu* context_menu;

  /** Whether or not the "snap-to-grid" functionality is on. */
  bool snap_to;

  /** What is the currently selected action that the mouse is doing */
  enum ActionType { NOTHING, HIGHLIGHTING, DRAGGING, SCROLLING } current_action;

  std::vector<Color> m_background_colors;
  std::vector<Color>::size_type  m_background_colors_idx;

public:
  /** Constructor
      @param e The EditorScreen to which this viewport belongs */
  Viewport(EditorScreen* e, const Rect& rect);

  /** Destructor */
  ~Viewport ();

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

  Vector2i get_scroll_pos() const;
  void     set_scroll_pos(const Vector2i& pos);

  /// Mouse actions
  void on_primary_button_press(int x, int y);
  void on_primary_button_release(int x, int y);

  void on_secondary_button_press(int x, int y);
  void on_secondary_button_release(int x, int y);

  void on_secondary_button_click(int x, int y);

  void on_key_pressed(const Input::KeyboardEvent& ev);

  void delete_selected_objects();
  void duplicate_selected_objects();

  void vflip_selected_objects();
  void hflip_selected_objects();

  void rotate_90_selected_objects();
  void rotate_270_selected_objects();

  void raise_objects();
  void lower_objects();

  void raise_objects_to_top();
  void lower_objects_to_bottom();

  void raise_objects_z_pos();
  void lower_objects_z_pos();

  void group_selection();
  void ungroup_selection();

  void move_objects(const Vector2i& offset);

  Vector2i screen2world(int x, int y) const;

  void update_layout();

  EditorLevel::Objects* get_objects();

  void clear_selection();

  boost::signals2::signal<void (const Selection&)> selection_changed;
private:
  Viewport();
  Viewport (const Viewport&);
  Viewport& operator= (const Viewport&);
};

} // Editor namespace

#endif

/* EOF */
