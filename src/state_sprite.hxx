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

#ifndef HEADER_STATE_SPRITE_HXX
#define HEADER_STATE_SPRITE_HXX

#include <map>
#include <ClanLib/Display/sprite.h>


/** Simple Wrapper class around CL_Sprite that allows to have multiple
    sprites for different states (running, walking, left, right) */
class StateSprite
{
private:
  typedef std::map<int, CL_Sprite> Sprites;
  Sprites sprites;

public:
  StateSprite();
  
  void update(float delta);
  void update();

  void load(int state, const std::string& name);
  void load(int state, CL_Sprite);
  CL_Sprite& operator[](int state);
};


#endif

/* EOF */
