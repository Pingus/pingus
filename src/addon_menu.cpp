//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "gettext.h"
#include "resource.hpp"
#include "screen/screen_manager.hpp"
#include "fonts.hpp"
#include "display/drawing_context.hpp"
#include "option_menu.hpp"
#include "addon_menu.hpp"

AddOnMenu::AddOnMenu()
{
  background = Resource::load_sprite("core/menu/filedialog");
  ok_button  = Resource::load_sprite("core/start/ok");
}

AddOnMenu::~AddOnMenu()
{
}

void
AddOnMenu::draw_background(DrawingContext& gc)
{
  // gc.draw_fillrect(Rect(100, 100, 400, 400), Color(255, 0, 0));
  gc.draw(background, Vector2i(gc.get_width()/2 - background.get_width()/2, gc.get_height()/2 - background.get_height()/2));

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 90, "Add-On Menu");

  gc.print_left(Fonts::chalk_normal, 120, 145, "X-Mas Pingus Sprites");
  gc.print_left(Fonts::chalk_small,  140, 170, "christmas look for penguins");
  gc.print_left(Fonts::chalk_small,  140, 190, "Author: John Foo <foo@example.com>");

  gc.print_center(Fonts::chalk_normal, gc.get_width()/2, gc.get_height()/2 + 160, "Update Online [ ]");

  gc.print_center(Fonts::chalk_normal, gc.get_width()/2 + 225 + 30, gc.get_height()/2 + 125 - 20, _("Close"));
  gc.draw(ok_button, Vector2i(gc.get_width()/2 + 225, gc.get_height()/2 + 125));

  gc.draw(ok_button, 610, 145);
}

void
AddOnMenu::on_escape_press()
{
  //std::cout << "OptionMenu: poping screen" << std::endl;
  ScreenManager::instance()->pop_screen();
}

/* EOF */
