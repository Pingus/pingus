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

#ifndef HEADER_PINGUS_EDITOR_GUI_STYLE_HPP
#define HEADER_PINGUS_EDITOR_GUI_STYLE_HPP

#include "engine/display/drawing_context.hpp"

namespace Editor {

class GUIStyle
{
private:
  public:
  static void draw_raised_box(DrawingContext& gc, const Rect& rect,
                              const Color& color = Color(237, 233, 227), int border = 1);
  static void draw_lowered_box(DrawingContext& gc, const Rect& rect,
                               const Color& color = Color(237, 233, 227), int border = 1);
};

} // namespace Editor

#endif

/* EOF */
