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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_FONT_TEST_SCREEN_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_FONT_TEST_SCREEN_HPP

#include "engine/display/font.hpp"
#include "engine/screen/screen.hpp"

class Pathname;

class FontTestScreen : public Screen
{
private:
  Font font;
  Font reference;
  Vector2i scroll;
  bool  dark;

public:
  FontTestScreen(const Pathname& fontfile);

  void draw(DrawingContext& gc);
  void update(const Input::Event& event);
  void update(float delta) {}

private:
  FontTestScreen (const FontTestScreen&);
  FontTestScreen& operator= (const FontTestScreen&);
};

#endif

/* EOF */
