//  $Id: pingus_menu.cxx,v 1.19 2003/04/10 16:01:02 grumbel Exp $
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

#include <ClanLib/Display/Display/display.h>
#include <config.h>
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
      //pout << "PingusMenu::init ()" << std::endl;
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
  if (gc.get_height() == 480)
    {
      gc.draw(background, 
              (gc.get_width()/2) - (background.get_width()/2), 
              20);
    }
  else
    {
      gc.draw(background, (gc.get_width()/2) - (background.get_width()/2), 
              CL_Display::get_height()/10);
    }
#ifdef OFFICIAL_PINGUS_BUILD
  gc.print_left(Fonts::pingus_small, 20, CL_Display::get_height()-100, 
                "Pingus version "VERSION", Copyright (C) 2003 Ingo Ruhnke <grumbel@gmx.de>\n");
#else
  gc.print_left(Fonts::pingus_small, 20, CL_Display::get_height()-100, 
                "Pingus version "VERSION" (unofficial build), Copyright (C) 2003 Ingo Ruhnke <grumbel@gmx.de>\n");
#endif
  gc.print_left(Fonts::pingus_small, 20, CL_Display::get_height()-70, 
                "Pingus comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome\n"
                "to redistribute it under certain conditions; see the file COPYING for details.\n");
}

/* EOF */
