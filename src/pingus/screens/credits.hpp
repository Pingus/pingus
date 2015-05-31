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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_CREDITS_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_CREDITS_HPP

#include <memory>
#include <string>
#include <vector>

#include "engine/display/font.hpp"
#include "engine/display/scene_context.hpp"
#include "engine/display/sprite.hpp"
#include "engine/screen/gui_screen.hpp"

class Pathname;

class Credits : public GUIScreen
{
private:
  std::unique_ptr<SceneContext> scene_context;

  bool fast_scrolling;
  Sprite background;
  Sprite blackboard;
  Sprite pingu;

  Font    font;
  Font    font_small;

  bool is_init;

  float end_offset;
  float offset;

  /** The string's which are shown in the scrolling text, the first
      character of the string can be used to apply a special format.

      'n' - Inserts a newline
      '_' - Use small font
      '-' - Use large font
  */
  std::vector<std::string> credits;

public:
  Credits(const Pathname& filename);
  virtual ~Credits();

  void update(float);
  void draw_background(DrawingContext& gc);

  void on_startup();

  void on_pause_press();
  void on_fast_forward_press();
  void on_escape_press();

private:
  Credits(const Credits&);
  Credits& operator=(const Credits&);

};

#endif

/* EOF */
