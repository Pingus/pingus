//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "resource.hxx"
#include "pingus_error.hxx"
#include "state_sprite.hxx"


StateSprite::StateSprite()
{
}

void
StateSprite::load(int state, const std::string& name)
{
  load(state, Resource::load_sprite(name));
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
      throw PingusError("StateSprite error");
    }
}


/* EOF */
