//  $Id: pingus_menu.cxx,v 1.10 2002/11/08 01:38:27 grumbel Exp $
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

#include "surface_button.hxx"
#include "pingus_resource.hxx"
#include "sound.hxx"
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
      SurfaceButton* editor_button = new EditorButton (this);
	  
      background = PingusResource::load_surface("misc/logo", "core");

      // FIXME: Lilla... memory leak
      gui_manager->add(new OptionsButton(this));
      gui_manager->add(new CreditButton(this));
      gui_manager->add(new QuitButton(this));
      //gui_manager->add(new MultiplayerButton(this));
      gui_manager->add(new ThemeButton(this));
      gui_manager->add(new StoryButton(this));
      gui_manager->add(editor_button);
    }

  // FIXME: Preload is probally the wrong place for this, there should
  // be show_hook and remove_hook
  PingusSound::play_music("../data/music/pingus-1.it");
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

/* EOF */
