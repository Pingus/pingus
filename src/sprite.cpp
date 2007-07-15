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
#include "pixel_buffer.hpp"
#include "sprite_description.hpp"

class SpriteImpl
{
public:
  SDL_Surface* surface;

  Vector2i offset;

  Vector2i frame_pos;
  Size     frame_size;
  int      frame_delay;

  Size     array;

  bool     loop;
  bool     loop_last_cycle;
  bool     finished;

  /** Current frame */
  int frame; 
  int tick_count;

  SpriteImpl(const SpriteDescription& desc)
    : surface(0),
      finished(false),
      frame(0),
      tick_count(0)
  {
    surface = IMG_Load(desc.filename.c_str());
    if (!surface)
      {
        std::cout << "Error: Couldn't load " << desc.filename << std::endl;
        surface = IMG_Load("data/images/core/misc/404.png");
        assert(surface);
      }
    
    frame_pos = desc.frame_pos;

    frame_size.width  = (desc.frame_size.width  == -1) ? surface->w : desc.frame_size.width;
    frame_size.height = (desc.frame_size.height == -1) ? surface->h : desc.frame_size.height;

    frame_delay  = desc.speed;

    array = desc.array;

    loop = desc.loop;
    loop_last_cycle = false;

    offset = calc_origin(desc.origin, frame_size) - desc.offset;
  }

  SpriteImpl(const PixelBuffer& pixelbuffer)
    : offset(0,0),
      frame_pos(0,0),
      frame_size(pixelbuffer.get_width(), pixelbuffer.get_height()),
      frame_delay(0),
      array(1,1),
      loop(true),
      loop_last_cycle(false),
      finished(false),
      frame(0),
      tick_count(0)
  {
    if (pixelbuffer.get_surface())
      {
        surface = SDL_DisplayFormatAlpha(pixelbuffer.get_surface());
      }
    else
      {
        surface = 0;
        std::cout << "Sprite: Error trying to create a Sprite out of an empty PixelBuffer"  << std::endl;
      }
  }

  ~SpriteImpl()
  {
    SDL_FreeSurface(surface);
  }

  void update(float delta)
  {
    if (finished)
      return;

    int total_time = frame_delay * (array.width * array.height);
    tick_count += int(delta * 1000.0f);
    if (tick_count >= total_time)
      {
	if (loop)
	  {
	    loop_last_cycle = true;
	    tick_count = tick_count % total_time;
	    frame = tick_count / frame_delay;
	  }
	else
	  {
	    finished = true;
	  }
      }
    else
      {
	loop_last_cycle = false;
	frame = tick_count / frame_delay;
      }
  }

  void draw(float x, float y, SDL_Surface* dst)
  {
    SDL_Rect dstrect;
    dstrect.x = (Sint16)(x - offset.x);
    dstrect.y = (Sint16)(y - offset.y);
    dstrect.w = 0;
    dstrect.h = 0;  

    SDL_Rect srcrect;
    srcrect.w = frame_size.width;
    srcrect.h = frame_size.height;

    srcrect.x = frame_pos.x + (srcrect.w * (frame%array.width));
    srcrect.y = frame_pos.y + (srcrect.h * (frame/array.width));

    SDL_BlitSurface(surface, &srcrect, dst, &dstrect);
  }

  void restart()
  {
    finished = false;
    loop_last_cycle = false;
    frame = 0;
    tick_count = 0;
  }

  void finish()
  {
    finished = true;
  }
};

Sprite::Sprite()
{
}

Sprite::Sprite(const PixelBuffer& pixelbuffer)
  : impl(new SpriteImpl(pixelbuffer))
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
  if (impl.get())
    impl->draw(x, y, target);
}

int
Sprite::get_width()
{
  if (impl.get()) 
    return impl->frame_size.width;
  else
    return 0;
}

int
Sprite::get_height()
{
  if (impl.get()) 
    return impl->frame_size.height;
  else
    return 0;
}

Sprite::operator bool()
{
  return (impl.get() != 0);
}

void
Sprite::update(float delta)
{
  if (impl.get())
    impl->update(delta);
}

void
Sprite::set_frame(int i)
{
  if (impl.get())
    impl->frame = i;
}

int
Sprite::get_frame_count() const
{
  if (impl.get())
    return (impl->array.width * impl->array.height);
  else
    return 0;
}

bool
Sprite::is_finished() const
{
  if (impl.get())
    return impl->finished;
  else
    return true;
}

bool
Sprite::is_looping() const
{
  if (impl.get())
    return impl->loop_last_cycle;
  else
    return false;
}

void
Sprite::set_play_loop(bool loop)
{
  if (impl.get())
    impl->loop = loop;
}

int
Sprite::get_current_frame() const
{
  if (impl.get())
    return impl->frame;
  else
    return 0;
}

void
Sprite::restart()
{
  if (impl.get())
    impl->restart();
}

void
Sprite::finish()
{
  if (impl.get())
    impl->finish();
}

/* EOF */
