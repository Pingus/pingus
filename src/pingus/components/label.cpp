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

#include "pingus/components/label.hpp"

#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"

Label::Label(const std::string& label_, const Rect& rect_)
  : RectComponent(rect_),
    label(label_)
{
}

void
Label::draw(DrawingContext& gc)
{
  if (globals::developer_mode)
    gc.draw_rect(rect, Color(0, 255, 255));

  gc.print_left(Fonts::chalk_normal, Vector2i(rect.left, rect.top), label);
}

/* EOF */
