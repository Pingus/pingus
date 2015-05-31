// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_SCREENS_LEVEL_MENU_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_LEVEL_MENU_HPP

#include "engine/display/sprite.hpp"
#include "engine/screen/gui_screen.hpp"
#include "pingus/levelset.hpp"

namespace GUI {
class SurfaceButton;
} // namespace GUI

class LevelSelector;
class LevelsetSelector;

class LevelMenu : public GUIScreen
{
private:
  int x_pos;
  int y_pos;

  Sprite background;
  Sprite blackboard;
  Sprite ok_button;

  LevelSelector*    level_selector;
  LevelsetSelector* levelset_selector;

  GUI::SurfaceButton* abort_button;
  GUI::SurfaceButton* next_button;
  GUI::SurfaceButton* prev_button;

public:
  LevelMenu();
  ~LevelMenu();

  void draw_background(DrawingContext& gc);
  void on_escape_press();
  void on_action_up_press();
  void on_action_down_press();

  void set_levelset(Levelset* levelset);

  void next_page();
  void prev_page();

  void resize(const Size& size);

private:
  LevelMenu (const LevelMenu&);
  LevelMenu& operator= (const LevelMenu&);
};

#endif

/* EOF */
