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
#ifdef HAVE_OPENGL
#  include "engine/display/opengl/opengl_framebuffer.hpp"
#endif
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

  // FIXME: Calling this causes horrible flicker, since the screen
  // goes black on a size change. Seems to be an SDL issue.
  // This call  also shouldn't be part of ScreenManager, but Framebuffer/Display internal
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

void
Display::create_window(FramebufferType framebuffer_type, const Size& size, bool fullscreen, bool resizable)
{
  assert(!s_framebuffer.get());

  log_info("%1% %2% %3%", framebuffer_type_to_string(framebuffer_type), size, (fullscreen?"fullscreen":"window"));

  switch (framebuffer_type)
  {
    case OPENGL_FRAMEBUFFER:
#ifdef HAVE_OPENGL
      s_framebuffer = std::unique_ptr<Framebuffer>(new OpenGLFramebuffer());
      s_framebuffer->set_video_mode(size, fullscreen, resizable);
#else
      raise_exception(std::runtime_error, "OpenGL support was not compiled in");
#endif
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
#ifdef OLD_SDL1
  SDL_Rect** modes = SDL_ListModes(NULL, SDL_WINDOW_FULLSCREEN);

  if (modes == static_cast<SDL_Rect**>(0))
  { // No resolutions at all available, bad
    return size;
  }
  else if(modes == reinterpret_cast<SDL_Rect**>(-1))
  {  
    return size;
  }
  else 
  {
    // FIXME: This might not work that well with different aspect ratios
    int distance = -1;
    Size best_fit = size;
      
    for(int i = 0; modes[i]; ++i)
    {
      int this_distance = abs(size.width - modes[i]->w) + abs(size.height - modes[i]->h);
          
      if (distance == -1 || distance > this_distance)
      {
        distance = this_distance;

        best_fit.width  = modes[i]->w;
        best_fit.height = modes[i]->h;
      }
    }

    return best_fit;
  }
#else
  return Size(800, 600);
#endif
}

struct SortBySize
{
  bool operator()(const Size& lhs, const Size& rhs)
  {
    return lhs.get_area() < rhs.get_area();
  }
};

std::vector<Size>
Display::get_fullscreen_video_modes()
{
#ifdef OLD_SDL1
  std::vector<Size> video_modes;  
  SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN);

  if (modes == reinterpret_cast<SDL_Rect**>(0))
  { // No resolutions at all available, bad
      
  }
  else if(modes == reinterpret_cast<SDL_Rect**>(-1))
  {  // FIXME: Under which OSs is this triggred, if ever?
    log_warn("falling back to hardcoded list of screen resolutions");

    // All resolutions should work, so we fall back to hardcoded defaults
    video_modes.push_back(Size( 640, 480)); // 4:3, VGA
    video_modes.push_back(Size( 800, 600)); // 4:3, PAL
    video_modes.push_back(Size(1024, 768)); // Nokia N770, N800
    video_modes.push_back(Size(1152, 864)); // 4:3, SVGA
    video_modes.push_back(Size(1280, 720)); // 16:9
    video_modes.push_back(Size(1280, 800)); // 16:10
    video_modes.push_back(Size(1280, 960)); // 4:3, XGA
    video_modes.push_back(Size(1280, 1024)); // 5:4
    video_modes.push_back(Size(1366,  768)); // ~16:9, Wide XGA
    video_modes.push_back(Size(1440, 900)); // 16:10
    video_modes.push_back(Size(1680, 1050)); // 16:10
    video_modes.push_back(Size(1600, 1200)); // 4:3, UXGA
    video_modes.push_back(Size(1920, 1080)); // 16:9, HD-TV, 1080p
    video_modes.push_back(Size(1920, 1200)); // 16:10
  }
  else 
  {
    for(int i = 0; modes[i]; ++i)
    {
      video_modes.push_back(Size(modes[i]->w,  modes[i]->h));
    }
  }

  std::sort(video_modes.begin(), video_modes.end(), SortBySize());

  return video_modes;
#else
  return std::vector<Size>{{800, 600}};
#endif
}

/* EOF */
