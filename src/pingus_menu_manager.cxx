//  $Id: pingus_menu_manager.cxx,v 1.21 2003/03/24 11:18:53 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Display/Display/display.h>
#include "gui/screen_manager.hxx"
#include "sound/sound.hxx"
#include "pingus_menu_manager.hxx"

PingusMenuManager* PingusMenuManager::instance_ = 0;

PingusMenuManager::PingusMenuManager ()
  : unplayable ("misc/unplayable", "core"),
    intro (this), mainmenu (this), optionmenu (this),
    exitmenu (this)
{
  unplayable.set_align_center ();

  //current_menu = 0;
  //if (intro_disabled)
  push_menu (&mainmenu);
  
  //else
  //push_menu (&intro); 
}

PingusMenuManager::~PingusMenuManager ()
{
}

bool
PingusMenuManager::draw (GraphicContext& gc)
{
  background.draw (gc);
  
  gc.draw (unplayable, Vector(CL_Display::get_width ()/2, 30));

  gc.draw_fillrect(0, CL_Display::get_height () - 22,
                   CL_Display::get_width (), CL_Display::get_height (),
                   0, 0, 0, 1.0f);

  for (MenuStackIter i = menu_stack.begin (); i != menu_stack.end (); ++i)
    (*i)->draw (gc);

  return true;
}

void
PingusMenuManager::update (const GameDelta& delta)
{
  background.update (delta.get_time ());
  // We copy the menu_stack so that we don't invalidate our
  // iterators when menu's are removed/added in update()
  //std::vector<PingusSubMenu *> tmp_menu_stack = menu_stack;
  /*for (MenuStackIter i = tmp_menu_stack.begin (); i != tmp_menu_stack.end (); ++i)
    (*i)->update (delta);*/
  menu_stack.back ()->update (delta);
}

void 
PingusMenuManager::set_menu (PingusSubMenu * menu)
{
  pop_menu ();
  push_menu (menu);
}

void 
PingusMenuManager::push_menu (PingusSubMenu * menu)
{
  menu->preload ();
  menu_stack.push_back (menu);
}

void 
PingusMenuManager::pop_menu ()
{
  if (!menu_stack.empty ())
    menu_stack.pop_back();
}

PingusSubMenu *
PingusMenuManager::current_menu ()
{
  if (!menu_stack.empty ())
    {
      MenuStackRIter i = menu_stack.rbegin ();
      if (! *i)
	std::cout << "PingusMenuManager: Error: current_menu is " << *i << std::endl;
      return *i;
    }
  else
    {
      std::cout << "PingusMenuManager: Error: MenuStack empty!" << std::endl;
      return 0;
    }
}

void 
PingusMenuManager::fadeout ()
{
  std::cout << "PingusMenuManager::fadeout () Not implemented" << std::endl;
#if 0
  DeltaManager delta_manager;
  EnlargingRectFadeOut fadeout;

  while (!fadeout.finished ())
    {
      float time_delta = delta_manager.getset ();
      fadeout.update (time_delta);

      current_menu ()->draw ();
      fadeout.draw ();
      Display::flip_display ();

      CL_System::keep_alive ();
    }
#endif 
}

void
PingusMenuManager::show_exit_menu ()
{
  push_menu (&exitmenu);
}

void
PingusMenuManager::exit ()
{
  std::cout << "poping PingusMenuManager" << std::endl;
  ScreenManager::instance ()->pop_screen ();
}

void
PingusMenuManager::on_startup()
{
  PingusSound::play_music("pingus-1.it");
}

PingusMenuManager*
PingusMenuManager::instance ()
{
  if (instance_)
    return instance_;
  else
    return instance_ = new PingusMenuManager ();
}

void
PingusMenuManager::init()
{
  instance_ = 0;
}

void
PingusMenuManager::deinit()
{
  delete instance_;
}

/* EOF */
