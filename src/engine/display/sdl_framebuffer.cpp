//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
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
  screen(0),
  cliprect_stack()
{
}

SDLFramebuffer::~SDLFramebuffer()
{
}

FramebufferSurface
SDLFramebuffer::create_surface(const Surface& surface)
{
  return FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
}

void
SDLFramebuffer::draw_surface(const FramebufferSurface& surface, const Vector2i& pos)
{
  SDLFramebufferSurfaceImpl* impl = dynamic_cast<SDLFramebufferSurfaceImpl*>(surface.get_impl());
  SDL_Surface* src = impl->get_surface();

  SDL_Rect dstrect;
  dstrect.x = static_cast<Sint16>(pos.x);
  dstrect.y = static_cast<Sint16>(pos.y);
  dstrect.w = 0;
  dstrect.h = 0;

  SDL_BlitSurface(src, NULL, screen, &dstrect);
}

void
SDLFramebuffer::draw_surface(const FramebufferSurface& surface, const Rect& srcrect, const Vector2i& pos)
{
  SDLFramebufferSurfaceImpl* impl = dynamic_cast<SDLFramebufferSurfaceImpl*>(surface.get_impl());
  SDL_Surface* src = impl->get_surface();

  SDL_Rect dstrect;
  dstrect.x = static_cast<Sint16>(pos.x);
  dstrect.y = static_cast<Sint16>(pos.y);
  dstrect.w = 0;
  dstrect.h = 0;

  SDL_Rect sdlsrcrect;
  sdlsrcrect.x = static_cast<Sint16>(srcrect.left);
  sdlsrcrect.y = static_cast<Sint16>(srcrect.top);
  sdlsrcrect.w = static_cast<Uint16>(srcrect.get_width());
  sdlsrcrect.h = static_cast<Uint16>(srcrect.get_height());

  SDL_BlitSurface(src, &sdlsrcrect, screen, &dstrect);
}

void
SDLFramebuffer::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
  int x, y, xlen, ylen, incr;
  int sx = pos1.x;
  int sy = pos1.y;
  int dx = pos2.x;
  int dy = pos2.y;
  draw_pixel_func draw_pixel;
  int clipx1, clipx2, clipy1, clipy2;
  SDL_Rect rect;

  SDL_GetClipRect(screen, &rect);
  clipx1 = rect.x;
  clipx2 = rect.x + rect.w - 1;
  clipy1 = rect.y;
  clipy2 = rect.y + rect.h - 1;

  // vertical line
  if (sx == dx) {
    if (sx < clipx1 || sx > clipx2 || (sy < clipy1 && dy < clipy1) || (sy > clipy2 && dy > clipy2)) {
      return;
    }
    clip(sy, clipy1, clipy2);
    clip(dy, clipy1, clipy2);
    if (sy < dy) {
      draw_vline(screen, sx, sy, dy - sy + 1, color);
    } else {
      draw_vline(screen, dx, dy, sy - dy + 1, color);
    }
    return;
  }

  // horizontal
  if (sy == dy) {
    if (sy < clipy1 || sy > clipy2 || (sx < clipx1 && dx < clipx1) || (sx > clipx2 && dx > clipx2)) {
      return;
    }
    clip(sx, clipx1, clipx2);
    clip(dx, clipx1, clipx2);
    if (sx < dx) {
      draw_hline(screen, sx, sy, dx - sx + 1, color);
    } else {
      draw_hline(screen, dx, dy, sx - dx + 1, color);
    }
    return;
  }

  draw_pixel = get_draw_pixel(screen);
  if (!draw_pixel) {
    return;
  }

  // exchange coordinates
  if (sy > dy) {
    int t = dx;
    dx = sx;
    sx = t;
    t = dy;
    dy = sy;
    sy = t;
  }
  ylen = dy - sy;

  if (sx > dx) {
    xlen = sx - dx;
    incr = -1;
  } else {
    xlen = dx - sx;
    incr = 1;
  }

  y = sy;
  x = sx;

  if (xlen > ylen) {
    if (sx > dx) {
      int t = sx;
      sx = dx;
      dx = t;
      y = dy;
    }

    int p = (ylen << 1) - xlen;

    SDL_LockSurface(screen);
    for (x = sx; x < dx; ++x) {
      if (x >= clipx1 && x <= clipx2 && y >= clipy1 && y <= clipy2) {
        draw_pixel(screen, x, y, color);
      }
      if (p >= 0) {
        y += incr;
        p += (ylen - xlen) << 1;
      } else {
        p += (ylen << 1);
      }
    }
    SDL_UnlockSurface(screen);
    return;
  }

  if (ylen > xlen) {
    int p = (xlen << 1) - ylen;

    SDL_LockSurface(screen);
    for (y = sy; y < dy; ++y) {
      if (x >= clipx1 && x <= clipx2 && y >= clipy1 && y <= clipy2) {
        draw_pixel(screen, x, y, color);
      }
      if (p >= 0) {
        x += incr;
        p += (xlen - ylen) << 1;
      } else {
        p += (xlen << 1);
      }
    }
    SDL_UnlockSurface(screen);
    return;
  }

  // Draw a diagonal line
  if (ylen == xlen) {
    SDL_LockSurface(screen);
    while (y != dy) {
      if (x >= clipx1 && x <= clipx2 && y >= clipy1 && y <= clipy2) {
        draw_pixel(screen, x, y, color);
      }
      x += incr;
      ++y;
    }
    SDL_UnlockSurface(screen);
  }
}

