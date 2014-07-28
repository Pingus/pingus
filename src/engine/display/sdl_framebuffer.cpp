//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/display/sdl_framebuffer.hpp"

#include <SDL_image.h>
#include <sstream>

#include "engine/display/sdl_framebuffer_surface_impl.hpp"
#include "util/log.hpp"

namespace {

SDL_Rect Intersection(SDL_Rect* r1, SDL_Rect* r2)
{
  SDL_Rect rect;
  rect.x = Math::max(r1->x, r2->x);
  rect.y = Math::max(r1->y, r2->y);
  int endx = Math::min(r1->x + r1->w, r2->x + r2->w);
  rect.w = static_cast<Uint16>(Math::max(static_cast<int>(endx - rect.x), 0));
  int endy = Math::min(r1->y + r1->h, r2->y + r2->h);
  rect.h = static_cast<Uint16>(Math::max(static_cast<int>(endy - rect.y), 0));
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
SDLFramebuffer::create_surface(const Surface& surface)
{
  return FramebufferSurface(new SDLFramebufferSurfaceImpl(m_renderer, surface.get_surface()));
}

Surface
SDLFramebuffer::make_screenshot() const
{
  Size size;
  if (SDL_GetRendererOutputSize(m_renderer, &size.width, &size.height) != 0)
  {
    log_error("SDL_GetRenderOutputSize failed: %1%", SDL_GetError());
    return Surface();
  }
  else
  {
    Surface screenshot(size.width, size.height);
    int ret = SDL_RenderReadPixels(m_renderer, nullptr,
                                   SDL_PIXELFORMAT_ABGR8888,
                                   screenshot.get_data(),
                                   screenshot.get_pitch());
    if (ret != 0)
    {
      log_error("%1%", SDL_GetError());
    }

    return screenshot;
  }
}

void
SDLFramebuffer::draw_surface(const FramebufferSurface& surface, const Vector2i& pos)
{
  SDLFramebufferSurfaceImpl* impl = dynamic_cast<SDLFramebufferSurfaceImpl*>(surface.get_impl());
  SDL_Texture* texture = impl->get_texture();

  SDL_Rect dstrect;
  dstrect.x = static_cast<Sint16>(pos.x);
  dstrect.y = static_cast<Sint16>(pos.y);
  dstrect.w = surface.get_width();
  dstrect.h = surface.get_height();

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(m_renderer, texture, nullptr, &dstrect);
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_surface(const FramebufferSurface& surface, const Rect& srcrect, const Vector2i& pos)
{
  SDLFramebufferSurfaceImpl* impl = dynamic_cast<SDLFramebufferSurfaceImpl*>(surface.get_impl());
  SDL_Texture* texture = impl->get_texture();

  SDL_Rect dstrect;
  dstrect.x = static_cast<Sint16>(pos.x);
  dstrect.y = static_cast<Sint16>(pos.y);
  dstrect.w = srcrect.get_width();
  dstrect.h = srcrect.get_height();

  SDL_Rect sdlsrcrect;
  sdlsrcrect.x = static_cast<Sint16>(srcrect.left);
  sdlsrcrect.y = static_cast<Sint16>(srcrect.top);
  sdlsrcrect.w = static_cast<Uint16>(srcrect.get_width());
  sdlsrcrect.h = static_cast<Uint16>(srcrect.get_height());

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(m_renderer, texture, &sdlsrcrect, &dstrect);
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(m_renderer, pos1.x, pos1.y, pos2.x, pos2.y);
  // FIXME: pop render state
}

void
SDLFramebuffer::draw_rect(const Rect& rect_, const Color& color)
{
  Rect rect = rect_;
  rect.normalize();

  SDL_Rect sdl_rect;
  sdl_rect.x = rect.left;
  sdl_rect.y = rect.top;
  sdl_rect.w = rect.get_width();
  sdl_rect.h = rect.get_height();

  // FIXME: push render state
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawRect(m_renderer, &sdl_rect);
  // FIXME: pop render state
}

void
SDLFramebuffer::fill_rect(const Rect& rect_, const Color& color)
{
  Rect rect = rect_;
  rect.normalize();

  SDL_Rect sdl_rect;
  sdl_rect.x = rect.left;
  sdl_rect.y = rect.top;
  sdl_rect.w = rect.get_width();
  sdl_rect.h = rect.get_height();

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
SDLFramebuffer::update_rects(const std::vector<Rect>& rects)
{
  flip();
}

Size
SDLFramebuffer::get_size() const
{
  Size s;
  SDL_GetWindowSize(m_window, &s.width, &s.height);
  return s;
}

void
SDLFramebuffer::set_video_mode(const Size& size, bool fullscreen, bool resizable)
{
  if (m_window)
  {
    if (!fullscreen)
    {
      SDL_SetWindowSize(m_window, size.width, size.height);
      SDL_SetWindowFullscreen(m_window, 0);
    }
    else
    {
      SDL_DisplayMode mode;
      mode.w = size.width;
      mode.h = size.height;
      mode.refresh_rate = 0;
      mode.driverdata = nullptr;

      if (SDL_SetWindowDisplayMode(m_window, &mode) != 0)
      {
        log_error("failed to set display mode: %1%", SDL_GetError());
      }
      SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    }
  }
  else
  {
    Uint32 flags = 0;

    if (fullscreen)
    {
      flags |= SDL_WINDOW_FULLSCREEN;
    }
    else if (resizable)
    {
      flags |= SDL_WINDOW_RESIZABLE;
    }

    m_window = SDL_CreateWindow("Pingus " VERSION,
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                size.width, size.height,
                                flags);
    if(m_window == 0)
    {
      std::ostringstream msg;
      msg << "Couldn't set video mode (" << size.width << "x" << size.height << "): " << SDL_GetError();
      throw std::runtime_error(msg.str());
    }
    SDL_SetWindowIcon(m_window, IMG_Load(Pathname("images/icons/pingus.png", Pathname::DATA_PATH).get_sys_path().c_str()));

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
  }
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
SDLFramebuffer::push_cliprect(const Rect& rect)
{
  SDL_Rect sdl_rect;
  sdl_rect.x = static_cast<Sint16>(rect.left);
  sdl_rect.y = static_cast<Sint16>(rect.top);
  sdl_rect.w = static_cast<Uint16>(rect.get_width());
  sdl_rect.h = static_cast<Uint16>(rect.get_height());

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
    SDL_RenderSetClipRect(m_renderer, NULL);
  else
    SDL_RenderSetClipRect(m_renderer, &cliprect_stack.back());
}

/* EOF */
