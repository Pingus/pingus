// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_MESSAGE_BOX_HPP
#define HEADER_PINGUS_EDITOR_MESSAGE_BOX_HPP

#include "engine/gui/group_component.hpp"
#include "editor/button.hpp"

namespace pingus::editor {

class MessageBox : public gui::GroupComponent
{
private:
  Button* m_ok_button;
  Button* m_cancel_button;

  std::string m_title;
  std::string m_text;

public:
  MessageBox(Rect const& rect);

  void draw_background(DrawingContext& gc) override;
  void update_layout() override;

  void set_title(std::string const& text);
  void set_text(std::string const& text);
  void set_ok_text(std::string const& text);

  void on_ok_button();
  void on_cancel_button();

public:
  sigc::signal<void()> on_ok;

private:
  MessageBox(MessageBox const&);
  MessageBox& operator=(MessageBox const&);
};

} // namespace pingus::editor

#endif

/* EOF */
