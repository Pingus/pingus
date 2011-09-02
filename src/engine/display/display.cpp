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

#include "engine/display/sdl_framebuffer.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/globals.hpp"
#ifdef HAVE_OPENGL
#  include "engine/display/opengl/opengl_framebuffer.hpp"
#endif
#include "engine/display/delta_framebuffer.hpp"
#include "engine/display/null_framebuffer.hpp"

std::unique_ptr<Framebuffer> Display::framebuffer;

void
Display::flip_display()
{
  return framebuffer->flip();
}

int
Display::get_width()
{
  return framebuffer.get() ? framebuffer->get_size().width : 0;
}

int
Display::get_height()
{
  return framebuffer.get() ? framebuffer->get_size().height : 0;
}

Size
Display::get_size()
{
  return framebuffer.get() ? framebuffer->get_size() : Size(0, 0);
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
  Display::set_video_mode(size, globals::fullscreen_enabled);

  if (ScreenManager::instance())
    ScreenManager::instance()->resize(size);
}

void
Display::set_video_mode(const Size& size, bool fullscreen)
{
  if (!framebuffer.get())
  {
    switch (globals::framebuffer_type)
    {
      case OPENGL_FRAMEBUFFER:
#ifdef HAVE_OPENGL
        framebuffer = std::unique_ptr<Framebuffer>(new OpenGLFramebuffer());
#else
        throw std::runtime_error("OpenGL support was not compiled in");
#endif
        break;

      case NULL_FRAMEBUFFER:
        framebuffer = std::unique_ptr<Framebuffer>(new NullFramebuffer());
        break;

      case DELTA_FRAMEBUFFER:
        globals::static_graphics = true;
        framebuffer = std::unique_ptr<Framebuffer>(new DeltaFramebuffer());
        break;

      case SDL_FRAMEBUFFER:
        framebuffer = std::unique_ptr<Framebuffer>(new SDLFramebuffer());
        break;
          
      default:
        assert(!"Unknown framebuffer_type");
        break;
    }
  }

  if (fullscreen)
  {
    Size new_size = find_closest_fullscreen_video_mode(size);
    framebuffer->set_video_mode(new_size, fullscreen);
  }
  else
  {
    framebuffer->set_video_mode(size, fullscreen);
  }
  
  if (ScreenManager::instance())
    ScreenManager::instance()->resize(framebuffer->get_size());
}

Framebuffer&
Display::get_framebuffer()
{
  return *framebuffer.get(); 
}

Size
Display::find_closest_fullscreen_video_mode(const Size& size)
{
  SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN);

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
          
      //std::cout << "Mode: " << size << " -> " << modes[i]->w << "x" << modes[i]->h << " " << this_distance << std::endl;        

      if (distance == -1 || distance > this_distance)
      {
        distance = this_distance;

        best_fit.width  = modes[i]->w;
        best_fit.height = modes[i]->h;
      }
    }

    return best_fit;
  }

}

std::vector<Size>
Display::get_fullscreen_video_modes()
{
  std::vector<Size> video_modes;  
  SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN);

  if (modes == reinterpret_cast<SDL_Rect**>(0))
  { // No resolutions at all available, bad
      
  }
  else if(modes == reinterpret_cast<SDL_Rect**>(-1))
  {  // FIXME: Under which OSs is this triggred, if ever?

    // All resolutions should work, so we fall back to hardcoded defaults
    video_modes.push_back(Size( 640, 480));
    video_modes.push_back(Size( 800, 600));
    video_modes.push_back(Size(1024, 768));
    video_modes.push_back(Size(1152, 864));
    video_modes.push_back(Size(1280, 960));
    video_modes.push_back(Size(1280, 1024));
    video_modes.push_back(Size(1440, 900));
    video_modes.push_back(Size(1680, 1050));
    video_modes.push_back(Size(1600, 1200));
    video_modes.push_back(Size(1920, 1080));
    video_modes.push_back(Size(1920, 1200));
  }
  else 
  {
    for(int i = 0; modes[i]; ++i)
      video_modes.push_back(Size(modes[i]->w,  modes[i]->h));
  }

  return video_modes;
}

/* EOF */
