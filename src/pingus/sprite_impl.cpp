//  Pingus - A free Lemmings clone
//  Copyright (C) 2005-2008 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus/sprite_impl.hpp"

#include <map>
#include "engine/display/framebuffer.hpp"
#include "engine/display/display.hpp"
#include "pingus/sprite_description.hpp"

typedef std::map<std::string, FramebufferSurface> SurfaceCache;
SurfaceCache surface_cache;

FramebufferSurface load_framebuffer_surface(const Pathname& filename, ResourceModifierNS::ResourceModifier mod)
{
  SurfaceCache::iterator i = surface_cache.find(filename.get_sys_path());
  
  if (i == surface_cache.end())
    {

      Surface surface(filename);
      if (mod != ResourceModifierNS::ROT0)
        surface = surface.mod(mod);

      if (!surface)
        {
          std::cout << "Error: Sprite: couldn't load '" << filename << "'" << std::endl;
          surface = Surface(Pathname("images/core/misc/404.png", Pathname::DATA_PATH));
          if (!surface) assert(!"Surface Couldn't find 404");
        }

      FramebufferSurface framebuffer_surface = Display::get_framebuffer().create_surface(surface);

      surface_cache[filename.get_sys_path()] = framebuffer_surface;

      return framebuffer_surface;
    }
  else
    {
      //std::cout << "Sharing: " << filename.get_sys_path() << std::endl;
      return i->second;
    }
}

void delete_framebuffer_surface(const Pathname& filename)
{
  SurfaceCache::iterator i = surface_cache.find(filename.get_sys_path());
  if (i != surface_cache.end())
    {
      //std::cout << "UseCount for " << filename << ": " << i->second.use_count() << std::endl;
      if (i->second.use_count() == 1)
        {
          surface_cache.erase(i);
        }
    }
}

SpriteImpl::SpriteImpl() :
  filename(),
  framebuffer_surface(),
  offset(),
  frame_pos(),
  frame_size(),
  frame_delay(),
  array(),
  loop(),
  loop_last_cycle(),
  finished(),
  frame(),
  tick_count()
{
}

SpriteImpl::SpriteImpl(const SpriteDescription& desc, ResourceModifierNS::ResourceModifier mod) :
  filename(desc.filename),
  framebuffer_surface(),
  offset(),
  frame_pos(),
  frame_size(),
  frame_delay(),
  array(),
  loop(),
  loop_last_cycle(),
  finished(false),
  frame(0),
  tick_count(0)
{
  framebuffer_surface = load_framebuffer_surface(filename, mod);

  frame_pos = desc.frame_pos;

  array = desc.array;

  frame_size.width  = (desc.frame_size.width  == -1) ? framebuffer_surface.get_width()/array.width   : desc.frame_size.width;
  frame_size.height = (desc.frame_size.height == -1) ? framebuffer_surface.get_height()/array.height : desc.frame_size.height;

  frame_delay  = desc.speed;

  loop = desc.loop;
  loop_last_cycle = false;

  offset = calc_origin(desc.origin, frame_size) - desc.offset;
    
}

SpriteImpl::SpriteImpl(const Surface& surface) :
  filename(),
  framebuffer_surface(Display::get_framebuffer().create_surface(surface)),
  offset(0,0),
  frame_pos(0,0),
  frame_size(surface.get_width(), surface.get_height()),
  frame_delay(0),
  array(1,1),
  loop(true),
  loop_last_cycle(false),
  finished(false),
  frame(0),
  tick_count(0)
{
}

SpriteImpl::~SpriteImpl()
{
  if (!filename.empty())
    {
      framebuffer_surface = FramebufferSurface();
      delete_framebuffer_surface(filename);
    }
}

void
SpriteImpl::update(float delta)
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

void 
SpriteImpl::render(int x, int y, Framebuffer& fb)
{
  fb.draw_surface(framebuffer_surface,
                  Rect(frame_pos + Vector2i(frame_size.width  * (frame%array.width),
                                            frame_size.height * (frame/array.width)),
                       frame_size),
                  Vector2i(x - offset.x, y - offset.y));
}

void
SpriteImpl::restart()
{
  finished = false;
  loop_last_cycle = false;
  frame = 0;
  tick_count = 0;
}

void
SpriteImpl::finish()
{
  finished = true;
}

Surface
SpriteImpl::to_surface() const
{
  return framebuffer_surface.to_surface();
}

/* EOF */
