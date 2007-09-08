//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <assert.h>
#include <iostream>
#include <algorithm>
#include "drawing_context.hpp"
#include "math.hpp"
#include "gui/display.hpp"
#include "../sprite.hpp"
#include "../font.hpp"
#include "../math/origin.hpp"


struct DrawingRequestsSorter
{
  bool operator()(DrawingRequest* a, DrawingRequest* b) {
    return a->get_z_pos() < b->get_z_pos();
  }
};

class FontDrawingRequest : public DrawingRequest
{
private:
  Font font;
  Origin origin;
  std::string text;
  float x;
  float y;

public:
  FontDrawingRequest(Font font_, Origin origin_, const Vector3f& pos, const std::string& text_, float z)
    : DrawingRequest(Vector3f(pos.x, pos.y, z)),
      font(font_), 
      origin(origin_),
      text(text_)
  {
  }

  virtual ~FontDrawingRequest() {}

  void render(SDL_Surface* target, const Rect& rect) {
    font.draw(origin, static_cast<int>(pos.x + rect.left), static_cast<int>(pos.y + rect.top), text, target);
  }
};

class SpriteDrawingRequest : public DrawingRequest
{
private:
  Sprite sprite;

public:
  SpriteDrawingRequest(const Sprite& sprite_, const Vector3f& pos_)
    : DrawingRequest(pos_),
      sprite(sprite_)
  {
  }

  virtual ~SpriteDrawingRequest() {}

  void render(SDL_Surface* target, const Rect& rect) {
    sprite.draw(pos.x + rect.left, pos.y + rect.top, target);
  }
};

class FillScreenDrawingRequest : public DrawingRequest
{
private:
  Color color;
public:
  FillScreenDrawingRequest(const Color& color_) 
    : DrawingRequest(Vector3f(0, 0, -1000.0f)), color(color_)
  {
  }
  virtual ~FillScreenDrawingRequest() {}

  void render(SDL_Surface* target, const Rect& rect) {
    SDL_Rect r;
    r.x = rect.left;
    r.y = rect.top;
    r.w = rect.get_width();
    r.h = rect.get_height();
    SDL_FillRect(target, &r, SDL_MapRGB(target->format, color.r, color.g, color.b));
  }
};

class LineDrawingRequest : public DrawingRequest
{
private:
  Vector2i pos1;
  Vector2i pos2;
  Color  color;

public:
  LineDrawingRequest(const Vector2i& pos1_, 
                     const Vector2i& pos2_, 
                     const Color&  color_,
                     float z)
    : DrawingRequest(Vector3f(0, 0, z)),
      pos1(pos1_),
      pos2(pos2_),
      color(color_)      
  {
  }

  void render(SDL_Surface* target, const Rect& rect)
  {
    Display::draw_line(pos1 + Vector2i(rect.left, rect.top),
                       pos2 + Vector2i(rect.left, rect.top), color);
  }
};

class RectDrawingRequest : public DrawingRequest
{
private:
  Rect  d_rect;
  Color color;
  bool  filled;
  
public:
  RectDrawingRequest(const Rect& rect_, const Color& color_, bool filled_, float z)
    : DrawingRequest(Vector3f(0, 0, z)),
      d_rect(rect_), color(color_), filled(filled_)
  {}
  
  void render(SDL_Surface* target, const Rect& rect)
  {
    if (filled)
      {
        Display::fill_rect(Rect(Vector2i(d_rect.left + rect.left, 
                                         d_rect.top  + rect.top),
                                d_rect.get_size()), 
                           color);
      }
    else
      {
        Display::draw_rect(Rect(Vector2i(d_rect.left + rect.left, 
                                         d_rect.top  + rect.top),
                                d_rect.get_size()), 
                           color);
      }
  }
};

class DrawingContextDrawingRequest : public DrawingRequest
{
private:
  DrawingContext& dc;
  
public:
  DrawingContextDrawingRequest(DrawingContext& dc_, float z)
    : DrawingRequest(Vector3f(0,0,z)),
      dc(dc_)
  {}
  
  virtual ~DrawingContextDrawingRequest()
  {
  }

  void render(SDL_Surface* target, const Rect& rect) {
    dc.render(target, rect);
  }
};

DrawingContext::DrawingContext(const Rect& rect_, bool clip)
  : rect(rect_),
    do_clipping(clip)
{
  translate_stack.push_back(Vector3f(0, 0));
}

DrawingContext::DrawingContext()
  : rect(0, 0, Display::get_width(), Display::get_height()),
    do_clipping(false)
{
  translate_stack.push_back(Vector3f(0, 0));
}

DrawingContext::~DrawingContext()
{
  if (drawingrequests.size() > 0) 
    clear();
}

void
DrawingContext::render(SDL_Surface* screen, const Rect& parent_rect)
{
  Rect this_rect(Math::max(rect.left   + parent_rect.left, parent_rect.left),
                 Math::max(rect.top    + parent_rect.top,  parent_rect.top),
                 Math::min(rect.right  + parent_rect.left, parent_rect.right),
                 Math::min(rect.bottom + parent_rect.top,  parent_rect.bottom));

  if (do_clipping) 
    Display::push_cliprect(this_rect);

  std::stable_sort(drawingrequests.begin(), drawingrequests.end(), DrawingRequestsSorter());
  
  if (0)
    {
      std::cout << "<<<<<<<<<<<<<<" << std::endl;
      for(DrawingRequests::iterator i = drawingrequests.begin(); i != drawingrequests.end(); ++i)
        std::cout << (*i)->get_z_pos() << std::endl;
      std::cout << ">>>>>>>>>>>>>>" << std::endl;
    }
  for(DrawingRequests::iterator i = drawingrequests.begin(); i != drawingrequests.end(); ++i)
    {
      //std::cout << this << ": " << (*i)->get_z_pos() << std::endl;
      (*i)->render(screen, this_rect); // FIXME: Should we clip size against parent rect?
    }

  if (do_clipping) 
    Display::pop_cliprect();
}

