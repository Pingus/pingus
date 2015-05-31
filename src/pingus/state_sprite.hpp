// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_STATE_SPRITE_HPP
#define HEADER_PINGUS_PINGUS_STATE_SPRITE_HPP

#include <map>

#include "engine/display/sprite.hpp"

/** Simple Wrapper class around Sprite that allows to have multiple
    sprites for different states (running, walking, left, right) */
class StateSprite
{
private:
  typedef std::map<int, Sprite> Sprites;
  Sprites sprites;

public:
  StateSprite();

  void update(float delta);
  void update();

  void load(int state, const std::string& name);
  void load(int state, Sprite);
  Sprite& operator[](int state);
};

#endif

/* EOF */
