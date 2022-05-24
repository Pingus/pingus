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

#include "editor/minimap.hpp"

#include "editor/editor_level.hpp"
#include "editor/editor_screen.hpp"
#include "editor/gui_style.hpp"
#include "editor/level_obj.hpp"
#include "editor/viewport.hpp"

namespace pingus::editor {

Minimap::Minimap(EditorScreen* editor_, const Rect& rect_)
  : RectComponent(rect_),
    editor(editor_),
    drawing_context(new DrawingContext(geom::grow(rect, -3))),
    dragging(false)
{
}

Minimap::~Minimap()
{
}

void
Minimap::draw(DrawingContext& gc)
{
  GUIStyle::draw_raised_box(gc, rect);

  DrawingContext& dc = *drawing_context;

  dc.clear();
  Rect minimap_rect = dc.get_rect();
  dc.draw_fillrect(Rect(geom::ipoint(0, 0), Size(minimap_rect.width(), minimap_rect.height())),
                   Color(0,0,0), -100000.0f);

  auto& objects = *editor->get_viewport()->get_objects();
  Size levelsize = editor->get_level()->get_size();

  for(auto i = objects.begin(); i != objects.end(); ++i)
  {
    Rect r = (*i)->get_rect();

    // Translate the object into minimap-co-space
    r = Rect(r.left() * minimap_rect.width() / levelsize.width(),
             r.top() * minimap_rect.height() / levelsize.height(),
             r.right() * minimap_rect.width() / levelsize.width(),
             r.bottom() * minimap_rect.height() / levelsize.height());

    unsigned attr = (*i)->get_attribs();

    Color color;

    if (attr & HAS_OWNER) // entrance & exit
      color = Color(255,255,0);
    else if (attr & HAS_GPTYPE) // groundpiece
      color = Color(0,255,0);
    else if (attr & HAS_REPEAT)
      color = Color(0,0,255); // liquid
    else // hotspot, background, etc.
      color = Color(255,0,0);

    Color bg_color(static_cast<uint8_t>(3 * color.r / 4),
                   static_cast<uint8_t>(3 * color.g / 4),
                   static_cast<uint8_t>(3 * color.b / 4));

    dc.draw_fillrect(r, bg_color, (*i)->z_index());
    dc.draw_rect(r, color, (*i)->z_index());
  }

  Vector2f viewport_pos(editor->get_viewport()->get_scroll_pos());
  Rect     viewport_rect = editor->get_viewport()->get_rect();

  viewport_pos = viewport_pos - geom::foffset(static_cast<float>(viewport_rect.width())  / 2,
                                              static_cast<float>(viewport_rect.height()) / 2);

  Rect view(Vector2i(static_cast<int>(viewport_pos.x() * static_cast<float>(minimap_rect.width())  / static_cast<float>(levelsize.width())),
                     static_cast<int>(viewport_pos.y() * static_cast<float>(minimap_rect.height()) / static_cast<float>(levelsize.height()))),
            Size(viewport_rect.width()  * minimap_rect.width() / levelsize.width(),
                 viewport_rect.height() * minimap_rect.height() / levelsize.height()));
  dc.draw_fillrect(view, Color(255, 255, 0, 150), 1000000.0f);
  dc.draw_rect(view, Color(255, 255, 0), 1000000.0f);

  gc.draw(dc);
}

void
Minimap::update (float delta)
{

}

void
Minimap::on_pointer_move(int x, int y)
{
  if (dragging)
  {
    Rect minimap_rect = drawing_context->get_rect();
    Size levelsize = editor->get_level()->get_size();

    x -= 3 + rect.left(); // take border into account
    y -= 3 + rect.top();

    x = x * levelsize.width() / minimap_rect.width();
    y = y * levelsize.height() / minimap_rect.height();

    editor->get_viewport()->set_scroll_pos(Vector2i(x, y));
  }
}

void
Minimap::on_primary_button_press (int x, int y)
{
  dragging = true;
  on_pointer_move(x, y);
}

void
Minimap::on_primary_button_release (int x, int y)
{
  on_pointer_move(x, y);
  dragging = false;
}

void
Minimap::update_layout()
{
  drawing_context->set_rect(geom::grow(rect, -3));
}

} // namespace pingus::editor

/* EOF */
