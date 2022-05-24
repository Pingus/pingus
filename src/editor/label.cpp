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

#include "editor/label.hpp"

#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"

namespace pingus::editor {

Label::Label(const Rect& rect_, const std::string& text_) :
  RectComponent(rect_),
  text(text_)
{
}

Label::Label(const std::string& text_) :
  text(text_)
{
}

void
Label::draw (DrawingContext& gc)
{
  gc.print_left(pingus::fonts::verdana11,
                Vector2i(rect.left(), rect.top() + rect.height()/2 - pingus::fonts::verdana11.get_height()/2),
                text);
}

void
Label::update (float delta)
{
}

} // namespace pingus::editor

/* EOF */
