//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <sstream>
#include <assert.h>
#include "SDL.h"
#include "math/origin.hpp"
#include "math/vector2i.hpp"
#include "SDL_image.h"
#include "sprite.hpp"
#include "blitter.hpp"
#include "surface.hpp"
#include "pathname.hpp"
#include "resource.hpp"
#include "sprite_impl.hpp"
#include "sprite_description.hpp"

Sprite::Sprite()
{
}

Sprite::Sprite(const std::string& name)
{
  SpriteDescription* desc = Resource::load_sprite_desc(name);
  if (desc)
    {
      impl = boost::shared_ptr<SpriteImpl>(new SpriteImpl(*desc));
    }
  else
    {
      SpriteDescription desc;
      desc.filename = Pathname("images/core/misc/404.png", Pathname::DATA_PATH);
      impl = boost::shared_ptr<SpriteImpl>(new SpriteImpl(desc));
    }
}

Sprite::Sprite(const ResDescriptor& res_desc)
{
  SpriteDescription* desc = Resource::load_sprite_desc(res_desc.res_name);
  if (desc)
    {
      impl = boost::shared_ptr<SpriteImpl>(new SpriteImpl(*desc, res_desc.modifier));
    }
  else
    {
      SpriteDescription desc;
      desc.filename = Pathname("images/core/misc/404.png", Pathname::DATA_PATH);
      impl = boost::shared_ptr<SpriteImpl>(new SpriteImpl(desc));
    } 
}

Sprite::Sprite(const Pathname& name)
{
  SpriteDescription desc;
  desc.filename = name;
  impl = boost::shared_ptr<SpriteImpl>(new SpriteImpl(desc));
}

Sprite::Sprite(const Surface& surface)
  : impl(new SpriteImpl(surface))
{
  
}

Sprite::Sprite(const SpriteDescription& desc, ResourceModifierNS::ResourceModifier mod)
  : impl(new SpriteImpl(desc, mod))
{
}

Sprite::~Sprite()
{
}

void
Sprite::render(float x, float y, SDL_Surface* target)
{
  if (impl.get())
    impl->render(x, y, target);
}

int
Sprite::get_width() const
{
  if (impl.get()) 
    return impl->frame_size.width;
  else
    return 0;
}

int
Sprite::get_height() const
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
    return impl->surface.get_surface();
  else
    return NULL;
}

void
Sprite::scale(int w, int h)
{
  if (impl.get())
    {
      // FIXME: This doesn't work for animated graphics, in which case it will only handle the first frame
      if (impl->frame_size.width != w || impl->frame_size.height != h)
        {
          boost::shared_ptr<SpriteImpl> new_impl(new SpriteImpl());

      
          if ((impl->frame_size.width  * impl->array.width)  == impl->surface.get_width() && 
              (impl->frame_size.height * impl->array.height) == impl->surface.get_height())
            { // single frame Sprite
              new_impl->surface = impl->surface.scale(w, h);
            }
          else
            { // multi frame sprite
              new_impl->surface = impl->surface.subsection(Rect(impl->frame_pos, impl->frame_size)).scale(w, h);
            }

          float scale_x = float(w) / float(impl->frame_size.width);
          float scale_y = float(h) / float(impl->frame_size.height);
      
          new_impl->offset          = Vector2i(int(impl->offset.x * scale_x),
                                               int(impl->offset.y * scale_y)); 
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
}

void
Sprite::fill(const Color& color)
{
  if (impl.get())
    {
      if (color.a != 0) 
        {
          make_single_user();
          impl->surface.fill(color);
        }
    }
}

void
Sprite::make_single_user()
{
  if (impl.get())
    {
      boost::shared_ptr<SpriteImpl> new_impl(new SpriteImpl());
  
      new_impl->surface         = impl->surface.clone();
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
}

Vector2i
Sprite::get_offset() const
{
  if (impl.get())
    return impl->offset;
  else
    return Vector2i();
}

void
Sprite::set_hotspot(Origin origin, int x, int y)
{
  if (impl.get())
    {
      // FIXME: offset and other stuff should be member of the Sprite, not the SpriteImpl
      impl->offset = calc_origin(origin, impl->frame_size) - Vector2i(x, y);
    }
}

void
Sprite::apply_mod(ResourceModifierNS::ResourceModifier mod)
{
  if (impl.get())
    {
      // FIXME: This isn't all that useful, since Sprites are optimized
      // per default and thus not modifiable, since the Modifier can only
      // handle indexed images.
      if (impl->frame_pos  == Vector2i(0, 0) &&
          impl->frame_size == Size(impl->surface.get_width(), impl->surface.get_height()) &&
          impl->array      == Size(1, 1))
        {
          make_single_user();
          impl->surface = impl->surface.mod(mod);
          impl->frame_size.width  = impl->surface.get_width();
          impl->frame_size.height = impl->surface.get_height();
        }
      else
        {
          std::cout << "Error: Sprite: apply_mod() only works with single frame Sprites" << std::endl;
        }
    }
}

/* EOF */
