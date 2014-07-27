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

#include "engine/display/sdl_framebuffer_surface_impl.hpp"
#include "util/log.hpp"

namespace {

typedef void (*draw_pixel_func)(SDL_Surface* screen, int, int, const Color&);

inline void draw_pixel16(SDL_Surface* screen, int x, int y, const Color& c)
{
  Uint32 color = SDL_MapRGBA(screen->format, c.r, c.g, c.b, c.a);

  if (c.a < 255) {
    Uint16 *p;
    unsigned long dp;
    uint8_t alpha;

    // Loses precision for speed
    alpha = static_cast<uint8_t>((255 - c.a) >> 3);

    p = &(static_cast<Uint16 *>(screen->pixels))[x + y * screen->w];
    color = (((color << 16) | color) & 0x07E0F81F);
    dp = *p;
    dp = ((dp << 16) | dp) & 0x07E0F81F;
    dp = ((((dp - color) * alpha) >> 5) + color) & 0x07E0F81F;
    *p = static_cast<Uint16>((dp >> 16) | dp);
  } else {
    static_cast<Uint16*>(screen->pixels)[x + y * screen->w] = static_cast<Uint16>(color);
  }
}

inline void draw_pixel32(SDL_Surface* screen, int x, int y, const Color& c)
{
  Uint32 color = SDL_MapRGBA(screen->format, c.r, c.g, c.b, c.a);

  if (c.a < 255) {
    Uint32 *p;
    unsigned long sp2;
    unsigned long dp1;
    unsigned long dp2;
    unsigned char alpha;

    alpha = static_cast<unsigned char>(255 - c.a);

    p = &(static_cast<Uint32*>(screen->pixels))[x + y * screen->w];

    sp2 = (color & 0xFF00FF00) >> 8;
    color &= 0x00FF00FF;

    dp1 = *p;
    dp2 = (dp1 & 0xFF00FF00) >> 8;
    dp1 &= 0x00FF00FF;

    dp1 = ((((dp1 - color) * alpha) >> 8) + color) & 0x00FF00FF;
    dp2 = ((((dp2 - sp2) * alpha) >> 8) + sp2) & 0x00FF00FF;
    *p = static_cast<Uint32>(dp1 | (dp2 << 8));
  } else {
    (static_cast<Uint32*>(screen->pixels))[x + y * screen->w] = color;
  }
}

draw_pixel_func get_draw_pixel(SDL_Surface* screen)
{
  switch (screen->format->BitsPerPixel)
  {
    case 16:
      return draw_pixel16;
    case 32:
      return draw_pixel32;
  }
  return NULL;
}

void draw_vline(SDL_Surface* screen, int x, int y, int length, const Color& color)
{
  draw_pixel_func draw_pixel = get_draw_pixel(screen);
  if (!draw_pixel)
    return;

  SDL_LockSurface(screen);
  for (int i = 0; i < length; ++i) {
    draw_pixel(screen, x, y + i, color);
  }
  SDL_UnlockSurface(screen);
}

void draw_hline(SDL_Surface* screen, int x, int y, int length, const Color& color)
{
  draw_pixel_func draw_pixel = get_draw_pixel(screen);
  if (!draw_pixel)
    return;

  SDL_LockSurface(screen);
  for (int i = 0; i < length; ++i) {
    draw_pixel(screen, x + i, y, color);
  }
  SDL_UnlockSurface(screen);
}

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

void clip(int& i, int min, int max)
{
  if (i < min)
    i = min;
  else if (i > max)
    i = max;
}

} // namespace

SDLFramebuffer::SDLFramebuffer() :
  m_window(nullptr),
  m_renderer(nullptr),
  m_screen(nullptr),
  m_texture(nullptr),
  cliprect_stack()
{
}

SDLFramebuffer::~SDLFramebuffer()
{
  SDL_DestroyTexture(m_texture);
  SDL_FreeSurface(m_screen);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

FramebufferSurface
SDLFramebuffer::create_surface(const Surface& surface)
{
  return FramebufferSurface(new SDLFramebufferSurfaceImpl(m_renderer, surface.get_surface()));
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
  m_screen = SDL_CreateRGBSurface(0, size.width, size.height, 32,
                                  0x00FF0000,
                                  0x0000FF00,
                                  0x000000FF,
                                  0xFF000000);
  m_texture = SDL_CreateTexture(m_renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                size.width, size.height);
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
