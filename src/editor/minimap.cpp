//  $Id: minimap.cpp 3448 2007-11-01 08:37:49Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "editor_level.hpp"
#include "editor_screen.hpp"
#include "viewport.hpp"
#include "level_objs.hpp"
#include "gui_style.hpp"
#include "minimap.hpp"

namespace Editor {

Minimap::Minimap(EditorScreen* editor_, const Rect& rect)
  : RectComponent(rect),
    editor(editor_),
    drawing_context(new DrawingContext(rect.grow(-3))),
    dragging(false)
{
}

Minimap::~Minimap()
{
  delete drawing_context;
}

void
Minimap::draw(DrawingContext& gc)
{
  GUIStyle::draw_raised_box(gc, rect);

  DrawingContext& dc = *drawing_context;

  dc.clear();
  Rect minimap_rect = dc.get_rect();
  dc.draw_fillrect(Rect(Vector2i(0, 0), Size(minimap_rect.get_width(), minimap_rect.get_height())),
                   Color(0,0,0), -100000.0f);

  std::vector<LevelObj*>& objects = *editor->get_viewport()->get_objects();
  Size levelsize = editor->get_level()->get_size();

  for(std::vector<LevelObj*>::iterator i = objects.begin(); i != objects.end(); ++i)
    {
      Rect r = (*i)->get_rect();

      // Translate the object into minimap-co-space
      r.left  = r.left  * minimap_rect.get_width() / levelsize.width;
      r.right = r.right * minimap_rect.get_width() / levelsize.width;

      r.top    = r.top    * minimap_rect.get_height() / levelsize.height;
      r.bottom = r.bottom * minimap_rect.get_height() / levelsize.height;

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

      Color bg_color(int(color.r * 0.75f), 
                     int(color.g * 0.75f), 
                     int(color.b * 0.75f));

      dc.draw_fillrect(r, bg_color, (*i)->get_pos().z);
      dc.draw_rect(r, color, (*i)->get_pos().z);
    }

  Vector2f viewport_pos  = editor->get_viewport()->get_scroll_pos();
  Rect     viewport_rect = editor->get_viewport()->get_rect();

  viewport_pos.x -= viewport_rect.get_width()/2;
  viewport_pos.y -= viewport_rect.get_height()/2;

  Rect view(Vector2i(int(viewport_pos.x * minimap_rect.get_width() / levelsize.width),
                     int(viewport_pos.y * minimap_rect.get_height() / levelsize.height)),
            Size(viewport_rect.get_width()  * minimap_rect.get_width() / levelsize.width,
                 viewport_rect.get_height() * minimap_rect.get_height() / levelsize.height));
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

      x -= 3 + rect.left; // take border into account
      y -= 3 + rect.top;

      x = x * levelsize.width / minimap_rect.get_width();
      y = y * levelsize.height / minimap_rect.get_height();

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
  drawing_context->set_rect(rect.grow(-3));
}

} // namespace Editor

/* EOF */
