//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/display/display.hpp"

#include <stdexcept>
#include <algorithm>

#include "engine/display/sdl_framebuffer.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/globals.hpp"
#include "engine/display/opengl/opengl_framebuffer.hpp"
#include "engine/display/delta/delta_framebuffer.hpp"
#include "engine/display/null_framebuffer.hpp"
#include "util/log.hpp"

std::unique_ptr<Framebuffer> Display::s_framebuffer;

void
Display::flip_display()
{
  return s_framebuffer->flip();
}

int
Display::get_width()
{
  return s_framebuffer.get() ? s_framebuffer->get_size().width : 0;
}

int
Display::get_height()
{
  return s_framebuffer.get() ? s_framebuffer->get_size().height : 0;
}

Size
Display::get_size()
{
  return s_framebuffer.get() ? s_framebuffer->get_size() : Size(0, 0);
}

void
Display::resize(const Size& size_)
{
  Size size(size_);

  // Limit Window size so some reasonable minimum
  if (size.width  < 640) size.width  = 640;
  if (size.height < 480) size.height = 480;

  Display::set_video_mode(size, is_fullscreen(), true);

  if (ScreenManager::instance())
    ScreenManager::instance()->resize(size);
}

bool
Display::is_fullscreen()
{
  return s_framebuffer->is_fullscreen();
}

bool
Display::is_resizable()
{
  return s_framebuffer->is_resizable();
}

bool
Display::has_grab()
{
  return s_framebuffer->has_grab();
}

void
Display::create_window(FramebufferType framebuffer_type, const Size& size, bool fullscreen, bool resizable)
{
  assert(!s_framebuffer.get());

  log_info("%1% %2% %3%", framebuffer_type_to_string(framebuffer_type), size, (fullscreen?"fullscreen":"window"));

  switch (framebuffer_type)
  {
    case OPENGL_FRAMEBUFFER:
      s_framebuffer = std::unique_ptr<Framebuffer>(new OpenGLFramebuffer());
      s_framebuffer->set_video_mode(size, fullscreen, resizable);
      break;

    case NULL_FRAMEBUFFER:
      s_framebuffer = std::unique_ptr<Framebuffer>(new NullFramebuffer());
      s_framebuffer->set_video_mode(size, fullscreen, resizable);
      break;

    case DELTA_FRAMEBUFFER:
      globals::static_graphics = true;
      s_framebuffer = std::unique_ptr<Framebuffer>(new DeltaFramebuffer());
      s_framebuffer->set_video_mode(size, fullscreen, resizable);
      break;

    case SDL_FRAMEBUFFER:
      s_framebuffer = std::unique_ptr<Framebuffer>(new SDLFramebuffer());
      s_framebuffer->set_video_mode(size, fullscreen, resizable);
      break;
          
    default:
      assert(!"Unknown framebuffer_type");
      break;
  }
}

void
Display::set_video_mode(const Size& size, bool fullscreen, bool resizable)
{
  if (fullscreen)
  {
    Size new_size = find_closest_fullscreen_video_mode(size);
    s_framebuffer->set_video_mode(new_size, fullscreen, resizable);
  }
  else
  {
    s_framebuffer->set_video_mode(size, fullscreen, resizable);
  }
  
  if (ScreenManager::instance())
  {
    ScreenManager::instance()->resize(s_framebuffer->get_size());
  }
}

Framebuffer*
Display::get_framebuffer()
{
  return s_framebuffer.get(); 
}

Size
Display::find_closest_fullscreen_video_mode(const Size& size)
{
  SDL_DisplayMode target;
  SDL_DisplayMode closest;
 
  target.w = size.width;
  target.h = size.height;
  target.format = 0;  // don't care
  target.refresh_rate = 0; // don't care
  target.driverdata   = nullptr;

  if (!SDL_GetClosestDisplayMode(0, &target, &closest))
  {
    log_error("couldn't find video mode matching %1%x%1%", size.width, size.height);
    return size;
  }
  else
  {
    return {closest.w, closest.h};
  }
}

std::vector<SDL_DisplayMode>
Display::get_fullscreen_video_modes()
{
  std::vector<SDL_DisplayMode> video_modes;

  int num_displays = SDL_GetNumVideoDisplays();
  log_info("number of displays: %1%", num_displays);

  for(int display = 0; display < num_displays; ++display)
  {
    int num_modes = SDL_GetNumDisplayModes(display);
    
    for (int i = 0; i < num_modes; ++i) 
    {
      SDL_DisplayMode mode;
      if (SDL_GetDisplayMode(display, i, &mode) != 0)
      {
        log_error("failed to get display mode: %1%", SDL_GetError());
      }
      else
      {
        log_debug("%1%x%2%@%3% %4%", mode.w, mode.h, mode.refresh_rate, SDL_GetPixelFormatName(mode.format));
        video_modes.push_back(mode);
      }
    }
  }

  return video_modes;
}

/* EOF */
