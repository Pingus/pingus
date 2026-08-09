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

#include <SDL_image.h>
#include <sstream>
#include <iostream>
#include <string>

#include <logmich/log.hpp>

#include "engine/display/sdl_framebuffer_surface_impl.hpp"

namespace pingus {

namespace {

SDL_Rect Intersection(SDL_Rect* r1, SDL_Rect* r2)
{
  SDL_Rect rect;
  rect.x = std::max(r1->x, r2->x);
  rect.y = std::max(r1->y, r2->y);
  int endx = std::min(r1->x + r1->w, r2->x + r2->w);
  rect.w = static_cast<Uint16>(std::max(endx - rect.x, 0));
  int endy = std::min(r1->y + r1->h, r2->y + r2->h);
  rect.h = static_cast<Uint16>(std::max(endy - rect.y, 0));
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
  if (SDL_GetRendererOutputSize(m_renderer, &w, &h) != 0)
  {
    log_error("SDL_GetRenderOutputgeom::isize failed: {}", SDL_GetError());
    return Surface();
  }
  else
  {
    Surface screenshot(w, h);
    int ret = SDL_RenderReadPixels(m_renderer, nullptr,
                                   SDL_PIXELFORMAT_ABGR8888,
                                   screenshot.get_data(),
                                   screenshot.get_pitch());
    if (ret != 0)
    {
      log_error("{}", SDL_GetError());
    }

    return screenshot;
  }
}

void
SDLFramebuffer::draw_surface(FramebufferSurface const& surface, geom::ipoint const& pos)
{
  SDLFramebufferSurfaceImpl* impl = dynamic_cast<SDLFramebufferSurfaceImpl*>(surface.get_impl());
  SDL_Texture* texture = impl->get_texture();

  SDL_Rect dstrect;
  dstrect.x = static_cast<Sint16>(pos.x());
  dstrect.y = static_cast<Sint16>(pos.y());
  dstrect.w = surface.get_width();
  dstrect.h = surface.get_height();

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(m_renderer, texture, nullptr, &dstrect);
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_surface(FramebufferSurface const& surface, geom::irect const& srcrect, geom::ipoint const& pos)
{
  SDLFramebufferSurfaceImpl* impl = dynamic_cast<SDLFramebufferSurfaceImpl*>(surface.get_impl());
  SDL_Texture* texture = impl->get_texture();

  SDL_Rect dstrect;
  dstrect.x = static_cast<Sint16>(pos.x());
  dstrect.y = static_cast<Sint16>(pos.y());
  dstrect.w = srcrect.width();
  dstrect.h = srcrect.height();

  SDL_Rect sdlsrcrect;
  sdlsrcrect.x = static_cast<Sint16>(srcrect.left());
  sdlsrcrect.y = static_cast<Sint16>(srcrect.top());
  sdlsrcrect.w = static_cast<Uint16>(srcrect.width());
  sdlsrcrect.h = static_cast<Uint16>(srcrect.height());

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(m_renderer, texture, &sdlsrcrect, &dstrect);
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_line(geom::ipoint const& pos1, geom::ipoint const& pos2, Color const& color)
{
  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(m_renderer, pos1.x(), pos1.y(), pos2.x(), pos2.y());
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_rect(geom::irect const& rect_, Color const& color)
{
  geom::irect rect = geom::normalize(rect_);

  SDL_Rect sdl_rect;
  sdl_rect.x = rect.left();
  sdl_rect.y = rect.top();
  sdl_rect.w = rect.width();
  sdl_rect.h = rect.height();

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawRect(m_renderer, &sdl_rect);
  // FIXME: pop render state
}

void
SDLFramebuffer::fill_rect(geom::irect const& rect_, Color const& color)
{
  geom::irect rect = geom::normalize(rect_);

  SDL_Rect sdl_rect;
  sdl_rect.x = rect.left();
  sdl_rect.y = rect.top();
  sdl_rect.w = rect.width();
  sdl_rect.h = rect.height();

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
SDLFramebuffer::update_rects(std::vector<geom::irect> const& rects)
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
      SDL_SetWindowFullscreen(m_window, 0);
    }
    else
    {
      SDL_DisplayMode mode;
      mode.w = size.width();
      mode.h = size.height();
      mode.refresh_rate = 0;
      mode.driverdata = nullptr;

      if (SDL_SetWindowDisplayMode(m_window, &mode) != 0)
      {
        log_error("failed to set display mode: {}", SDL_GetError());
      }
      SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    }
    return true;
  }

  // Drop any GL attributes left by a failed OpenGLFramebuffer attempt so the
  // KMSDRM backend does not keep treating this as an EGL/GLES window.
  SDL_GL_ResetAttributes();
  SDL_ClearError();

  {
    char const* driver = SDL_GetCurrentVideoDriver();
    std::cerr << "SDLFramebuffer: driver=" << (driver ? driver : "(none)")
              << " displays=" << SDL_GetNumVideoDisplays()
              << " size=" << size.width() << "x" << size.height()
              << " fullscreen=" << (fullscreen ? "yes" : "no") << std::endl;
    SDL_DisplayMode desk;
    if (SDL_GetDesktopDisplayMode(0, &desk) == 0)
    {
      std::cerr << "SDLFramebuffer: desktop " << desk.w << "x" << desk.h
                << " @" << desk.refresh_rate << "Hz format=" << desk.format
                << std::endl;
    }
  }

  int req_w = size.width();
  int req_h = size.height();
  if (fullscreen)
  {
    SDL_DisplayMode mode;
    if (SDL_GetDesktopDisplayMode(0, &mode) == 0 && mode.w > 0 && mode.h > 0)
    {
      req_w = mode.w;
      req_h = mode.h;
    }
  }

  struct Attempt { char const* name; Uint32 flags; };
  Attempt attempts[4];
  int n = 0;
  if (fullscreen)
  {
    attempts[n++] = Attempt{"fullscreen-desktop", SDL_WINDOW_FULLSCREEN_DESKTOP};
    attempts[n++] = Attempt{"fullscreen-exclusive", SDL_WINDOW_FULLSCREEN};
  }
  attempts[n++] = Attempt{"windowed",
                          static_cast<Uint32>(resizable && !fullscreen ? SDL_WINDOW_RESIZABLE : 0)};
  // Last resort: zero flags
  attempts[n++] = Attempt{"minimal", 0};

  std::string last_error;
  for (int i = 0; i < n; ++i)
  {
    SDL_ClearError();
    std::cerr << "SDLFramebuffer: SDL_CreateWindow try[" << attempts[i].name
              << "] " << req_w << "x" << req_h
              << " flags=0x" << std::hex << attempts[i].flags << std::dec
              << std::endl;
    m_window = SDL_CreateWindow("Pingus " PROJECT_VERSION,
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                req_w, req_h,
                                attempts[i].flags);
    if (m_window)
    {
      std::cerr << "SDLFramebuffer: window ok via " << attempts[i].name << std::endl;
      break;
    }
    last_error = SDL_GetError();
    if (last_error.empty())
      last_error = "(SDL_GetError empty — possible DRM master / permission issue)";
    std::cerr << "SDLFramebuffer: SDL_CreateWindow FAILED (" << attempts[i].name
              << "): " << last_error << std::endl;
  }

  if (!m_window)
  {
    std::cerr << "SDLFramebuffer: all CreateWindow attempts failed: " << last_error
              << "\n  Hint: on ArkOS/R36S leave EmulationStation fully, run from a TTY,"
                 " and ensure /dev/dri/card0 is free (fuser -v /dev/dri/card0)."
              << " Prefer matching mali GBM libs via LD_LIBRARY_PATH=/usr/local/lib/aarch64-linux-gnu"
              << std::endl;
    return false;
  }

  {
    SDL_Surface* icon = IMG_Load(Pathname("images/icons/pingus.png", Pathname::DATA_PATH).get_sys_path().c_str());
    if (icon)
    {
      SDL_SetWindowIcon(m_window, icon);
      SDL_FreeSurface(icon);
    }
  }

  // Prefer software on KMSDRM handhelds — ACCELERATED often goes through EGL/GLES
  // and can fail with the same "EGL window surface" error as the GL path.
  SDL_ClearError();
  char const* drv = SDL_GetCurrentVideoDriver();
  bool prefer_sw = (drv && (std::string(drv) == "KMSDRM" || std::string(drv) == "kmsdrm"));
  if (prefer_sw)
  {
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);
    if (!m_renderer)
    {
      std::cerr << "SDLFramebuffer: SOFTWARE renderer failed (" << SDL_GetError()
                << "), trying ACCELERATED" << std::endl;
      m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    }
  }
  else
  {
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
      log_warn("SDL_RENDERER_ACCELERATED failed ({}), trying software", SDL_GetError());
      m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);
    }
  }
  if (!m_renderer)
  {
    std::cerr << "SDLFramebuffer: SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    return false;
  }

