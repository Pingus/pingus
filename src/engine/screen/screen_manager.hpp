// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_SCREEN_SCREEN_MANAGER_HPP
#define HEADER_PINGUS_ENGINE_SCREEN_SCREEN_MANAGER_HPP

#include <memory>
#include <vector>
#include <geom/fwd.hpp>

#include "engine/input/controller.hpp"
#include "engine/display/sprite.hpp"
#include "math/vector2f.hpp"
#include "fwd.hpp"

namespace pingus {

typedef std::shared_ptr<Screen> ScreenPtr;

class ScreenManager
{
private:
  static ScreenManager* instance_;

private:
  pingus::input::Manager& input_manager;
  pingus::input::ControllerPtr input_controller;

  std::unique_ptr<DrawingContext> display_gc;

  std::unique_ptr<FPSCounter> fps_counter;
  Sprite cursor;

  /** Screen stack (first is the screen, second is delete_screen,
      which tells if the screen should be deleted onces it got poped
      or replaced) */
  std::vector<ScreenPtr> screens;

  Vector2i mouse_pos;

  bool record_input;
  bool playback_input;

public:
  ScreenManager(pingus::input::Manager& input_manager,
                pingus::input::ControllerPtr arg_input_controller);
  ~ScreenManager();

  void resize(geom::isize const& size);

  /** Start the screen manager and let it take control, this will
      not return until the somebody signals a quit() */
  void display();

  void update(float delta, std::vector<pingus::input::Event> const& events);

  /** Replace the current screen */
  void replace_screen(ScreenPtr screen);

  /** Add a screen on top of another screen */
  void push_screen(ScreenPtr const& screen);

  /** Remove the current screen and fall back to the last one */
  void pop_screen();

  /** Remove all screens */
  void pop_all_screens();

  void show_software_cursor(bool v);

  /** @return a pointer to the current Screen */
  ScreenPtr get_current_screen();

private:
  void process_events();

  /** FadeOver test*/
  void fade_over(ScreenPtr const& old_screen, ScreenPtr const& new_screen);

public:
  static ScreenManager* instance();

private:
  ScreenManager (ScreenManager const&);
  ScreenManager& operator= (ScreenManager const&);
};

} // namespace pingus

#endif

/* EOF */
