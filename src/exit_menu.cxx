//  $Id: exit_menu.cxx,v 1.14 2003/04/02 10:34:58 grumbel Exp $
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

class ExitMenuYesButton : public GUI::SurfaceButton
{
private:
  PingusMenuManager* manager;
public:
  ExitMenuYesButton (PingusMenuManager* m) 
    : GUI::SurfaceButton(230, 315,
                         ResDescriptor("menu/exit_button_normal", "core"),
                         ResDescriptor("menu/exit_button_pressed", "core"),
                         ResDescriptor("menu/exit_button_hover", "core")),
      manager (m)
  {
  }
 
  void draw (GraphicContext& gc) {
    GUI::SurfaceButton::draw(gc);
    gc.print_left(Fonts::chalk_large, 250,  325, _(" Yes"));
  }

  void on_click()
  {
    manager->exit ();
  }
};

class ExitMenuNoButton : public GUI::SurfaceButton
{
private:
  PingusMenuManager* manager;
public:
  ExitMenuNoButton (PingusMenuManager* m)
    : GUI::SurfaceButton(420, 315,
                         ResDescriptor("menu/exit_button_normal", "core"),
                         ResDescriptor("menu/exit_button_pressed", "core"),
                         ResDescriptor("menu/exit_button_hover", "core")),
      manager (m)
  {
  }

  void draw (GraphicContext& gc) {
    GUI::SurfaceButton::draw(gc);
    gc.print_left(Fonts::chalk_large, 460, 325, _("No"));
  }

  void on_click() {
    manager->pop_menu();
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