  {
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(m_renderer, &info) == 0)
      std::cerr << "SDLFramebuffer: renderer='" << info.name << "'" << std::endl;
  }
  return true;
}

bool
SDLFramebuffer::is_fullscreen() const
{
  return SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN;
}

bool
SDLFramebuffer::is_resizable() const
{
  return SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE;
}

bool
SDLFramebuffer::has_grab() const
{
  return SDL_GetWindowGrab(m_window);
}

void
SDLFramebuffer::push_cliprect(geom::irect const& rect)
{
  SDL_Rect sdl_rect;
  sdl_rect.x = static_cast<Sint16>(rect.left());
  sdl_rect.y = static_cast<Sint16>(rect.top());
  sdl_rect.w = static_cast<Uint16>(rect.width());
  sdl_rect.h = static_cast<Uint16>(rect.height());

  if (!cliprect_stack.empty())
  {
    sdl_rect = Intersection(&cliprect_stack.back(), &sdl_rect);
  }

  cliprect_stack.push_back(sdl_rect);
  SDL_RenderSetClipRect(m_renderer, &cliprect_stack.back());
}

void
SDLFramebuffer::pop_cliprect()
{
  cliprect_stack.pop_back();
  if (cliprect_stack.empty())
    SDL_RenderSetClipRect(m_renderer, nullptr);
  else
    SDL_RenderSetClipRect(m_renderer, &cliprect_stack.back());
}

} // namespace pingus

/* EOF */
