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

class PingusMenu;
class Vector2i;

/** Framework for menu buttons */
class MenuButton : public GUI::Component
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
  MenuButton(PingusMenu* menu, const Vector2i& pos,
             const std::string& text, const std::string& desc);
  virtual ~MenuButton();

  void draw(DrawingContext& gc);
  void update (float delta);

  bool is_at (int x, int y);

  void on_pointer_enter ();
  void on_pointer_leave ();
  void on_pointer_press ();
  void on_pointer_release ();

  void on_primary_button_click (int x, int y) { on_click (); }

  virtual void on_click ();

  void set_pos(int x, int y);

private:
  MenuButton (const MenuButton&);
  MenuButton& operator= (const MenuButton&);
};

#endif

/* EOF */
