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
#include <ClanLib/Display/sprite.h>
#include <ClanLib/Display/font.h>
#include <ClanLib/Display/graphic_context.h>
#include <iostream>
#include "drawing_context.hxx"

namespace Pingus {

struct DrawingRequestsSorter
{
  bool operator()(DrawingRequest* a, DrawingRequest* b) {
    return a->get_z_pos() < b->get_z_pos();
  }
};

class FillScreenDrawingRequest : public DrawingRequest
{
private:
  CL_Color color;
public:
  FillScreenDrawingRequest(const CL_Color& color_) 
    : DrawingRequest(CL_Vector(0, 0, -1000.0f)), color(color_)
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
  SurfaceDrawingRequest(const CL_Surface& sprite_, const CL_Vector& pos_)
    : DrawingRequest(pos_),
      sprite(sprite_)
  {}
  virtual ~SurfaceDrawingRequest() {}

  void draw(CL_GraphicContext* gc) {
    sprite.draw(static_cast<int>(pos.x), static_cast<int>(pos.y), gc);
  }
};

class FontDrawingRequest : public DrawingRequest
{
private:
  CL_Font font;
  std::string text;
  float x;
  float y;
public:
  FontDrawingRequest(CL_Font font_, CL_Origin origin, const CL_Vector& pos, const std::string& text_)
    : DrawingRequest(pos),
      font(font_), text(text_)
  {
    font.set_alignment(origin);
  }

  virtual ~FontDrawingRequest() {}

  void draw(CL_GraphicContext* gc) {
    font.draw(static_cast<int>(pos.x), static_cast<int>(pos.y), text, gc);
  }
};

class LineDrawingRequest : public DrawingRequest
{
private:
  CL_Pointf pos1;
  CL_Pointf pos2;
  CL_Color  color;

public:
  LineDrawingRequest(const CL_Pointf& pos1_, 
                     const CL_Pointf& pos2_, 
                     const CL_Color&  color_,
                     float z)
    : DrawingRequest(CL_Vector(0, 0, z)),
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
    : DrawingRequest(CL_Vector(rect.left, rect.top, z)),
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

class SpriteDrawingRequest : public DrawingRequest
{
private:
  CL_Sprite sprite;

public:
  SpriteDrawingRequest(const CL_Sprite& sprite_, const CL_Vector& pos_)
    : DrawingRequest(pos_),
      sprite(sprite_)
  {}
  virtual ~SpriteDrawingRequest() {}

  void draw(CL_GraphicContext* gc) {
    sprite.draw(static_cast<int>(pos.x), static_cast<int>(pos.y), gc);
  }
};

class TextDrawingRequest : public DrawingRequest
{
private:
  std::string text;
public:
  TextDrawingRequest(const std::string& text_, const CL_Vector& pos_)
    : DrawingRequest(pos_),
      text(text_)
  {}
  virtual ~TextDrawingRequest() {}

  void draw(CL_GraphicContext* gc) {
    // FIXME: not implemented
  }
};

class DrawingContextDrawingRequest : public DrawingRequest
{
private:
  DrawingContext* dc;
  
public:
  DrawingContextDrawingRequest(DrawingContext* dc_, float z)
    : DrawingRequest(CL_Vector(0,0,z)),
      dc(dc_)
  {}
  
  virtual ~DrawingContextDrawingRequest()
  {
    delete dc;
  }

