//  $Id: pingus_menu.cxx,v 1.15 2003/03/27 17:04:49 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include "menu_button.hxx"
#include "pingus_resource.hxx"
#include "debug.hxx"
#include "pingus_menu_manager.hxx"
#include "gui/gui_manager.hxx"

PingusMenu::PingusMenu (PingusMenuManager* m)
  : PingusSubMenu (m)
{
  is_init = false;
}

void
PingusMenu::preload ()
{
  if (!is_init)
    {
      pout << "PingusMenu::init ()" << std::endl;
      //event_enabled = true;
      is_init = true;

      background = PingusResource::load_surface("misc/logo", "core");

      //gui_manager->add(new OptionsButton(this));
      /*if (game_finished)
        gui_manager->add(new CreditButton(this));*/
      //gui_manager->add(new MultiplayerButton(this));
      //gui_manager->add(new ThemeButton(this));

      gui_manager->add(new QuitButton(this));
      gui_manager->add(new StoryButton(this));
      gui_manager->add(new EditorButton (this));
    }
}

PingusMenu::~PingusMenu()
{
}

void
PingusMenu::on_resize(int w, int h)
{
  pout << "Width: " << w << " Height: " << h << std::endl;
}

void
PingusMenu::on_escape_press ()
{
  get_manager ()->show_exit_menu ();
}

void
PingusMenu::draw_foreground(GraphicContext& gc)
{
  gc.draw(background, (gc.get_width()/2) - (background.get_width()/2), 50);
  gc.print_left(Fonts::pingus_small, 20, 500, 
                "Pingus version 0.6.0, Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>\n");
  gc.print_left(Fonts::pingus_small, 20, 530, 
                "Pingus comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome\n"
                "to redistribute it under certain conditions; see the file COPYING for details.\n");
}

/* EOF */
