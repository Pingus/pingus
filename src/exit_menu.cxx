//  $Id: exit_menu.cxx,v 1.19 2003/04/19 10:23:17 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include "my_gettext.hxx"
#include "pingus_menu_manager.hxx"
#include "pingus_resource.hxx"
#include "gui/surface_button.hxx"
#include "gui/gui_manager.hxx"
#include "sound/sound.hxx"

class ExitMenuYesButton : public GUI::SurfaceButton
{
private:
  PingusMenuManager* manager;
public:
  ExitMenuYesButton (PingusMenuManager* m)
    : GUI::SurfaceButton(CL_Display::get_width()/2 - 170,
                         CL_Display::get_height()/2 + 15,
                         ResDescriptor("menu/exit_button_normal", "core"),
                         ResDescriptor("menu/exit_button_pressed", "core"),
                         ResDescriptor("menu/exit_button_hover", "core")),
      manager (m)
  {
  }

  void draw (GraphicContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_right(Fonts::chalk_large, CL_Display::get_width()/2 - 80,
                   CL_Display::get_height()/2 + 25, _("Yes"));
  }

  void on_click()
  {
    PingusSound::play_sound ("yipee");
    manager->exit ();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    PingusSound::play_sound ("tick");
  }
};

class ExitMenuNoButton : public GUI::SurfaceButton
{
private:
  PingusMenuManager* manager;
public:
  ExitMenuNoButton (PingusMenuManager* m)
    : GUI::SurfaceButton(CL_Display::get_width()/2, CL_Display::get_height()/2 + 15,
                         ResDescriptor("menu/exit_button_normal", "core"),
                         ResDescriptor("menu/exit_button_pressed", "core"),
                         ResDescriptor("menu/exit_button_hover", "core")),
      manager (m)
  {
  }

  void draw (GraphicContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_right(Fonts::chalk_large, CL_Display::get_width()/2 + 80,
                   CL_Display::get_height()/2 + 25, _("No"));
  }

  void on_click() {
    PingusSound::play_sound ("yipee");
    manager->pop_menu();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    PingusSound::play_sound ("tick");
  }
};

ExitMenu::ExitMenu (PingusMenuManager* manager_)
  : PingusSubMenu (manager_)
{
  gui_manager->add(new ExitMenuYesButton(manager), true);
  gui_manager->add(new ExitMenuNoButton (manager), true);
}

ExitMenu::~ExitMenu ()
{
}

bool
ExitMenu::draw (GraphicContext& gc)
{
  //gc.draw_fillrect (0, 0, CL_Display::get_width (), CL_Display::get_height (),
  //0, 0, 0, 0.5);
  gc.draw(sur,  gc.get_width ()/2 - sur.get_width ()/2,
          gc.get_height ()/2 - sur.get_height ()/2);
  gc.print_center(Fonts::chalk_large, gc.get_width()/2, gc.get_height()/2 - 70, _("Exit Pingus?"));
  PingusSubMenu::draw(gc);
  return true;
}

void
ExitMenu::preload ()
{
  sur = PingusResource::load_surface ("menu/exit_menu", "core");
}

/* EOF */
