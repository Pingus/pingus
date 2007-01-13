/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <iostream>
#include <sstream>
#include <assert.h>
#include "SDL.h"
#include "SDL_image.h"
#include "sprite.hpp"

class SpriteImpl
{
public:
  SDL_Surface* surface;

  SpriteImpl(const std::string& name) 
  {
    std::ostringstream str;
    str << "data/images/" << name << ".png";
    surface = IMG_Load(str.str().c_str());
    if (!surface)
      {
        std::cout << "Error: Couldn't load " << str.str() << std::endl;
        surface = IMG_Load("data/images/core/misc/404.png");
        assert(surface);
      }
    else
      {
        std::cout << "Loaded sprite: " << name << std::endl;
      }
  }
};

Sprite::Sprite()
  : impl(0)
{
}

Sprite::Sprite(const std::string& name)
  : impl(new SpriteImpl(name))
{  
}

Sprite::~Sprite()
{
}

void
Sprite::draw(float x, float y, SDL_Surface* target)
{
  //std::cout << "Sprite: draw; " << x << ", " << y << std::endl;
  SDL_Rect pos;
  pos.x = (Sint16)x;
  pos.y = (Sint16)y;
  pos.w = 0;
  pos.h = 0;
  SDL_BlitSurface(impl->surface, NULL, target, &pos);
}

int
Sprite::get_width()
{
  return 0;
}

int
Sprite::get_height()
{
  return 0;
}

/* EOF */
