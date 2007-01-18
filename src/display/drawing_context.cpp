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
#include <iostream>
#include "drawing_context.hpp"
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
  std::string text;
  float x;
  float y;
public:
  FontDrawingRequest(Font font_, Origin origin, const Vector3f& pos, const std::string& text_, float z)
    : DrawingRequest(Vector3f(pos.x, pos.y, z)),
      font(font_), 
      text(text_)
  {
    font.set_alignment(origin);
  }

  virtual ~FontDrawingRequest() {}

  void draw(SDL_Surface* target) {
    font.draw(static_cast<int>(pos.x), static_cast<int>(pos.y), text, target);
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
  {}
  virtual ~SpriteDrawingRequest() {}

  void draw(SDL_Surface* target) {
    sprite.draw(pos.x, pos.y, target);
  }
};

#if 0
class FillScreenDrawingRequest : public DrawingRequest
{
private:
  CL_Color color;
public:
  FillScreenDrawingRequest(const CL_Color& color_) 
    : DrawingRequest(Vector3f(0, 0, -1000.0f)), color(color_)
  {
  }
  virtual ~FillScreenDrawingRequest() {}

  void draw(CL_GraphicContext* gc) {
    gc->clear(color);
  }
};

class SurfaceDrawingRequest : public DrawingRequest
{
private:
  CL_Surface sprite;

public:
  SurfaceDrawingRequest(const CL_Surface& sprite_, const Vector3f& pos_)
    : DrawingRequest(pos_),
      sprite(sprite_)
  {}
  virtual ~SurfaceDrawingRequest() {}

  void draw(CL_GraphicContext* gc) {
    sprite.draw(pos.x, pos.y, gc);
  }
};

class LineDrawingRequest : public DrawingRequest
{
private:
  Vector pos1;
  Vector pos2;
  CL_Color  color;

public:
  LineDrawingRequest(const Vector& pos1_, 
                     const Vector& pos2_, 
                     const CL_Color&  color_,
                     float z)
    : DrawingRequest(Vector3f(0, 0, z)),
      pos1(pos1_),
      pos2(pos2_),
      color(color_)      
  {
  }

  void draw(CL_GraphicContext* gc) 
  {
    gc->draw_line(pos1.x, pos1.y, pos2.x, pos2.y,
                  color);
  }
};

class RectDrawingRequest : public DrawingRequest
{
private:
  CL_Rectf  rect;
  CL_Color color;
  bool     filled;
  
public:
  RectDrawingRequest(const CL_Rectf& rect_, const CL_Color& color_, bool filled_, float z)
    : DrawingRequest(Vector3f(rect.left, rect.top, z)),
      rect(rect_), color(color_), filled(filled_)
  {}
  
  void draw(CL_GraphicContext* gc) 
  {
    if (filled)
      gc->fill_rect(rect, color);
    else
      gc->draw_rect(rect, color);
  }
};

class TextDrawingRequest : public DrawingRequest
{
private:
  std::string text;
public:
  TextDrawingRequest(const std::string& text_, const Vector3f& pos_)
    : DrawingRequest(pos_),
      text(text_)
  {}
  virtual ~TextDrawingRequest() {}

  void draw(CL_GraphicContext* gc) {
    // FIXME: not implemented
  }
};

#endif 
class DrawingContextDrawingRequest : public DrawingRequest
{
private:
  DrawingContext* dc;
  
public:
  DrawingContextDrawingRequest(DrawingContext* dc_, float z)
    : DrawingRequest(Vector3f(0,0,z)),
      dc(dc_)
  {}
  
  virtual ~DrawingContextDrawingRequest()
  {
    delete dc;
  }

  void draw(SDL_Surface* screen) {
    dc->render(screen);
  }
};

DrawingContext::DrawingContext()
{
  translate_stack.push_back(Vector3f(0, 0));
}

DrawingContext::~DrawingContext()
{
  if (drawingrequests.size() > 0) 
		clear();
}

void
DrawingContext::render(SDL_Surface* screen)
{
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
      (*i)->draw(screen);
    }
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
DrawingContext::draw(DrawingContext* dc, float z)
{
  draw(new DrawingContextDrawingRequest(dc, z));
}

#if 0
void
DrawingContext::draw(const CL_Surface&  surface, const Vector& pos)
{
  draw(new SurfaceDrawingRequest(surface, Vector3f(translate_stack.back().x + pos.x,
                                                    translate_stack.back().y + pos.y,
                                                    pos.z))); 
}
#endif 

void
DrawingContext::draw(const Sprite& sprite, const Vector3f& pos)
{
  draw(sprite, pos.x, pos.y, pos.z);
}

void
DrawingContext::draw(const Sprite&   sprite,  float x, float y, float z)
{ // FIXME: This should get flattend down to a simple texture draw
  // command for easier sorting after texture-id/alpha
  draw(new SpriteDrawingRequest(sprite, Vector3f(translate_stack.back().x + x,
                                                  translate_stack.back().y + y,
                                                  z)));
}

#if 0
void
DrawingContext::draw(const std::string& text, float x, float y, float z)
{ 
  draw(new TextDrawingRequest(text, Vector3f(x, y, z)));
}
#endif

void
DrawingContext::draw_line (float x1, float y1, float x2, float y2, 
                           const Color& color, float z)
{
  ////  draw(new LineDrawingRequest(Vector(x1, y1), Vector(x2, y2), color, z));
}

void
DrawingContext::draw_fillrect (float x1, float y1, float x2, float y2, 
                               const Color& color, float z)
{
#if 0
  draw(new RectDrawingRequest(CL_Rectf(x1 + translate_stack.back().x, y1 + translate_stack.back().y, 
                                       x2 + translate_stack.back().x, y2 + translate_stack.back().y),
                              color,
                              true,
                              z));
#endif
}

void
DrawingContext::draw_rect (float x1, float y1, float x2, float y2, 
                           const Color& color, float z)
{
#if 0
  draw(new RectDrawingRequest(CL_Rectf(x1 + translate_stack.back().x, y1 + translate_stack.back().y, 
                                       x2 + translate_stack.back().x, y2 + translate_stack.back().y),
                              color,
                              false,
                              z));
#endif
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
  ////  draw(new FillScreenDrawingRequest(color));
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
DrawingContext::get_clip_rect() const
{
  return Rect(Vector2i(static_cast<int>(-translate_stack.back().x),
                          static_cast<int>(-translate_stack.back().y)),
                 Size((int)get_width(), (int)get_height()));
}
float
DrawingContext::get_width() const
{
  return (float)Display::get_width();
}

float
DrawingContext::get_height() const
{
  return (float)Display::get_height();  
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
  return pos - Vector3f(translate_stack.back().x, translate_stack.back().y);
}

Vector3f
DrawingContext::world_to_screen (Vector3f pos)
{
  return pos + Vector3f(translate_stack.back().x, translate_stack.back().y);
}

/* EOF */
