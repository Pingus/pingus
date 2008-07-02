//  $Id: level_dot.cpp 3328 2007-10-26 04:19:25Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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
#include <math.h>
#include "../font.hpp"
#include "../gettext.h"
#include "../globals.hpp"
#include "../system.hpp"
#include "../fonts.hpp"
#include "../display/drawing_context.hpp"
#include "../screen/screen_manager.hpp"
#include "../resource.hpp"
#include "../path_manager.hpp"
#include "../start_screen.hpp"
#include "../plf_res_mgr.hpp"
#include "../savegame_manager.hpp"
#include "../file_reader.hpp"
#include "input/controller.hpp"
#include "input/control.hpp"
#include "level_dot.hpp"

namespace WorldMapNS {

LevelDot::LevelDot(FileReader reader)
  : Dot(reader.read_section("dot")),
    green_dot_sur(Resource::load_sprite("core/worldmap/dot_green")),
    red_dot_sur(Resource::load_sprite("core/worldmap/dot_red")),
    inaccessible_dot_sur(Resource::load_sprite("core/worldmap/dot_invalid")),
    highlight_green_dot_sur(Resource::load_sprite("core/worldmap/dot_green_hl")),
    highlight_red_dot_sur(Resource::load_sprite("core/worldmap/dot_red_hl"))
{
  std::string resname;
  reader.read_string("levelname", resname); 

  plf = PLFResMgr::load_plf(resname);
}

void
LevelDot::draw(DrawingContext& gc)
{
  Vector3f mpos
    = gc.screen_to_world(Vector3f(Input::Controller::current()->get_pointer(Input::STANDARD_POINTER)->get_pos()));

  float x = mpos.x - pos.x;
  float y = mpos.y - pos.y;

  bool highlight = false;

  if (sqrt(x*x + y*y) < 30.0f)
    highlight = true;

  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame
      && (savegame->get_status() == Savegame::FINISHED
          || savegame->get_status() == Savegame::ACCESSIBLE))
    {
      if (savegame->get_status() == Savegame::FINISHED)
        if (highlight)
          {
            gc.draw (highlight_green_dot_sur, pos);
          }
        else
          {
            gc.draw (green_dot_sur, pos);
          }
      else
        if (highlight)
          gc.draw (highlight_red_dot_sur, pos);
        else
          gc.draw (red_dot_sur, pos);
    }
  else
    {
      gc.draw (inaccessible_dot_sur, pos);
    }
}

void
LevelDot::update(float delta)
{
  UNUSED_ARG(delta);
}

void
LevelDot::on_click()
{
  //std::cout << "Starting level: " << levelname << std::endl;
  ScreenManager::instance()->push_screen(new StartScreen(plf),
                                         true);
}

bool
LevelDot::finished()
{
  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame && savegame->get_status() == Savegame::FINISHED)
    return true;
  else
    return false;
}

bool
LevelDot::accessible()
{
  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame && savegame->get_status() != Savegame::NONE)
    return true;
  else
    return false;
}


void
LevelDot::draw_hover(DrawingContext& gc)
{
  int pos_correction = 0;

  if (accessible())
    {
      int length = Fonts::pingus_small.bounding_rect(0, 0, _(get_plf().get_levelname())).get_width() / 2;
      int realpos = static_cast<int>(gc.world_to_screen(Vector3f(pos.x, pos.y, 0)).x);
      if (realpos - length < 0)
        pos_correction = realpos - length;
      else if (realpos + length > gc.get_width())
        pos_correction = realpos + length - static_cast<int>(gc.get_width());
      
      gc.print_center(Fonts::pingus_small,
                      (int)pos.x - pos_correction,
                      (int)pos.y - 44,
                      _(get_plf().get_levelname()), 
                      10000);
    }
  else
    {
      int length  = Fonts::pingus_small.bounding_rect(0, 0, _("locked")).get_width() / 2;
      int realpos = static_cast<int>(gc.world_to_screen(Vector3f(pos.x, pos.y, 0)).x);
      if (realpos - length < 0)
        pos_correction = realpos - length;
      else if (realpos + length > gc.get_width())
        pos_correction = realpos + length - static_cast<int>(gc.get_width());
        
      gc.print_center(Fonts::pingus_small,
                      (int)pos.x - pos_correction,
                      (int)pos.y - 30,
                      _("locked"), 
                      10000);
    }

  if (maintainer_mode)
    {
      gc.print_center(Fonts::pingus_small,
                      (int)pos.x, (int)pos.y - 56,
                      get_plf().get_resname(), 
                      10000);
    }
}

void
LevelDot::unlock()
{
  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame == 0 || savegame->get_status() == Savegame::NONE)
    {
      Savegame savegame(plf.get_resname(),
                        Savegame::ACCESSIBLE,
                        0,
                        0);
      SavegameManager::instance()->store(savegame);
    }
  else
    {
    }
}

} // namespace WorldMapNS

/* EOF */
