//  $Id: pingus_menu.cxx,v 1.4 2002/07/29 22:17:53 grumbel Exp $
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

#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/keyboard.h>
#include <ClanLib/Display/Input/inputdevice.h>

#include "globals.hxx"
#include "alpha_button.hxx"
#include "surface_button.hxx"
#include "credits.hxx"
#include "pingus_resource.hxx"
#include "pingus_menu.hxx"
#include "display.hxx"
#include "sound.hxx"
#include "debug.hxx"
#include "gui/root_gui_manager.hxx"

PingusMenu::PingusMenu(PingusMenuManager* m)
  : PingusSubMenu (m)
{
  is_init = false;

  // FIXME: Memory leak
  controller = new Input::Controller ("../doc/mycontroller.xml");
  gui_manager = new GUI::RootGUIManager (controller);
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
 
      if (start_editor)
	{  
	  std::cout << "Editor starting is disabled" << std::endl;
	  //editor_button->on_click ();
	}

      background = PingusResource::load_surface("misc/logo", "core");

      // FIXME: Lilla... memory leak
      gui_manager->add(new OptionsButton (this));
      gui_manager->add(new CreditButton (this));
      gui_manager->add(new QuitButton (this));
      gui_manager->add(new MultiplayerButton (this));
      gui_manager->add(new ThemeButton (this));
      gui_manager->add(new StoryButton (this));
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
PingusMenu::draw()
{
  gui_manager->draw ();
}

void
PingusMenu::update (float delta)
{
  controller->update (delta);
  gui_manager->update (delta);
}

void
PingusMenu::on_resize(int w, int h)
{
  pout << "Width: " << w << " Height: " << h << std::endl;
}

/* EOF */
