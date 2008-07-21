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
Sprite::render(float x, float y, Framebuffer& fb)
{
  if (impl.get())
    impl->render(x, y, fb);
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

Surface
Sprite::to_surface() const
{
  if (impl.get())
    return impl->to_surface();
  else
    return Surface();
}

/* EOF */