void
DrawingContext::clear()
{
  for(DrawingRequests::iterator i = drawingrequests.begin(); i != drawingrequests.end(); ++i)
    {
      delete *i;
    }
  drawingrequests.clear();
}

void
DrawingContext::draw(DrawingRequest* request)
{
  drawingrequests.push_back(request);
}

void
DrawingContext::draw(DrawingContext& dc, float z)
{
  draw(new DrawingContextDrawingRequest(dc, z));
}

void
DrawingContext::draw(const Sprite& sprite, const Vector3f& pos)
{
  draw(sprite, pos.x, pos.y, pos.z);
}

void
DrawingContext::draw(const Sprite&   sprite,  float x, float y, float z)
{ // FIXME: This should get flattend down to a simple texture draw
  // command for easier sorting after texture-id/alpha
  draw(new SpriteDrawingRequest(sprite, Vector3f((int)translate_stack.back().x + x,
                                                 (int)translate_stack.back().y + y,
                                                  z)));
}

void
DrawingContext::draw_line (float x1, float y1, float x2, float y2, 
                           const Color& color, float z)
{
  draw(new LineDrawingRequest(Vector2i(int(x1 + translate_stack.back().x), int(y1 + translate_stack.back().y)),
    Vector2i(int(x2 + translate_stack.back().x), int(y2 + translate_stack.back().y)), color, z));
}

void
DrawingContext::draw_fillrect (float x1, float y1, float x2, float y2, 
                               const Color& color, float z)
{
  draw(new RectDrawingRequest(Rect(int(x1 + translate_stack.back().x), int(y1 + translate_stack.back().y), 
                                   int(x2 + translate_stack.back().x), int(y2 + translate_stack.back().y)),
                              color,
                              true,
                              z));
}

void
DrawingContext::draw_rect (float x1, float y1, float x2, float y2, 
                           const Color& color, float z)
{
  draw(new RectDrawingRequest(Rect(int(x1 + translate_stack.back().x), int(y1 + translate_stack.back().y), 
                                   int(x2 + translate_stack.back().x), int(y2 + translate_stack.back().y)),
                              color,
                              false,
                              z));
}

void
DrawingContext::draw_pixel (float x_pos, float y_pos, 
                            const Color& color)
{
}

void
DrawingContext::draw_circle (float x_pos, float y_pos, float radius,
                             const Color& color)
{
}

/** Draws an arc, starting from angle_start to angle_end in
      counterclockwise direction. Angles are taken in radian */
void
DrawingContext::draw_arc (float x_pos, float y_pos, float radius, float angle_start, float angle_end,
                          const Color& color)
{
}

void
DrawingContext::fill_screen(const Color& color)
{
  draw(new FillScreenDrawingRequest(color));
}

void
DrawingContext::rotate(float angel)
{
  // FIXME: not implemented
}

void
DrawingContext::scale(float x, float y)
{
  // FIXME: not implemented
}

void
DrawingContext::translate(float x, float y)
{
  translate_stack.back().x += x;
  translate_stack.back().y += y;
}

void
DrawingContext::push_modelview()
{
  translate_stack.push_back(translate_stack.back());
}

void
DrawingContext::pop_modelview()
{
  translate_stack.pop_back();
  assert(!translate_stack.empty());
}

void
DrawingContext::reset_modelview()
{
  translate_stack.clear();
  translate_stack.push_back(Vector3f(0, 0));
}

Rect
DrawingContext::get_world_clip_rect() const
{
  return Rect(Vector2i(static_cast<int>(-translate_stack.back().x),
                          static_cast<int>(-translate_stack.back().y)),
                 Size((int)get_width(), (int)get_height()));
}

void
DrawingContext::set_rect(const Rect& rect_)
{
  rect = rect_;
}

Rect
DrawingContext::get_rect() const
{
  return rect;
}

float
DrawingContext::get_width() const
{
  return rect.get_width();
}

float
DrawingContext::get_height() const
{
  return rect.get_height();  
}

void
DrawingContext::print_left (const Font& font_, float x_pos, float y_pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_, 
                              origin_top_left,
                              Vector3f(x_pos + translate_stack.back().x,
                                        y_pos + translate_stack.back().y),
                              str,
                              z));
}

void
DrawingContext::print_center (const Font& font_, float x_pos, float y_pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_, 
                              origin_top_center,
                              Vector3f(x_pos + translate_stack.back().x,
                                        y_pos + translate_stack.back().y),
                              str,
                              z));
}

void
DrawingContext::print_right (const Font& font_, float x_pos, float y_pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_, 
                              origin_top_right,
                              Vector3f(x_pos + translate_stack.back().x,
                                       y_pos + translate_stack.back().y),
                              str,
                              z));
}

Vector3f
DrawingContext::screen_to_world (Vector3f pos)
{
  return pos - Vector3f(translate_stack.back().x + rect.left,
                        translate_stack.back().y + rect.top);
}

Vector3f
DrawingContext::world_to_screen (Vector3f pos)
{
  return pos + Vector3f(translate_stack.back().x + rect.left, 
                        translate_stack.back().y + rect.top);
}

Vector2i
DrawingContext::screen_to_world(const Vector2i pos)
{
  return pos - Vector2i(int(translate_stack.back().x + rect.left), 
                        int(translate_stack.back().y + rect.top));
}

Vector2i
DrawingContext::world_to_screen(const Vector2i pos)
{
  return pos + Vector2i(int(translate_stack.back().x + rect.left), 
                        int(translate_stack.back().y + rect.top));
}

/* EOF */
