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

#ifndef HEADER_PINGUS_SCREEN_MANAGER_HPP
#define HEADER_PINGUS_SCREEN_MANAGER_HPP

#include "../pingus.hpp"
#include <memory>
#include <boost/smart_ptr.hpp>
#include <vector>
#include "math/vector2f.hpp"
#include "sprite.hpp"

namespace Input {
class Manager;
class Controller;
}

class FPSCounter;
class Size;
class DrawingContext;
class Screen;

typedef boost::shared_ptr<Screen> ScreenPtr;

class ScreenManager
{
private:
  static ScreenManager* instance_;

  std::auto_ptr<Input::Manager>    input_manager;
  std::auto_ptr<Input::Controller> input_controller;

  std::auto_ptr<DrawingContext> display_gc;
  
  std::auto_ptr<FPSCounter> fps_counter;
  Sprite cursor;

  /** Screen stack (first is the screen, second is delete_screen,
      which tells if the screen should be deleted onces it got poped
      or replaced) */
  std::vector<ScreenPtr> screens;

  Vector2f mouse_pos;

public:
  ScreenManager ();
  ~ScreenManager();

  void resize(const Size& size);

  /** Start the screen manager and let it take control, this will
      not return until the somebody signals a quit() */
  void display();

  void update(float delta);

  /** Replace the current screen */
  void replace_screen(Screen*);

  /** Add a screen on top of another screen */
  void push_screen(Screen*);

  /** Remove the current screen and fall back to the last one */
  void pop_screen();

  /** Remove all screens */
  void pop_all_screens();

  /** @return a pointer to the current Screen */
  ScreenPtr get_current_screen();

  void show_swcursor(bool v);
  
private:
  /** FadeOver test*/
  void fade_over(ScreenPtr old_screen, ScreenPtr new_screen);

public:
  static ScreenManager* instance();

private:
  ScreenManager (const ScreenManager&);
  ScreenManager& operator= (const ScreenManager&);
};

#endif

/* EOF */