void
SDLFramebuffer::draw_rect(const Rect& rect_, const Color& color)
{
  Rect rect = rect_;
  rect.normalize();

  draw_line(Vector2i(rect.left,    rect.top),      Vector2i(rect.right-1, rect.top),      color);
  draw_line(Vector2i(rect.left,    rect.bottom-1), Vector2i(rect.right-1, rect.bottom-1), color);
  draw_line(Vector2i(rect.left,    rect.top),      Vector2i(rect.left,    rect.bottom-1), color);
  draw_line(Vector2i(rect.right-1, rect.top),      Vector2i(rect.right-1, rect.bottom-1), color);
}

void
SDLFramebuffer::fill_rect(const Rect& rect_, const Color& color)
{
  Rect rect = rect_;
  rect.normalize();

  if (color.a == 255)
  {
    SDL_Rect srcrect;

    srcrect.x = static_cast<Sint16>(rect.left);
    srcrect.y = static_cast<Sint16>(rect.top);
    srcrect.w = static_cast<Uint16>(rect.get_width());
    srcrect.h = static_cast<Uint16>(rect.get_height());

    SDL_FillRect(screen, &srcrect, SDL_MapRGB(screen->format, color.r, color.g, color.b));
  }
  else if (color.a != 0)
  {
    int top, bottom, left, right;
    int clipx1, clipx2, clipy1, clipy2;
    SDL_Rect cliprect;

    SDL_GetClipRect(screen, &cliprect);
    clipx1 = cliprect.x;
    clipx2 = cliprect.x + cliprect.w - 1;
    clipy1 = cliprect.y;
    clipy2 = cliprect.y + cliprect.h - 1;

    if (rect.right < clipx1 || rect.left > clipx2 || rect.bottom < clipy1 || rect.top > clipy2)
      return;

    top    = rect.top    < clipy1 ? clipy1 : rect.top;
    bottom = rect.bottom > clipy2 ? clipy2 : rect.bottom-1;
    left   = rect.left   < clipx1 ? clipx1 : rect.left;
    right  = rect.right  > clipx2 ? clipx2 : rect.right-1;

    draw_pixel_func draw_pixel = get_draw_pixel(screen);
    if (!draw_pixel)
      return;

    SDL_LockSurface(screen);
    for (int y = top; y <= bottom; ++y) {
      for (int x = left; x <= right; ++x) {
        draw_pixel(screen, x, y, color);
      }
    }
    SDL_UnlockSurface(screen);
  }
}

void
SDLFramebuffer::flip()
{
  SDL_Flip(screen);
}

void
SDLFramebuffer::update_rects(const std::vector<Rect>& rects)
{
  std::vector<SDL_Rect> sdl_rects;

  for(std::vector<Rect>::const_iterator i = rects.begin(); i != rects.end(); ++i)
  {
    SDL_Rect sdl_rect;
    sdl_rect.x = static_cast<Sint16>(i->left);
    sdl_rect.y = static_cast<Sint16>(i->top);
    sdl_rect.w = static_cast<Uint16>(i->get_width());
    sdl_rect.h = static_cast<Uint16>(i->get_height());
    sdl_rects.push_back(sdl_rect);
  }

  SDL_UpdateRects(screen, static_cast<int>(sdl_rects.size()), const_cast<SDL_Rect*>(&*sdl_rects.begin()));
}

Size
SDLFramebuffer::get_size() const
{
  return Size(screen->w, screen->h);
}

void
SDLFramebuffer::set_video_mode(const Size& size, bool fullscreen, bool resizable)
{
  Uint32 flags = 0;

  if (fullscreen)
  {
    flags |= SDL_FULLSCREEN;
  }
  else if (resizable)
  {
    flags |= SDL_RESIZABLE;
  }

  screen = SDL_SetVideoMode(size.width, size.height, 0, flags);

  if (screen == NULL)
  {
    log_error("Unable to set video mode: %1%", SDL_GetError());
    exit(1);
  }
}

bool
SDLFramebuffer::is_fullscreen() const
{
  return screen->flags & SDL_FULLSCREEN;
}

bool
SDLFramebuffer::is_resizable() const
{
  return screen->flags & SDL_RESIZABLE;
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
  SDL_SetClipRect(screen, &cliprect_stack.back());
}

void
SDLFramebuffer::pop_cliprect()
{
  cliprect_stack.pop_back();
  if (cliprect_stack.empty())
    SDL_SetClipRect(screen, NULL);
  else
    SDL_SetClipRect(screen, &cliprect_stack.back());
}

/* EOF */
