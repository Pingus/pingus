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

#include <boost/signals2.hpp>

#include "engine/gui/rect_component.hpp"

namespace Editor {

class Inputbox : public GUI::RectComponent
{
private:
  std::string text;

public:
  Inputbox(const Rect& rect);
  Inputbox();

  void draw(DrawingContext& gc);

  void set_text(const std::string& text);
  std::string get_text() const { return text; }
  void on_key_pressed(const Input::KeyboardEvent& ev);
  void on_text_input(const Input::TextInputEvent& ev);

  void update_layout() {}

  boost::signals2::signal<void (const std::string&)> on_change;
  boost::signals2::signal<void (const std::string&)> on_enter;

private:
  Inputbox (const Inputbox&);
  Inputbox& operator= (const Inputbox&);
};

} // namespace Editor

#endif

/* EOF */
