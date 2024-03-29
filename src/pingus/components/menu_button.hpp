// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_MENU_BUTTON_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_MENU_BUTTON_HPP

#include <string>

#include "engine/display/font.hpp"
#include "engine/display/sprite.hpp"
#include "engine/gui/component.hpp"

namespace pingus {

class PingusMenu;

/** Framework for menu buttons */
class MenuButton : public pingus::gui::Component
{
protected:
  PingusMenu* menu;
  Sprite surface_p;
  Sprite highlight;
  Font   font;
  Font   font_large;

  int x_pos;
  int y_pos;

  std::string desc;
  std::string text;

  bool mouse_over;
  bool pressed;

public:
  MenuButton(PingusMenu* menu, geom::ipoint const& pos,
             std::string const& text, std::string const& desc);
  ~MenuButton() override;

  void draw(DrawingContext& gc) override;
  void update (float delta) override;

  bool is_at (int x, int y) override;

  void on_pointer_enter() override;
  void on_pointer_leave() override;
  void on_pointer_press();
  void on_pointer_release();

  void on_primary_button_click (int x, int y) override { on_click(); }

  virtual void on_click();

  void set_pos(int x, int y);

private:
  MenuButton (MenuButton const&);
  MenuButton& operator= (MenuButton const&);
};

} // namespace pingus

#endif

/* EOF */
