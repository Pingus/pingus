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

#include "editor/checkbox.hpp"

#include "editor/gui_style.hpp"
#include "pingus/fonts.hpp"

namespace Editor {

Checkbox::Checkbox(const Rect& rect_, const std::string& label_)
  : RectComponent(rect_),
    checked(false),
    label(label_),
    on_change()
{
}

Checkbox::~Checkbox()
{
}

void
Checkbox::draw(DrawingContext& gc)
{
  if (checked)
    GUIStyle::draw_lowered_box(gc, rect);
  else
    GUIStyle::draw_raised_box(gc, rect);

  if (!label.empty())
    gc.print_center(Fonts::verdana11,
                    Vector2i(rect.left + rect.get_width()/2,
                             rect.top + rect.get_height()/2 - Fonts::verdana11.get_height()/2),
                    label);
}

void
Checkbox::on_primary_button_press(int x, int y)
{
  checked = !checked;
  on_change(checked);
}

void
Checkbox::set_checked(bool t)
{
  checked = t;
  //on_change(checked);
}

} // namespace Editor

/* EOF */
