//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "screen/screen_manager.hpp"
#include "sound/sound.hpp"
#include "resource.hpp"
#include "pingus_menu_manager.hpp"
#include "globals.hpp"


PingusMenuManager* PingusMenuManager::instance_ = 0;

PingusMenuManager::PingusMenuManager ()
  : mainmenu (this),
    exitmenu (this)
{
  int w = Display::get_width();
  int h = Display::get_height();

  Sprite layer1("core/menu/layer1");
  Sprite layer2("core/menu/layer2");
  Sprite layer3("core/menu/layer3");
  Sprite layer4("core/menu/layer4");
  Sprite layer5("core/menu/layer5");

  // We only need to scale the background main menu images if the screen 
  // resolution is not default
  if (w != default_screen_width && h != default_screen_height)
    {
      layer1.scale(w, 185 * h / default_screen_height);
      layer2.scale(w, 362 * h / default_screen_height);
      layer3.scale(w, 306 * h / default_screen_height);
      layer4.scale(w, 171 * h / default_screen_height);
      layer5.scale(302 * w / default_screen_width, 104 * h / default_screen_height);
      
      background.add_layer(layer1, 0, 0, 12, 0);
      background.add_layer(layer2, 0, 150 * (float)h / default_screen_height, 25, 0);
      background.add_layer(layer3, 0, 200 * (float)h / default_screen_height, 50, 0);
      background.add_layer(layer4, 0, 429 * (float)h / default_screen_height, 100, 0);
      background.add_layer(layer5, 0, 500 * (float)h / default_screen_height, 200, 0);
    }
  else
    {
      background.add_layer(layer1, 0, 0, 12, 0);
      background.add_layer(layer2, 0, 150, 25, 0);
      background.add_layer(layer3, 0, 200, 50, 0);
      background.add_layer(layer4, 0, 429, 100, 0);
      background.add_layer(layer5, 0, 500, 200, 0);
    }

  push_menu (&mainmenu);
}

PingusMenuManager::~PingusMenuManager ()
{
}

bool
PingusMenuManager::draw (DrawingContext& gc)
{
  background.draw(gc);

  gc.draw_fillrect(0,
                   Display::get_height () - 26,
                   Display::get_width (),
                   Display::get_height (),
                   Color(0, 0, 0, 255));

  for (MenuStackIter i = menu_stack.begin (); i != menu_stack.end (); ++i)
    (*i)->draw(gc);

  return true;
}

void
PingusMenuManager::update(float delta)
{
  background.update(delta);
  menu_stack.back()->update(delta);
}

void
PingusMenuManager::update(const Input::Event& event)
{
  menu_stack.back()->update(event);
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
PingusMenuManager::show_exit_menu ()
{
  push_menu(&exitmenu);
}

void
PingusMenuManager::exit ()
{
  //std::cout << "poping PingusMenuManager" << std::endl;
  ScreenManager::instance ()->pop_screen ();
}

void
PingusMenuManager::on_startup()
{
  Sound::PingusSound::play_music("pingus-1.it");
}

void
PingusMenuManager::resize(const Size& size)
{
  
}

/* EOF */
