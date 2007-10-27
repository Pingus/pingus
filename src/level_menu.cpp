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
#include "system.hpp"
#include "display/display.hpp"
#include "level_menu.hpp"

LevelMenu::LevelMenu()
  : x_pos((Display::get_width()  - 800)/2),
    y_pos((Display::get_height() - 600)/2)
{
  background = Resource::load_sprite("core/menu/filedialog");
  ok_button  = Resource::load_sprite("core/start/ok");
  marker     = Resource::load_sprite("core/menu/marker");
  marker_small = Resource::load_sprite("core/menu/marker2");

  std::string path = Pathname("levelsets", Pathname::DATA_PATH).get_sys_path();
  System::Directory directory = System::opendir(path, "*.levelset");
  for(System::Directory::iterator i = directory.begin(); i != directory.end(); ++i)
    {
      levelsets.push_back(new Levelset(Pathname(path + "/" + i->name, Pathname::SYSTEM_PATH)));
    }
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
  gc.push_modelview();
  gc.translate(x_pos, y_pos);

  // gc.draw_fillrect(Rect(100, 100, 400, 400), Color(255, 0, 0));
  gc.draw(background, Vector2i(400 - background.get_width()/2, 300 - background.get_height()/2));

  if (0)
    {
      gc.print_center(Fonts::chalk_large, 800/2, 90, _("Levelset Menu"));

      int y = 145;
      for(Levelsets::iterator i = levelsets.begin(); i != levelsets.end(); ++i)
        {
          if (i == levelsets.begin())
            gc.draw(marker, 100, y - 20);

          gc.print_left(Fonts::chalk_normal, 120,  0 + y, (*i)->get_title());
          gc.print_left(Fonts::chalk_small,  140, 25 + y, (*i)->get_description());

          gc.print_right(Fonts::chalk_normal, 650, 0 + y, (boost::format("Completion: %1%%%") % (*i)->get_completion()).str());
          gc.print_right(Fonts::chalk_small,  650, 45 + y, (boost::format("%1% levels") % (*i)->get_level_count()).str());

          //gc.draw(ok_button, 620, y);

          y += 90;
        }
      //gc.draw(ok_button, 610, 145);
    }
  else
    {
      Levelset* levelset = *levelsets.begin();

      gc.print_center(Fonts::chalk_large, 800/2, 90, _(levelset->get_title()));

      gc.print_left(Fonts::chalk_normal,  120, 145, "Levelname");
      gc.print_right(Fonts::chalk_normal, 660, 145, "Completed");
      int y = 185;
      for(int i = 0; i < levelset->get_level_count(); ++i)
        {
          if (i == 0)
            gc.draw(marker_small, 100, y-4);

          std::string level = levelset->get_level(i);          
          gc.print_left(Fonts::chalk_small, 120, y, level);
          gc.print_right(Fonts::chalk_small, 660, y, "[x]");
          y += 32;
        }
    }
  gc.pop_modelview();
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
