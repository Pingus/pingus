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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_CHECK_BOX_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_CHECK_BOX_HPP

#include <sigc++/signal.h>

#include "engine/display/sprite.hpp"
#include "engine/gui/rect_component.hpp"

namespace pingus {

class CheckBox : public pingus::gui::RectComponent
{
private:
  Sprite m_checkbox_marked;
  Sprite m_checkbox;

  bool state;

public:
  CheckBox(Rect const& rect);

  void draw(DrawingContext& gc) override;
  void on_primary_button_press(int x, int y) override;
  void update_layout() override {}

  void set_state(bool v, bool send_signal);

  sigc::signal<void (bool)> on_change;

private:
  CheckBox (CheckBox const&);
  CheckBox& operator= (CheckBox const&);
};

} // namespace pingus

#endif

/* EOF */
