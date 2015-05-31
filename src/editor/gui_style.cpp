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

#include "editor/gui_style.hpp"

namespace Editor {

void
GUIStyle::draw_raised_box(DrawingContext& gc, const Rect& rect, const Color& color, int border)
{
  // FIXME: Should use draw_line
  gc.draw_fillrect(rect, Color(255, 255, 255));
  gc.draw_fillrect(Rect(rect.left+border, rect.top+border, rect.right, rect.bottom),
                   Color(169, 157, 140));
  gc.draw_fillrect(Rect(rect.left+border, rect.top+border, rect.right-border, rect.bottom-border),
                   color);
}

void
GUIStyle::draw_lowered_box(DrawingContext& gc, const Rect& rect, const Color& color, int border)
{
  // FIXME: Should use draw_line
  gc.draw_fillrect(rect,
                   Color(169, 157, 140));
  gc.draw_fillrect(Rect(rect.left+border, rect.top+border, rect.right, rect.bottom),
                   Color(255, 255, 255));
  gc.draw_fillrect(Rect(rect.left+border, rect.top+border, rect.right-border, rect.bottom-border),
                   color);
}

} // namespace Editor

/* EOF */
