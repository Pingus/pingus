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

#include "editor/object_selector_list.hpp"

#include <logmich/log.hpp>

#include "editor/editor_level.hpp"
#include "editor/editor_screen.hpp"
#include "editor/object_selector.hpp"
#include "editor/object_selector_set.hpp"
#include "editor/viewport.hpp"

namespace pingus::editor {

ObjectSelectorList::ObjectSelectorList(EditorScreen* editor_, ObjectSelector* object_selector_, Rect const& rect_) :
  RectComponent(rect_),
  editor(editor_),
  object_selector(object_selector_),
  drawing_context(new DrawingContext(rect, true)),
  offset(0),
  old_offset(0),
  drag_start(),
  mouse_pos(),
  real_mouse_pos(),
  mode(NOTHING),
  current_object(-1),
  drag_object(-1),
  set(nullptr)
{
}

ObjectSelectorList::~ObjectSelectorList()
{
  delete drawing_context;
}

void
ObjectSelectorList::draw(DrawingContext& parent_gc)
{
  DrawingContext& gc = *drawing_context;
  gc.clear();
  gc.fill_screen(Color(100, 100, 100));

  if (set)
  {
    gc.push_modelview();
    gc.translate(0, static_cast<int>(offset));

    for(auto i = set->get_objects().begin(); i != set->get_objects().end(); ++i)
    {
      int x = static_cast<int>(i - set->get_objects().begin()) % 5;
      int y = static_cast<int>(i - set->get_objects().begin()) / 5;

      gc.draw((*i)->thumbnail, Vector2i(x * 48, y * 48));

      gc.draw_rect(Rect(x * 48,      y * 48,
                        x * 48 + 48, y * 48 + 48),
                   Color(155,155,155));

      if (has_mouse_over() && current_object != -1 && (i - set->get_objects().begin()) == current_object)
      {
        gc.draw_fillrect(Rect(x * 48,      y * 48,
                              x * 48 + 48, y * 48 + 48),
                         Color(255,255,255, 100));

        gc.draw_rect(Rect(x * 48,      y * 48,
                          x * 48 + 48, y * 48 + 48),
                     Color(255,255,255));
      }
    }

    gc.pop_modelview();
  }
  parent_gc.draw(gc);

  if (set && mode == OBJECT_DRAG)
  {
    Sprite sprite = set->get_objects()[static_cast<size_t>(current_object)]->sprite;
    parent_gc.draw(sprite,
                   real_mouse_pos
                   - geom::ioffset(sprite.get_width()/2, sprite.get_height()/2)
                   + geom::ioffset(sprite.get_offset().as_vec()),
                   2000.0f);
  }
}

void
ObjectSelectorList::on_primary_button_press (int x, int y)
{
  if (!set) return;

  if (mode == NOTHING && current_object != -1)
  {
    drag_object = current_object;
    mode = OBJECT_DRAG;
  }
}

void
ObjectSelectorList::on_primary_button_release (int x, int y)
{
  if (!set) return;

  if (mode == OBJECT_DRAG)
  {
    mode = NOTHING;

    if (current_object != -1)
    {
      // FIXME: Should check if the current mouse over component
      // is the Viewport, else no drag should take place, this
      // checks if the current mouse_over_comp is the
      // ObjectSelector, which is good enough but not perfect
      if (!geom::contains(object_selector->get_rect(), geom::ipoint(x + object_selector->get_rect().left(),
                                                                    y + object_selector->get_rect().top())))
      {
        Vector2i p = editor->get_viewport()->screen2world(x + object_selector->get_rect().left(),
                                                          y + object_selector->get_rect().top());

        // place object with left/top instead of center origin
        p -= geom::ioffset(set->get_objects()[static_cast<size_t>(current_object)]->sprite.get_width()/2,
                           set->get_objects()[static_cast<size_t>(current_object)]->sprite.get_height()/2);

        p += geom::ioffset(set->get_objects()[static_cast<size_t>(current_object)]->sprite.get_offset().as_vec());

        LevelObjPtr obj = set->get_objects()[static_cast<size_t>(current_object)]->create(p);
        if (obj)
        {
          editor->get_level()->add_object(obj);
        }
        else
        {
          log_error("ObjectSelectorList::Object: create() not implemented");
        }
      }
    }
  }
}

void
ObjectSelectorList::on_secondary_button_press (int x, int y)
{
  if (!set) return;

  if (mode == NOTHING)
  {
    drag_start = Vector2i(x,y);
    mode = SCROLLING;
    old_offset = offset;
  }
}

void
ObjectSelectorList::on_secondary_button_release (int x, int y)
{
  if (!set) return;

  if (mode == SCROLLING)
    mode = NOTHING;
}

void
ObjectSelectorList::on_pointer_move (int x, int y)
{
  if (!set) return;

  real_mouse_pos = Vector2i(x, y);

  mouse_pos = Vector2i(x - rect.left(), y - rect.top());

  int width = 5;
  int height = (static_cast<int>(set->get_objects().size()) / width) +
    ((static_cast<int>(set->get_objects().size()) % width > 0) ? 1 : 0);

  if (mode != OBJECT_DRAG)
  {
    if (!set->get_objects().empty())
    {
      int obj_x = std::clamp(mouse_pos.x() / 48, 0, width - 1);
      int obj_y = (mouse_pos.y() - static_cast<int>(offset)) / 48;

      current_object = (obj_y * 5) + obj_x;

      if (obj_y < 0 ||
          obj_y >= height)
      {
        current_object = -1;
      }
      else if (current_object < 0 ||
          current_object >= int(set->get_objects().size()))
      {
        current_object = -1;
      }
    }
  }

  if (mode == SCROLLING)
  {
    offset = old_offset + static_cast<float>(y - drag_start.y());
    offset = std::clamp(offset, std::min(static_cast<float>(rect.height()) - (static_cast<float>(height) * 48.0f), 0.0f), 0.0f);
  }
}

void
ObjectSelectorList::scroll(float y)
{
  if (set)
  {
    offset += y;

    // FIXME: move this to a separate function
    int width = 5;
    int height = (static_cast<int>(set->get_objects().size()) / width) +
      ((static_cast<int>(set->get_objects().size()) % width > 0) ? 1 : 0);
    offset = std::clamp(offset,
                        std::min(static_cast<float>(rect.height()) - (static_cast<float>(height) * 48.0f), 0.0f),
                        0.0f);
  }
}

void
ObjectSelectorList::set_objects(ObjectSelectorSet* new_set)
{
  if (set)
    set->set_offset(offset);

  if (new_set)
    offset = new_set->get_offset();
  else
    offset = 0.0f;

  set = new_set;

  current_object = -1;
}

void
ObjectSelectorList::update_layout()
{
  drawing_context->set_rect(rect);
}

} // namespace pingus::editor

/* EOF */
