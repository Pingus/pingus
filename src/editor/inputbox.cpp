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

#include "editor/inputbox.hpp"

#include "util/utf8.hpp"
#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"

namespace Editor {

Inputbox::Inputbox() :
  text(),
  on_change(),
  on_enter()
{
}

Inputbox::Inputbox(const Rect& rect_) :
  RectComponent(rect_),
  text(),
  on_change(),
  on_enter()
{
}

void
Inputbox::draw(DrawingContext& gc)
{
  gc.draw_fillrect(rect, Color(255,255,255));
  gc.draw_rect(rect, has_focus() ? Color(255,128,0) : Color(0,0,0));

  gc.print_left(Fonts::verdana11,
                Vector2i(rect.left + 5,
                         rect.top + rect.get_height()/2 - Fonts::verdana11.get_height()/2),
                text);
}

void
Inputbox::set_text(const std::string& text_)
{
  text = text_;
}

void
Inputbox::on_key_pressed(const Input::KeyboardEvent& ev)
{
  if (ev.keysym.sym == SDLK_BACKSPACE) // backspace
  {
    if (!text.empty())
    {
      text = text.substr(0, text.size()-1);
      on_change(text);
    }
  }
  else if (ev.keysym.sym == SDLK_RETURN) // enter
  {
    on_change(text);
    on_enter(text);
  }
}

void
Inputbox::on_text_input(const Input::TextInputEvent& ev)
{
  text += ev.text;
  on_change(text);
}

} // namespace Editor

/* EOF */
