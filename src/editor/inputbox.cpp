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

#include <logmich/log.hpp>

#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"

namespace Editor {

Inputbox::Inputbox() :
  m_text(),
  m_faulty_input(false),
  on_change(),
  on_enter()
{
}

Inputbox::Inputbox(const Rect& rect_) :
  RectComponent(rect_),
  m_text(),
  m_faulty_input(false),
  on_change(),
  on_enter()
{
}

void
Inputbox::draw(DrawingContext& gc)
{
  Color const bg_color = m_faulty_input ? Color(255, 128, 128) : Color(255, 255, 255);

  gc.draw_fillrect(rect, bg_color);
  gc.draw_rect(rect, has_focus() ? Color(255,128,0) : Color(0,0,0));

  gc.print_left(Fonts::verdana11,
                Vector2i(rect.left() + 5,
                         rect.top() + rect.height()/2 - Fonts::verdana11.get_height()/2),
                m_text);
}

void
Inputbox::set_text(const std::string& text)
{
  m_text = text;
}

void
Inputbox::on_key_pressed(const Input::KeyboardEvent& ev)
{
  if (ev.keysym.sym == SDLK_BACKSPACE) // backspace
  {
    if (!m_text.empty())
    {
      m_text = m_text.substr(0, m_text.size()-1);
      try {
        on_change(m_text);
        m_faulty_input = false;
      } catch(std::exception const& err) {
        m_faulty_input = true;
        log_debug(err.what());
      }
    }
  }
  else if (ev.keysym.sym == SDLK_RETURN) // enter
  {
    try {
      on_change(m_text);
      on_enter(m_text);
      m_faulty_input = false;
    } catch(std::exception const& err) {
      m_faulty_input = true;
      log_debug(err.what());
    }
  }
}

void
Inputbox::on_text_input(const Input::TextInputEvent& ev)
{
  m_text += ev.text;
  try {
    on_change(m_text);
    m_faulty_input = false;
  } catch(std::exception const& err) {
    m_faulty_input = true;
    log_debug(err.what());
  }
}

} // namespace Editor

/* EOF */
