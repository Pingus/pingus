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

#include "pingus/screens/addon_menu.hpp"

#include "engine/display/drawing_context.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"

AddOnMenu::AddOnMenu() :
  background(),
  ok_button()
{
  //background("core/menu/wood"),
  //blackboard("core/menu/blackboard"),

  background = Sprite("core/menu/blackboard");
  ok_button  = Sprite("core/start/ok");
}

AddOnMenu::~AddOnMenu()
{
}

void
AddOnMenu::draw_background(DrawingContext& gc)
{
  // gc.draw_fillrect(Rect(100, 100, 400, 400), Color(255, 0, 0));
  gc.draw(background, Vector2i(gc.get_width()/2, gc.get_height()/2));

  gc.print_center(Fonts::chalk_large, Vector2i(gc.get_width()/2, 90), "Add-On Menu");

  gc.print_left(Fonts::chalk_normal, Vector2i(120, 145), "X-Mas Pingus Sprites");
  gc.print_left(Fonts::chalk_small,  Vector2i(140, 170), "christmas look for penguins");
  gc.print_left(Fonts::chalk_small,  Vector2i(140, 190), "Author: John Foo <foo@example.com>");

  gc.print_center(Fonts::chalk_normal, Vector2i(gc.get_width()/2, gc.get_height()/2 + 160), "Update Online [ ]");

  gc.print_center(Fonts::chalk_normal, Vector2i(gc.get_width()/2 + 245 + 30, gc.get_height()/2 + 150 - 20), _("Close"));
  gc.draw(ok_button, Vector2i(gc.get_width()/2 + 245, gc.get_height()/2 + 150));

  gc.draw(ok_button, Vector2i(610, 145));
}

void
AddOnMenu::on_escape_press()
{
  //log_info("OptionMenu: poping screen");
  ScreenManager::instance()->pop_screen();
}

/* EOF */
