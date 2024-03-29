// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_LIST_HPP
#define HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_LIST_HPP

#include "editor/level_obj_ptr.hpp"
#include "engine/display/sprite.hpp"
#include "engine/gui/rect_component.hpp"

class DrawingContext;

namespace pingus::editor {

class LevelObj;
class EditorScreen;
class ObjectSelector;
class ObjectSelectorSet;

class ObjectSelectorList : public gui::RectComponent
{
public:
  struct Object {
    Sprite  sprite;
    Sprite  thumbnail;

    Object(Sprite const& sprite_, Sprite const& thumbnail_)
      : sprite(sprite_),
        thumbnail(thumbnail_)
    {
    }

    virtual ~Object() {}

    virtual LevelObjPtr create(Vector2i const&) { return LevelObjPtr(); }
  };

private:
  EditorScreen* editor;
  ObjectSelector* object_selector;
  DrawingContext* drawing_context;
  float    offset;
  float    old_offset;
  Vector2i drag_start;
  Vector2i mouse_pos;
  Vector2i real_mouse_pos;
  enum Mode { NOTHING, SCROLLING, OBJECT_DRAG };
  Mode mode;

  int current_object;
  int drag_object;

  ObjectSelectorSet* set;

public:
  ObjectSelectorList(EditorScreen* editor_, ObjectSelector* object_selector_, Rect const& rect);
  ~ObjectSelectorList() override;

  void on_primary_button_press (int x, int y) override;
  void on_primary_button_release (int x, int y) override;

  void on_secondary_button_press (int x, int y) override;
  void on_secondary_button_release (int x, int y) override;

  void on_pointer_move (int x, int y) override;

  void draw(DrawingContext& gc) override;

  void set_objects(ObjectSelectorSet* object_set);

  void scroll(float y);

  void update_layout() override;
private:
  ObjectSelectorList (ObjectSelectorList const&);
  ObjectSelectorList& operator= (ObjectSelectorList const&);
};

} // namespace pingus::editor

#endif

/* EOF */