  void draw(CL_GraphicContext* gc) {
    dc->render(gc);
  }
};

DrawingContext::DrawingContext()
{
  translate_stack.push_back(CL_Pointf(0, 0));
}

void
DrawingContext::render(CL_GraphicContext* gc)
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
      (*i)->draw(gc);
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

void
DrawingContext::draw(const CL_Surface&  surface, const Vector& pos)
{
  draw(new SurfaceDrawingRequest(surface, CL_Vector(translate_stack.back().x + pos.x,
                                                    translate_stack.back().y + pos.y,
                                                    pos.z))); 
}

void
DrawingContext::draw(const CL_Sprite& sprite, const Vector& pos, int frame)
{
  draw(sprite, pos.x, pos.y, pos.z);
}

void
DrawingContext::draw(const CL_Sprite&   sprite,  float x, float y, float z)
{ // FIXME: This should get flattend down to a simple texture draw
  // command for easier sorting after texture-id/alpha
  draw(new SpriteDrawingRequest(sprite, CL_Vector(translate_stack.back().x + x,
                                                  translate_stack.back().y + y,
                                                  z)));
}

void
DrawingContext::draw(const std::string& text, float x, float y, float z)
{ 
  draw(new TextDrawingRequest(text, CL_Vector(x, y, z)));
}

void
DrawingContext::draw_line (float x1, float y1, float x2, float y2, 
                           const CL_Color& color, float z)
{
  draw(new LineDrawingRequest(CL_Pointf(x1, y1), CL_Pointf(x2, y2), color, z));
}

void
DrawingContext::draw_fillrect (float x1, float y1, float x2, float y2, 
                               const CL_Color& color, float z)
{
  draw(new RectDrawingRequest(CL_Rectf(x1 + translate_stack.back().x, y1 + translate_stack.back().y, 
                                       x2 + translate_stack.back().x, y2 + translate_stack.back().y),
                              color,
                              true,
                              z));
}

void
DrawingContext::draw_rect (float x1, float y1, float x2, float y2, 
                           const CL_Color& color, float z)
{
  draw(new RectDrawingRequest(CL_Rectf(x1 + translate_stack.back().x, y1 + translate_stack.back().y, 
                                       x2 + translate_stack.back().x, y2 + translate_stack.back().y),
                              color,
                              false,
                              z));
}

void
DrawingContext::draw_pixel (float x_pos, float y_pos, 
                            const CL_Color& color)
{
}

void
DrawingContext::draw_circle (float x_pos, float y_pos, float radius,
                             const CL_Color& color)
{
}

/** Draws an arc, starting from angle_start to angle_end in
      counterclockwise direction. Angles are taken in radian */
void
DrawingContext::draw_arc (float x_pos, float y_pos, float radius, float angle_start, float angle_end,
                          const CL_Color& color)
{
}

void
DrawingContext::fill_screen(const CL_Color& color)
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
  translate_stack.push_back(CL_Pointf(0, 0));
}

CL_Rect
DrawingContext::get_clip_rect()
{
  return CL_Rect(CL_Point(static_cast<int>(-translate_stack.back().x),
                          static_cast<int>(-translate_stack.back().y)),
                 CL_Size(800, 600));
}

float
DrawingContext::get_width() const
{
  return 800;
}

float
DrawingContext::get_height() const
{
  return 600;  
}

void
DrawingContext::print_left (const CL_Font& font_, float x_pos, float y_pos, const std::string& str)
{
  draw(new FontDrawingRequest(font_, 
                              origin_top_left,
                              CL_Vector(x_pos + translate_stack.back().x,
                                        y_pos + translate_stack.back().y),
                              str));
}

void
DrawingContext::print_center (const CL_Font& font_, float x_pos, float y_pos, const std::string& str)
{
  draw(new FontDrawingRequest(font_, 
                              origin_top_center,
                              CL_Vector(x_pos + translate_stack.back().x,
                                        y_pos + translate_stack.back().y),
                              str));
}

void
DrawingContext::print_right (const CL_Font& font_, float x_pos, float y_pos, const std::string& str)
{
  draw(new FontDrawingRequest(font_, 
                              origin_top_right,
                              CL_Vector(x_pos + translate_stack.back().x,
                                        y_pos + translate_stack.back().y),
                              str));
}

Vector
DrawingContext::screen_to_world (Vector pos)
{
  return pos - Vector(translate_stack.back().x, translate_stack.back().y);
}

Vector
DrawingContext::world_to_screen (Vector pos)
{
  return pos + Vector(translate_stack.back().x, translate_stack.back().y);
}

}

/* EOF */
