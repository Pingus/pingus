//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "gettext.h"
#include "pingus_menu_manager.hpp"
#include "resource.hpp"
#include "gui/surface_button.hpp"
#include "gui/gui_manager.hpp"
#include "sound/sound.hpp"


class ExitMenuYesButton : public GUI::SurfaceButton
{
private:
  PingusMenuManager* manager;
public:
  ExitMenuYesButton (PingusMenuManager* m)
    : GUI::SurfaceButton(Display::get_width()/2 - 80,
                         Display::get_height()/2 + 15,
                         ResDescriptor("core/menu/exit_button_normal"),
                         ResDescriptor("core/menu/exit_button_pressed"),
                         ResDescriptor("core/menu/exit_button_hover")),
      manager (m)
  {
  }

  void draw (DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_right(Fonts::chalk_large, (float)Display::get_width()/2 - 80,
                   (float)Display::get_height()/2 + 15, _("Yes"));
  }

  bool is_at(int x, int y) {
	  return x > x_pos - Fonts::chalk_large.get_width(_("Yes"))
		&& x < x_pos + int(button_surface.get_width())
		&& y > y_pos && y < y_pos + int(button_surface.get_height());
  }

  void on_click()
  {
    Sound::PingusSound::play_sound ("yipee");
    manager->exit ();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};

class ExitMenuNoButton : public GUI::SurfaceButton
{
private:
  PingusMenuManager* manager;
public:
  ExitMenuNoButton(PingusMenuManager* m)
    : GUI::SurfaceButton(Display::get_width()/2 + 80, Display::get_height()/2 + 15,
                         ResDescriptor("core/menu/exit_button_normal"),
                         ResDescriptor("core/menu/exit_button_pressed"),
                         ResDescriptor("core/menu/exit_button_hover")),
      manager(m)
  {
  }

  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_right(Fonts::chalk_large, (float)Display::get_width()/2 + 80,
                   (float)Display::get_height()/2 + 15, _("No"));
  }

  bool is_at(int x, int y) {
	  return x > x_pos - Fonts::chalk_large.get_width(_("No"))
		&& x < x_pos + int(button_surface.get_width())
		&& y > y_pos && y < y_pos + int(button_surface.get_height());
  }

  void on_click() {
    Sound::PingusSound::play_sound ("yipee");
    manager->pop_menu();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};

ExitMenu::ExitMenu(PingusMenuManager* manager_)
  : PingusSubMenu(manager_)
{
  gui_manager->add(new ExitMenuYesButton(manager), true);
  gui_manager->add(new ExitMenuNoButton(manager), true);
}

ExitMenu::~ExitMenu()
{
}

bool
ExitMenu::draw(DrawingContext& gc)
{
  //gc.draw_fillrect (0, 0, Display::get_width (), Display::get_height (),
  //0, 0, 0, 0.5);
  gc.draw(sur, Vector3f(gc.get_width()/2 - sur.get_width()/2,
                        gc.get_height()/2 - sur.get_height()/2));
  gc.print_center(Fonts::chalk_large, gc.get_width()/2, gc.get_height()/2 - 70, _("Exit Pingus?"));
  PingusSubMenu::draw(gc);
  return true;
}

void
ExitMenu::preload()
{
  sur = Resource::load_sprite("core/menu/exit_menu");
}


/* EOF */
