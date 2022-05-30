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

#ifndef HEADER_PINGUS_EDITOR_INPUTBOX_HPP
#define HEADER_PINGUS_EDITOR_INPUTBOX_HPP

#include <sigc++/signal.h>

#include "engine/gui/rect_component.hpp"

namespace pingus::editor {

class Inputbox : public gui::RectComponent
{
private:
  std::string m_text;
  bool m_faulty_input;

public:
  Inputbox(Rect const& rect);
  Inputbox();

  void draw(DrawingContext& gc) override;

  void set_text(std::string const& text);
  std::string get_text() const { return m_text; }
  void on_key_pressed(input::KeyboardEvent const& ev) override;
  void on_text_input(input::TextInputEvent const& ev) override;

  void update_layout() override {}

  sigc::signal<void (std::string const&)> on_change;
  sigc::signal<void (std::string const&)> on_enter;

private:
  Inputbox (Inputbox const&);
  Inputbox& operator= (Inputbox const&);
};

} // namespace pingus::editor

#endif

/* EOF */
