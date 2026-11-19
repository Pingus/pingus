// Pingus - A free Lemmings clone
// Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/display/sdl_framebuffer.hpp"

#include <SDL3_image/SDL_image.h>
#include <sstream>
#include <iostream>
#include <string>

#include <logmich/log.hpp>

#include "util/print.hpp"

#include "engine/display/sdl_framebuffer_surface_impl.hpp"

namespace pingus {

namespace {

SDL_Rect Intersection(SDL_Rect* r1, SDL_Rect* r2)
{
  SDL_Rect rect;
  rect.x = std::max(r1->x, r2->x);
  rect.y = std::max(r1->y, r2->y);
  int endx = std::min(r1->x + r1->w, r2->x + r2->w);
  rect.w = std::max(endx - rect.x, 0);
  int endy = std::min(r1->y + r1->h, r2->y + r2->h);
  rect.h = std::max(endy - rect.y, 0);
  return rect;
}

} // namespace

SDLFramebuffer::SDLFramebuffer() :
  m_window(nullptr),
  m_renderer(nullptr),
  cliprect_stack()
{
}

SDLFramebuffer::~SDLFramebuffer()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

FramebufferSurface
SDLFramebuffer::create_surface(Surface const& surface)
{
  return FramebufferSurface(new SDLFramebufferSurfaceImpl(m_renderer, surface.get_surface()));
}

Surface
SDLFramebuffer::make_screenshot() const
{
  int w;
  int h;
  if (!SDL_GetCurrentRenderOutputSize(m_renderer, &w, &h))
  {
    log_error("SDL_GetCurrentRenderOutputSize failed: {}", SDL_GetError());
    return Surface();
  }

  SDL_Surface* pixels = SDL_RenderReadPixels(m_renderer, nullptr);
  if (!pixels)
  {
    log_error("SDL_RenderReadPixels failed: {}", SDL_GetError());
    return Surface();
  }

  Surface screenshot(w, h);
  if (!SDL_ConvertPixels(pixels->w, pixels->h,
                         pixels->format, pixels->pixels, pixels->pitch,
                         SDL_PIXELFORMAT_ABGR8888,
                         screenshot.get_data(), screenshot.get_pitch()))
  {
    log_error("SDL_ConvertPixels failed: {}", SDL_GetError());
  }
  SDL_DestroySurface(pixels);
  return screenshot;
}

void
SDLFramebuffer::draw_surface(FramebufferSurface const& surface, geom::ipoint const& pos)
{
  SDLFramebufferSurfaceImpl* impl = dynamic_cast<SDLFramebufferSurfaceImpl*>(surface.get_impl());
  SDL_Texture* texture = impl->get_texture();

  SDL_FRect dstrect;
  dstrect.x = static_cast<float>(pos.x());
  dstrect.y = static_cast<float>(pos.y());
  dstrect.w = static_cast<float>(surface.get_width());
  dstrect.h = static_cast<float>(surface.get_height());

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderTexture(m_renderer, texture, nullptr, &dstrect);
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_surface(FramebufferSurface const& surface, geom::irect const& srcrect, geom::ipoint const& pos)
{
  SDLFramebufferSurfaceImpl* impl = dynamic_cast<SDLFramebufferSurfaceImpl*>(surface.get_impl());
  SDL_Texture* texture = impl->get_texture();

  SDL_FRect dstrect;
  dstrect.x = static_cast<float>(pos.x());
  dstrect.y = static_cast<float>(pos.y());
  dstrect.w = static_cast<float>(srcrect.width());
  dstrect.h = static_cast<float>(srcrect.height());

  SDL_FRect sdlsrcrect;
  sdlsrcrect.x = static_cast<float>(srcrect.left());
  sdlsrcrect.y = static_cast<float>(srcrect.top());
  sdlsrcrect.w = static_cast<float>(srcrect.width());
  sdlsrcrect.h = static_cast<float>(srcrect.height());

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderTexture(m_renderer, texture, &sdlsrcrect, &dstrect);
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_line(geom::ipoint const& pos1, geom::ipoint const& pos2, Color const& color)
{
  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderLine(m_renderer,
                 static_cast<float>(pos1.x()), static_cast<float>(pos1.y()),
                 static_cast<float>(pos2.x()), static_cast<float>(pos2.y()));
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_rect(geom::irect const& rect_, Color const& color)
{
  geom::irect rect = geom::normalize(rect_);

  SDL_FRect sdl_rect;
  sdl_rect.x = static_cast<float>(rect.left());
  sdl_rect.y = static_cast<float>(rect.top());
  sdl_rect.w = static_cast<float>(rect.width());
  sdl_rect.h = static_cast<float>(rect.height());

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderRect(m_renderer, &sdl_rect);
  // FIXME: pop render state
}

void
SDLFramebuffer::fill_rect(geom::irect const& rect_, Color const& color)
{
  geom::irect rect = geom::normalize(rect_);

  SDL_FRect sdl_rect;
  sdl_rect.x = static_cast<float>(rect.left());
  sdl_rect.y = static_cast<float>(rect.top());
  sdl_rect.w = static_cast<float>(rect.width());
  sdl_rect.h = static_cast<float>(rect.height());

  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(m_renderer, &sdl_rect);
}

void
SDLFramebuffer::flip()
{
  SDL_RenderPresent(m_renderer);
}

void
SDLFramebuffer::update_rects(std::vector<geom::irect> const& /* rects */)
{
  flip();
}

geom::isize
SDLFramebuffer::get_size() const
{
  int w;
  int h;
  SDL_GetWindowSize(m_window, &w, &h);
  return geom::isize(w, h);
}

bool
SDLFramebuffer::set_video_mode(geom::isize const& size, bool fullscreen, bool resizable)
{
  if (m_window)
  {
    if (!fullscreen)
    {
      SDL_SetWindowSize(m_window, size.width(), size.height());
      SDL_SetWindowFullscreen(m_window, false);
    }
    else
    {
      SDL_DisplayMode mode;
      if (SDL_GetClosestFullscreenDisplayMode(SDL_GetDisplayForWindow(m_window),
                                              size.width(), size.height(),
                                              0.0f, false, &mode))
      {
        if (!SDL_SetWindowFullscreenMode(m_window, &mode))
        {
          log_error("failed to set display mode: {}", SDL_GetError());
        }
      }
      else
      {
        log_error("no fullscreen mode close to {}x{}: {}",
                  size.width(), size.height(), SDL_GetError());
      }
      SDL_SetWindowFullscreen(m_window, true);
    }
    return true;
  }

  // Drop any GL attributes left by a failed OpenGLFramebuffer attempt so the
  // KMSDRM backend does not keep treating this as an EGL/GLES window.
  SDL_GL_ResetAttributes();
  SDL_ClearError();

  {
    char const* driver = SDL_GetCurrentVideoDriver();
    int num_displays = 0;
    SDL_free(SDL_GetDisplays(&num_displays));
    print_err("SDLFramebuffer: driver={} displays={} size={}x{} fullscreen={}",
              driver ? driver : "(none)", num_displays,
              size.width(), size.height(), fullscreen ? "yes" : "no");
    SDL_DisplayMode const* desk = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
    if (desk)
    {
      print_err("SDLFramebuffer: desktop {}x{} @{}Hz format={}",
                desk->w, desk->h, desk->refresh_rate,
                SDL_GetPixelFormatName(desk->format));
    }
  }

  int req_w = size.width();
  int req_h = size.height();
  if (fullscreen)
  {
    SDL_DisplayMode const* mode = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
    if (mode && mode->w > 0 && mode->h > 0)
    {
      req_w = mode->w;
      req_h = mode->h;
    }
  }

  // SDL3 creates SDL_WINDOW_FULLSCREEN windows as borderless desktop-size
  // fullscreen (what SDL2 called FULLSCREEN_DESKTOP).
  struct Attempt { char const* name; SDL_WindowFlags flags; };
  Attempt attempts[3];
  int n = 0;
  if (fullscreen)
  {
    attempts[n++] = Attempt{"fullscreen", SDL_WINDOW_FULLSCREEN};
  }
  attempts[n++] = Attempt{"windowed",
                          resizable && !fullscreen ? SDL_WINDOW_RESIZABLE : 0};
  // Last resort: zero flags
  attempts[n++] = Attempt{"minimal", 0};

  std::string last_error;
  for (int i = 0; i < n; ++i)
  {
    SDL_ClearError();
    print_err("SDLFramebuffer: SDL_CreateWindow try[{}] {}x{} flags=0x{:x}",
              attempts[i].name, req_w, req_h,
              static_cast<unsigned long long>(attempts[i].flags));
    m_window = SDL_CreateWindow("Pingus " PROJECT_VERSION,
                                req_w, req_h,
                                attempts[i].flags);
    if (m_window)
    {
      print_err("SDLFramebuffer: window ok via {}", attempts[i].name);
      break;
    }
    last_error = SDL_GetError();
    if (last_error.empty())
      last_error = "(SDL_GetError empty — possible DRM master / permission issue)";
    print_err("SDLFramebuffer: SDL_CreateWindow FAILED ({}): {}",
              attempts[i].name, last_error);
  }

  if (!m_window)
  {
#ifdef ANDROID
    if (last_error.find("Android only supports one window") != std::string::npos)
    {
      m_window = SDL_GetWindowFromID(1);
      if (m_window)
        print_err("SDLFramebuffer: reusing existing Android window id={}",
                  SDL_GetWindowID(m_window));
    }
#endif
    if (!m_window)
    {
      print_err("SDLFramebuffer: all CreateWindow attempts failed: {}", last_error);
#ifndef ANDROID
      print_err("  Hint: on ArkOS/R36S leave EmulationStation fully, run from a TTY, "
                "and ensure /dev/dri/card0 is free (fuser -v /dev/dri/card0). "
                "Prefer matching mali GBM libs via LD_LIBRARY_PATH=/usr/local/lib/aarch64-linux-gnu");
#endif
      return false;
    }
  }

  {
    SDL_Surface* icon = IMG_Load(Pathname("images/icons/pingus.png", Pathname::DATA_PATH).get_sys_path().c_str());
    if (icon)
    {
      SDL_SetWindowIcon(m_window, icon);
      SDL_DestroySurface(icon);
    }
  }

#ifndef __ANDROID__
  // SDL2 delivered text input events by default on desktop, SDL3 needs it
  // switched on per window. The editor's input boxes rely on them. Android
  // is left out, as this would pop up the on-screen keyboard.
  if (!SDL_StartTextInput(m_window))
    log_warn("SDL_StartTextInput failed: {}", SDL_GetError());
#endif

  // Prefer software on KMSDRM handhelds — ACCELERATED often goes through EGL/GLES
  // and can fail with the same "EGL window surface" error as the GL path.
  SDL_ClearError();
  char const* drv = SDL_GetCurrentVideoDriver();
  bool prefer_sw = (drv && (std::string(drv) == "KMSDRM" || std::string(drv) == "kmsdrm"));
  if (prefer_sw)
  {
    m_renderer = SDL_CreateRenderer(m_window, SDL_SOFTWARE_RENDERER);
    if (!m_renderer)
    {
      print_err("SDLFramebuffer: software renderer failed ({}), trying the default",
                SDL_GetError());
      m_renderer = SDL_CreateRenderer(m_window, nullptr);
    }
  }
  else
  {
    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer)
    {
      log_warn("default renderer failed ({}), trying software", SDL_GetError());
      m_renderer = SDL_CreateRenderer(m_window, SDL_SOFTWARE_RENDERER);
    }
  }
  if (!m_renderer)
  {
    print_err("SDLFramebuffer: SDL_CreateRenderer failed: {}", SDL_GetError());
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    return false;
  }

  {
    char const* name = SDL_GetRendererName(m_renderer);
    print_err("SDLFramebuffer: renderer='{}'", name ? name : "(unknown)");
  }
  return true;
}

bool
SDLFramebuffer::is_fullscreen() const
{
  return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0;
}

bool
SDLFramebuffer::is_resizable() const
{
  return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE) != 0;
}

bool
SDLFramebuffer::has_grab() const
{
  return SDL_GetWindowMouseGrab(m_window);
}

void
SDLFramebuffer::push_cliprect(geom::irect const& rect)
{
  SDL_Rect sdl_rect;
  sdl_rect.x = rect.left();
  sdl_rect.y = rect.top();
  sdl_rect.w = rect.width();
  sdl_rect.h = rect.height();

  if (!cliprect_stack.empty())
  {
    sdl_rect = Intersection(&cliprect_stack.back(), &sdl_rect);
  }

  cliprect_stack.push_back(sdl_rect);
  SDL_SetRenderClipRect(m_renderer, &cliprect_stack.back());
}

void
SDLFramebuffer::pop_cliprect()
{
  cliprect_stack.pop_back();
  if (cliprect_stack.empty())
    SDL_SetRenderClipRect(m_renderer, nullptr);
  else
    SDL_SetRenderClipRect(m_renderer, &cliprect_stack.back());
}

} // namespace pingus

/* EOF */
