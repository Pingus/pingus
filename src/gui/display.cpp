//  $Id: display.cxx,v 1.5 2003/10/18 23:17:28 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include "../globals.hpp"
#include "../math/vector2i.hpp"
#include "../math/rect.hpp"
#include "../math/color.hpp"
#include "display.hpp"

std::list<DisplayHook*> Display::display_hooks;
SDL_Surface* Display::screen;

DisplayHook::DisplayHook() : is_visible(false)
{
}

void
DisplayHook::toggle_display()
{
  if (is_visible)
    Display::remove_flip_screen_hook(this);
  else
    Display::add_flip_screen_hook(this);

  is_visible = !is_visible;
}

void
Display::flip_display(bool sync)
{
  for(std::list<DisplayHook*>::iterator i = display_hooks.begin();
      i != display_hooks.end();
      i++)
    {
      (*i)->on_event();
    }

  SDL_Flip(screen);
}

void
Display::add_flip_screen_hook(DisplayHook* hook)
{
  if (std::find(display_hooks.begin(), display_hooks.end(), hook) == display_hooks.end())
    display_hooks.push_back(hook);
  else
    std::cout << "Display: Trying to insert a display hook multiple times..." << std::endl;
}

void
Display::remove_flip_screen_hook(DisplayHook* hook)
{
  display_hooks.remove(hook);
}

void
Display::set_video_mode(int width, int height)
{
  screen = SDL_SetVideoMode(width, height, 0, SDL_DOUBLEBUF); // | SDL_FULLSCREEN
  if (screen == NULL) {
    printf("Unable to set video mode: %s\n", SDL_GetError());
    exit(1);
  }
}

int
Display::get_width()
{
  return screen->w;
}

int
Display::get_height()
{
  return screen->h;
}

void
Display::clear()
{
  SDL_FillRect(screen, NULL, SDL_MapRGB(Display::get_screen()->format, 0, 0, 0));
}

static void draw_pixel16(int x, int y, const Color& c)
{
  Uint32 color = SDL_MapRGBA(Display::get_screen()->format, c.r, c.g, c.b, c.a);

  if (c.a < 255) {
    Uint16 *p;
    unsigned long dp;
    unsigned char alpha;

    // Loses precision for speed
    alpha = (255 - c.a) >> 3;

    p = &((Uint16 *)Display::get_screen()->pixels)[x + y * Display::get_screen()->w];
    color = (((color << 16) | color) & 0x07E0F81F);
    dp = *p;
    dp = ((dp << 16) | dp) & 0x07E0F81F;
    dp = ((((dp - color) * alpha) >> 5) + color) & 0x07E0F81F;
    *p = (Uint16)((dp >> 16) | dp);
  } else {
    ((Uint16 *)Display::get_screen()->pixels)[x + y * Display::get_screen()->w] = color;
  }
}
static void draw_pixel32(int x, int y, const Color& c)
{
  Uint32 color = SDL_MapRGBA(Display::get_screen()->format, c.r, c.g, c.b, c.a);

  if (c.a < 255) {
    Uint32 *p;
    unsigned long sp2;
    unsigned long dp1;
    unsigned long dp2;
    unsigned char alpha;

    alpha = 255 - c.a;

    p = &((Uint32*)Display::get_screen()->pixels)[x + y * Display::get_screen()->w];

    sp2 = (color & 0xFF00FF00) >> 8;
    color &= 0x00FF00FF;

    dp1 = *p;
    dp2 = (dp1 & 0xFF00FF00) >> 8;
    dp1 &= 0x00FF00FF;

    dp1 = ((((dp1 - color) * alpha) >> 8) + color) & 0x00FF00FF;
    dp2 = ((((dp2 - sp2) * alpha) >> 8) + sp2) & 0x00FF00FF;
    *p = (dp1 | (dp2 << 8));
  } else {
    ((Uint32 *)Display::get_screen()->pixels)[x + y * Display::get_screen()->w] = color;
  }
}

typedef void (*draw_pixel_func)(int, int, const Color&);
static draw_pixel_func get_draw_pixel()
{
  switch (Display::get_screen()->format->BitsPerPixel)
  {
  case 16:
    return draw_pixel16;
  case 32:
    return draw_pixel32;
  }
  return NULL;
}

static void draw_vline(int x, int y, int length, const Color& color)
{
  draw_pixel_func draw_pixel = get_draw_pixel();
  if (!draw_pixel)
    return;

  for (int i = 0; i < length; ++i) {
    draw_pixel(x, y + i, color);
  }
}

static void draw_hline(int x, int y, int length, const Color& color)
{
  draw_pixel_func draw_pixel = get_draw_pixel();
  if (!draw_pixel)
    return;

  for (int i = 0; i < length; ++i) {
    draw_pixel(x + i, y, color);
  }
}

void
Display::draw_line(int x1, int y1, int x2, int y2, const Color& color)
{
  Display::draw_line(Vector2i(x1, y1), Vector2i(x2, y2), color);
}

void
Display::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
  int x, y, xlen, ylen, incr;
  int sx = pos1.x;
  int sy = pos1.y;
  int dx = pos2.x;
  int dy = pos2.y;
  void (*draw_pixel)(int x, int y, const Color& color);

  // vertical line
  if (sx == dx) {
    if (sy < dy) {
      draw_vline(sx, sy, dy - sy + 1, color);
    } else {
      draw_vline(dx, dy, sy - dy + 1, color);
    }
    return;
  }

  // horizontal
  if (sy == dy) {
    if (sx < dx) {
      draw_hline(sx, sy, dx - sx + 1, color);
    } else {
      draw_hline(dx, dy, sx - dx + 1, color);
    }
    return;
  }

  draw_pixel = get_draw_pixel();
  if (!draw_pixel)
    return;

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
      draw_pixel(x, y, color);
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
      draw_pixel(x, y, color);
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
      draw_pixel(x, y, color);
      x += incr;
      ++y;
    }
    SDL_UnlockSurface(screen);
  }
}

void
Display::draw_rect(int x1, int y1, int x2, int y2, const Color& color)
{
  Display::draw_line(Vector2i(x1, y1), Vector2i(x2, y1), color);
  Display::draw_line(Vector2i(x1, y2), Vector2i(x2, y2), color);
  Display::draw_line(Vector2i(x1, y1), Vector2i(x1, y2), color);
  Display::draw_line(Vector2i(x2, y1), Vector2i(x2, y2), color);
}

void
Display::draw_rect(const Rect& rect, const Color& color)
{
  Display::draw_rect(rect.left, rect.top, rect.right, rect.bottom, color);
}

void
Display::fill_rect(const Rect& rect, const Color& color)
{
  SDL_Rect srcrect;

  srcrect.x = rect.left;
  srcrect.y = rect.top;
  srcrect.w = rect.get_width();
  srcrect.h = rect.get_height();

  SDL_FillRect(screen, &srcrect, SDL_MapRGB(screen->format, color.r, color.g, color.b));
}

void
Display::push_cliprect(const Rect&)
{
}

void
Display::pop_cliprect()
{
}

/* EOF */