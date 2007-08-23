//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include <sstream>
#include <assert.h>
#include "SDL.h"
#include "math/origin.hpp"
#include "math/vector2i.hpp"
#include "SDL_image.h"
#include "sprite.hpp"
#include "blitter.hpp"
#include "pixel_buffer.hpp"
#include "path_manager.hpp"
#include "sprite_description.hpp"

class SpriteImpl
{
private:
  friend class Sprite;

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

public:
  SpriteImpl()
  {
  }

  SpriteImpl(const SpriteDescription& desc)
    : surface(0),
      finished(false),
      frame(0),
      tick_count(0)
  {
    surface = IMG_Load(path_manager.complete(desc.filename).c_str());
    if (!surface)
      {
        std::cout << "Error: Couldn't load " << desc.filename << std::endl;
        surface = IMG_Load(path_manager.complete("images/core/misc/404.png").c_str());
        assert(surface);
      }

    frame_pos = desc.frame_pos;

    array = desc.array;

    frame_size.width  = (desc.frame_size.width  == -1) ? surface->w/array.width  : desc.frame_size.width;
    frame_size.height = (desc.frame_size.height == -1) ? surface->h/array.height : desc.frame_size.height;

    frame_delay  = desc.speed;

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

SDL_Surface*
Sprite::get_surface() const
{
  if (impl.get())
    return impl->surface;
  else
    return NULL;
}

void
Sprite::scale(int w, int h)
{
  if (impl->frame_size.width != w || impl->frame_size.height != h)
    {
      boost::shared_ptr<SpriteImpl> new_impl(new SpriteImpl());

      if ((impl->frame_size.width  * impl->array.width)  == impl->surface->w && 
          (impl->frame_size.height * impl->array.height) == impl->surface->h)
        {
          new_impl->surface = Blitter::scale_surface(impl->surface, 
                                                     w * impl->array.width,
                                                     h * impl->array.height);
        }
      else
        {
          // Create a temporary surface that contains the subsection
          // that is actually used for this Sprite
          SDL_Surface* subsurface = SDL_CreateRGBSurfaceFrom((uint8_t*)(impl->surface->pixels)
                                                             + (impl->frame_pos.y * impl->surface->pitch) 
                                                             + (impl->frame_pos.x * impl->surface->format->BytesPerPixel),
                                                             impl->array.width  * impl->frame_size.width,
                                                             impl->array.height * impl->frame_size.height,
                                                             impl->surface->format->BitsPerPixel, 
                                                             impl->surface->pitch,
                                                             impl->surface->format->Rmask,
                                                             impl->surface->format->Gmask,
                                                             impl->surface->format->Bmask,
                                                             impl->surface->format->Amask);
      
          if (impl->surface->format->palette)
            SDL_SetPalette(subsurface, SDL_LOGPAL, impl->surface->format->palette->colors, 
                           0, impl->surface->format->palette->ncolors);

          if (impl->surface->flags & SDL_SRCCOLORKEY)
            SDL_SetColorKey(subsurface, SDL_SRCCOLORKEY, impl->surface->format->colorkey);

          new_impl->surface = Blitter::scale_surface(subsurface, 
                                                     w * impl->array.width,
                                                     h * impl->array.height);

          SDL_FreeSurface(subsurface);
        }

      float scale_x = float(w) / float(impl->frame_size.width); // ok
      float scale_y = float(h) / float(impl->frame_size.height); // ok
            
      new_impl->offset          = Vector2i(int(impl->offset.x * scale_x),
                                           int(impl->offset.y * scale_y)); //ok
      new_impl->frame_pos       = Vector2i(0, 0);
      new_impl->frame_size      = Size(w, h);
      new_impl->frame_delay     = impl->frame_delay;
      new_impl->array           = impl->array;
      new_impl->loop            = impl->loop;
      new_impl->loop_last_cycle = impl->loop_last_cycle;
      new_impl->finished        = impl->finished;
      new_impl->frame           = impl->frame;
      new_impl->tick_count      = impl->tick_count;

      impl = new_impl;
    }
}

void
Sprite::fill(const Color& color)
{
  if (color.a != 0) 
    {
      make_single_user();

      // FIXME: Couldn't get this to work with a RGBA surface for some
      // reason, something to do with tmp format and impl->surface
      // matching up maybe, anyway with RGB it works and it saves a
      // little bit of space to
      SDL_Surface* tmp = Blitter::create_surface_rgb(impl->surface->w, impl->surface->h);
      SDL_FillRect(tmp, NULL, SDL_MapRGBA(tmp->format, color.r, color.g, color.b, 255));
      SDL_SetAlpha(tmp, SDL_SRCALPHA, color.a);
          
      SDL_BlitSurface(tmp, NULL, impl->surface, NULL);

      SDL_FreeSurface(tmp);
    }
}

void
Sprite::make_single_user()
{
  boost::shared_ptr<SpriteImpl> new_impl(new SpriteImpl());
  
  if (impl->surface->format->Amask == 0)
    new_impl->surface         = Blitter::create_surface_rgb(impl->surface->w, impl->surface->h);
  else
    new_impl->surface         = Blitter::create_surface_rgba(impl->surface->w, impl->surface->h);

  SDL_BlitSurface(impl->surface, NULL, new_impl->surface, NULL);

  new_impl->offset          = impl->offset;
  new_impl->frame_pos       = impl->frame_pos;
  new_impl->frame_size      = impl->frame_size;
  new_impl->frame_delay     = impl->frame_delay;
  new_impl->array           = impl->array;
  new_impl->loop            = impl->loop;
  new_impl->loop_last_cycle = impl->loop_last_cycle;
  new_impl->finished        = impl->finished;
  new_impl->frame           = impl->frame;
  new_impl->tick_count      = impl->tick_count;

  impl = new_impl;  
}

void
Sprite::optimize()
{
  // FIXME: Could add a check to check if the surface is already optimized
  SDL_Surface* new_surface;

  if (impl->surface->format->Amask == 0)
    new_surface = SDL_DisplayFormat(impl->surface);
  else
    new_surface = SDL_DisplayFormatAlpha(impl->surface);
  
  SDL_FreeSurface(impl->surface);

  impl->surface = new_surface;
}

/* EOF */
