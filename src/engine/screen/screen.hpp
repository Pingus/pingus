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

#ifndef HEADER_PINGUS_ENGINE_SCREEN_SCREEN_HPP
#define HEADER_PINGUS_ENGINE_SCREEN_SCREEN_HPP

#include "math/size.hpp"

namespace Input {
struct Event;
}

class DrawingContext;

/** A interface for screens. A screen is a Pingus 'thing' which gets
    complete controll over the display and input. Examples of
    screens are the PingusMenu or a PingusGameSession */
class Screen
{
protected:
  Size size;

public:
  Screen (const Size& size_) : size(size_) { }
  virtual ~Screen () {}

  /** Draw this screen */
  virtual void draw(DrawingContext& gc) =0;

  /** Pass a delta to the screen */
  virtual void update (const Input::Event& event) =0;
  virtual void update (float delta) =0;

  /** Called once the screen gets activated and becomes the current
      screen */
  virtual void on_startup () {}

  virtual void resize(const Size& s) { size = s; }

  virtual Size get_size() const { return size; }

private:
  Screen (const Screen&);
  Screen& operator= (const Screen&);
};

#endif

/* EOF */
