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

#include "pingus/components/menu_button.hpp"

#include "engine/display/drawing_context.hpp"
#include "engine/sound/sound.hpp"
#include "math/vector2i.hpp"
#include "pingus/screens/pingus_menu.hpp"

MenuButton::MenuButton(PingusMenu* menu_,
                       const Vector2i& pos_,
                       const std::string& text_, const std::string& desc_) :
  menu(menu_),
  surface_p(),
  highlight(),
  font(),
  font_large(),
  x_pos(),
  y_pos(),
  desc(),
  text(),
  mouse_over(),
  pressed()
{
  surface_p = Sprite("core/menu/menuitem");
  highlight = Sprite("core/menu/menuitem_highlight");

  text = text_;
  desc  = desc_;

  x_pos = pos_.x;
  y_pos = pos_.y;

  font       = Fonts::pingus_small;
  font_large = Fonts::chalk_large;

  mouse_over = false;
  pressed    = false;
}

MenuButton::~MenuButton ()
{
}

void
MenuButton::on_click ()
{
  //log_info("MenuButton: Click");
  menu->on_click(this);
}

void
MenuButton::draw (DrawingContext& gc)
{
  if (mouse_over) // pressed
  {
    gc.draw(surface_p,Vector2i(x_pos, y_pos));
    gc.draw(highlight, Vector2i(x_pos, y_pos));
    gc.print_center(font_large, Vector2i(x_pos, y_pos - 28), text);
  }
  else
  {
    gc.draw(surface_p, Vector2i(x_pos, y_pos));
    gc.print_center(font_large, Vector2i(x_pos, y_pos - 28), text);
  }
}

void
MenuButton::update (float delta)
{
}

void
MenuButton::on_pointer_enter ()
{
  mouse_over = true;
  Sound::PingusSound::play_sound ("tick");
  //log_info("X: " << this << "enter");
  menu->set_hint(desc);
}

void
MenuButton::on_pointer_leave ()
{
  //log_info("X: " << this << "leave");
  mouse_over = false;
  menu->set_hint("");
}

void
MenuButton::on_pointer_press ()
{
  pressed = true;
}

void
MenuButton::on_pointer_release ()
{
  pressed = false;
}

bool
MenuButton::is_at(int x, int y)
{
  return (x > x_pos - int(surface_p.get_width()) / 2
          && x < x_pos + int(surface_p.get_width()) / 2
          && y > y_pos - int(surface_p.get_height()) / 2
          && y < y_pos + int(surface_p.get_height()) / 2);
}

void
MenuButton::set_pos(int x, int y)
{
  x_pos = x;
  y_pos = y;
}

/* EOF */
