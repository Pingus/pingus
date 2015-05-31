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

#include "pingus/state_sprite.hpp"

#include <sstream>
#include <stdexcept>

#include "util/raise_exception.hpp"

StateSprite::StateSprite() :
  sprites()
{
}

void
StateSprite::load(int state, const std::string& name)
{
  load(state, Sprite(name));
}

void
StateSprite::load(int state, Sprite sprite)
{
  sprites[state] = sprite;
}

void
StateSprite::update()
{
  update(0.033f);
}

void
StateSprite::update(float delta)
{
  for(Sprites::iterator i = sprites.begin(); i != sprites.end(); ++i)
    (*i).second.update(delta);
}

Sprite&
StateSprite::operator[](int state)
{
  Sprites::iterator i = sprites.find(state);
  if (i != sprites.end())
  {
    return i->second;
  }
  else
  {
    raise_exception(std::runtime_error, "StateSprite error: state " << state << " not available");
  }
}

/* EOF */
