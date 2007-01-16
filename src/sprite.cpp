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
#include "math/origin.hpp"
#include "math/vector2i.hpp"
#include "SDL_image.h"
#include "sprite.hpp"
#include "sprite_description.hpp"

class SpriteImpl
{
public:
  SDL_Surface* surface;
  Vector2i     offset;
  Origin       origin;

  SpriteImpl(const SpriteDescription& desc)
  {
    origin = desc.origin;
    offset = desc.offset;
    load(desc.filename);
  }

  SpriteImpl(const std::string& name) 
  {
    std::ostringstream str;
    str << "data/images/" << name << ".png";

    load(str.str());
  }

  void 
  load(const std::string& filename)
  {
    surface = IMG_Load(filename.c_str());
    if (!surface)
      {
        std::cout << "Error: Couldn't load " << filename << std::endl;
        surface = IMG_Load("data/images/core/misc/404.png");
        assert(surface);
      }
    else
      {
        std::cout << "Loaded sprite: " << filename << std::endl;
      }
    
    offset = calc_origin(origin, Size(surface->w, surface->h)) + offset;

    std::cout << "offset: " << offset.x << ", " << offset.y << std::endl;
  }

  ~SpriteImpl()
  {
    SDL_FreeSurface(surface);
  }

  void draw(float x, float y, SDL_Surface* target)
  {
    SDL_Rect pos;
    
    pos.x = (Sint16)(x - offset.x);
    pos.y = (Sint16)(y - offset.y);
    pos.w = 0;
    pos.h = 0;
    
    SDL_BlitSurface(surface, NULL, target, &pos);
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

Sprite::Sprite(const SpriteDescription& desc)
  : impl(new SpriteImpl(desc))
{
}

Sprite::~Sprite()
{
}

void
Sprite::draw(float x, float y, SDL_Surface* target)
{
  //std::cout << "Sprite: draw; " << x << ", " << y << std::endl;
  impl->draw(x, y, target);
}

int
Sprite::get_width()
{
  return impl->surface->w;
}

int
Sprite::get_height()
{
  return impl->surface->h;
}

Sprite::operator bool()
{
  return impl.get();
}

void
Sprite:: update(float delta)
{
}

void
Sprite::set_frame(int i)
{
  
}

int
Sprite::get_frame_count() const
{
  return 1;
}

bool
Sprite::is_finished() const
{
  return true;
}

/* EOF */
