// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_GUI_COMPONENT_HPP
#define HEADER_PINGUS_ENGINE_GUI_COMPONENT_HPP

#include "engine/input/event.hpp"

class DrawingContext;

namespace GUI {

class GroupComponent;

/** A component represents an area which recivies events in the GUI,
    some people might call it a widget */
class Component
{
private:
  GroupComponent* parent;
  bool focus;
  bool mouse_over;
  bool visible;

public:
  Component () : parent(0), focus(false), mouse_over(false), visible(true) { }
  virtual ~Component() {}

  virtual void set_focus(bool val) { focus = val; }
  virtual void set_mouse_over(bool val) { mouse_over = val; }

  virtual bool has_mouse_over() const { return mouse_over; }
  virtual bool has_focus() const { return focus; }

  virtual void hide() { visible = false; }
  virtual void show() { visible = true; }

  virtual bool is_visible() const { return visible; }

  virtual void draw (DrawingContext& gc) =0;
  virtual void update (float delta) { }

  virtual bool is_at (int x, int y) { return false; }

  // Events
  /** Gets issued once the primary button is pressed */
  virtual void on_primary_button_press (int x, int y) { }

  /** Gets issued once the primary button is released */
  virtual void on_primary_button_release (int x, int y) { }

  virtual void on_secondary_button_press (int x, int y) { }
  virtual void on_secondary_button_release (int x, int y) { }

  /** Gets emmited when a button is pressed and released over the
      same component */
  virtual void on_primary_button_click (int x, int y) {}

  virtual void on_secondary_button_click (int x, int y) {}

  /** Emmitted when pointer enters the region of the component */
  virtual void on_pointer_enter () {}

  /** Emmitted when pointer leaves the region of the component */
  virtual void on_pointer_leave () {}

  /** Emitted when the pointer moved, x and y are the new pointer
      coordinates */
  virtual void on_pointer_move (int x, int y) {}

  virtual void on_scroller_move (float x, float y) {}

  /** Emitted whenever a keyboard character is pressed.  Only certain
      components should implement this */
  virtual void on_key_pressed(const Input::KeyboardEvent& ev) {}

  virtual void on_text_input(const Input::TextInputEvent& ev) {}

  GroupComponent* get_parent() const;
  void set_parent(GroupComponent* p);

  void grab();
  void ungrab();

private:
  Component (const Component&);
  Component& operator= (const Component&);
};

} // namespace GUI

#endif

/* EOF */
