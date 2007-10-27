//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include <boost/format.hpp>
#include "gettext.h"
#include "resource.hpp"
#include "screen/screen_manager.hpp"
#include "fonts.hpp"
#include "display/drawing_context.hpp"
#include "option_menu.hpp"
#include "level_menu.hpp"

LevelMenu::LevelMenu()
{
  background = Resource::load_sprite("core/menu/filedialog");
  ok_button  = Resource::load_sprite("core/start/ok");
  marker     = Resource::load_sprite("core/menu/marker");

  levelsets.push_back(new Levelset(Pathname("levelsets/halloween.levelset", Pathname::DATA_PATH)));
}

LevelMenu::~LevelMenu()
{
  for(Levelsets::iterator i = levelsets.begin(); i != levelsets.end(); ++i)
    {
      delete *i;
    }
}

void
LevelMenu::update(const GameDelta& delta)
{
  GUIScreen::update(delta);
  SDL_Delay(50);
}

void
LevelMenu::draw_background(DrawingContext& gc)
{
  // gc.draw_fillrect(Rect(100, 100, 400, 400), Color(255, 0, 0));
  gc.draw(background, Vector2i(gc.get_width()/2 - background.get_width()/2, gc.get_height()/2 - background.get_height()/2));

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 90, "Contrib Level Menu");

  int y = 145;
  for(Levelsets::iterator i = levelsets.begin(); i != levelsets.end(); ++i)
    {
      gc.print_left(Fonts::chalk_normal, 120,  0 + y, (*i)->get_title());
      gc.print_left(Fonts::chalk_small,  140, 25 + y, (*i)->get_description());

      gc.print_right(Fonts::chalk_normal, 650, 0 + y, (boost::format("Completion: %1%%%") % (*i)->get_completion()).str());
      gc.print_right(Fonts::chalk_small, 650, 45 + y, (boost::format("%1% levels") % (*i)->get_level_count()).str());

      y += 90;
    }
  gc.draw(marker, 100, 136);
  //gc.draw(ok_button, 610, 145);
}

void
LevelMenu::on_pointer_move (int x, int y)
{
  
}

void
LevelMenu::on_escape_press()
{
  std::cout << "OptionMenu: poping screen" << std::endl;
  ScreenManager::instance()->pop_screen();
}

/* EOF */
