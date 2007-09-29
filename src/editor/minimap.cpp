//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "editor_level.hpp"
#include "editor_screen.hpp"
#include "editor_viewport.hpp"
#include "level_objs.hpp"
#include "gui_style.hpp"
#include "minimap.hpp"

namespace Editor {

Minimap::Minimap(EditorScreen* editor_, const Rect& rect)
  : RectComponent(rect),
    editor(editor_),
    drawing_context(new DrawingContext(rect.grow(-3)))
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
  dc.draw_fillrect(Rect(Vector2i(0, 0), Size(minimap_rect.get_width(), minimap_rect.get_height())),
                   Color(0,0,0), -100000.0f);

  // FIXME: add current viewport and scrolling
  // editor->get_viewport()->get_rect();

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

      dc.draw_fillrect(r, color, (*i)->get_pos().z);
    }

  gc.draw(dc);
}

void
Minimap::update (float delta)
{
  
}

void
Minimap::update_layout()
{
  drawing_context->set_rect(rect.grow(-3));
}

} // namespace Editor

/* EOF */
