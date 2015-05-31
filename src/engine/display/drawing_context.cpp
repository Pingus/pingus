// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/display/drawing_context.hpp"

#include <algorithm>

#include "engine/display/display.hpp"
#include "engine/display/font.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/display/sprite.hpp"
#include "util/log.hpp"

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

public:
  FontDrawingRequest(Font font_, Origin origin_, const Vector2i& pos_, const std::string& text_, float z_)
    : DrawingRequest(pos_, z_),
      font(font_),
      origin(origin_),
      text(text_)
  {
  }

  virtual ~FontDrawingRequest() {}

  void render(Framebuffer& fb, const Rect& rect) {
    font.render(origin, static_cast<int>(pos.x + rect.left), static_cast<int>(pos.y + rect.top), text, fb);
  }
};

class SpriteDrawingRequest : public DrawingRequest
{
private:
  Sprite sprite;

public:
  SpriteDrawingRequest(const Sprite& sprite_, const Vector2i& pos_, float z_)
    : DrawingRequest(pos_, z_),
      sprite(sprite_)
  {
  }

  virtual ~SpriteDrawingRequest() {}

  void render(Framebuffer& fb, const Rect& rect) {
    sprite.render(pos.x + rect.left, pos.y + rect.top, fb);
  }
};

class FillScreenDrawingRequest : public DrawingRequest
{
private:
  Color color;

public:
  FillScreenDrawingRequest(const Color& color_)
    : DrawingRequest(Vector2i(0, 0), -1000.0f),
      color(color_)
  {
  }
  virtual ~FillScreenDrawingRequest() {}

  void render(Framebuffer& fb, const Rect& rect) {
    fb.fill_rect(rect, color);
  }
};

class LineDrawingRequest : public DrawingRequest
{
private:
  Vector2i pos1;
  Vector2i pos2;
  Color    color;

public:
  LineDrawingRequest(const Vector2i& pos1_,
                     const Vector2i& pos2_,
                     const Color&  color_,
                     float z_)
    : DrawingRequest(Vector2i(0, 0), z_),
      pos1(pos1_),
      pos2(pos2_),
      color(color_)
  {
  }

  void render(Framebuffer& fb, const Rect& rect)
  {
    fb.draw_line(pos1 + Vector2i(rect.left, rect.top),
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
  RectDrawingRequest(const Rect& rect_, const Color& color_, bool filled_, float z_)
    : DrawingRequest(Vector2i(0, 0), z_),
      d_rect(rect_), color(color_), filled(filled_)
  {}

  void render(Framebuffer& fb, const Rect& rect)
  {
    if (filled)
    {
      fb.fill_rect(Rect(Vector2i(d_rect.left + rect.left,
                                 d_rect.top  + rect.top),
                        d_rect.get_size()),
                   color);
    }
    else
    {
      fb.draw_rect(Rect(Vector2i(d_rect.left + rect.left,
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
  DrawingContextDrawingRequest(DrawingContext& dc_, float z_)
    : DrawingRequest(Vector2i(0,0), z_),
      dc(dc_)
  {}

  virtual ~DrawingContextDrawingRequest()
  {
  }

  void render(Framebuffer& fb, const Rect& rect) {
    dc.render(fb, rect);
  }
};

DrawingContext::DrawingContext(const Rect& rect_, bool clip) :
  drawingrequests(),
  translate_stack(),
  rect(rect_),
  do_clipping(clip)
{
  translate_stack.push_back(Vector2i(0, 0));
}

DrawingContext::DrawingContext() :
  drawingrequests(),
  translate_stack(),
  rect(0, 0, Display::get_width(), Display::get_height()),
  do_clipping(false)
{
  translate_stack.push_back(Vector2i(0, 0));
}

DrawingContext::~DrawingContext()
{
  if (drawingrequests.size() > 0)
    clear();
}

void
DrawingContext::render(Framebuffer& fb, const Rect& parent_rect)
{
  Rect this_rect(Math::max(rect.left   + parent_rect.left, parent_rect.left),
                 Math::max(rect.top    + parent_rect.top,  parent_rect.top),
                 Math::min(rect.right  + parent_rect.left, parent_rect.right),
                 Math::min(rect.bottom + parent_rect.top,  parent_rect.bottom));

  if (do_clipping)
    fb.push_cliprect(this_rect);

  std::stable_sort(drawingrequests.begin(), drawingrequests.end(), DrawingRequestsSorter());

  if (0)
  {
    log_info("<<<<<<<<<<<<<<");
    for(DrawingRequests::iterator i = drawingrequests.begin(); i != drawingrequests.end(); ++i)
      log_info("%1%", (*i)->get_z_pos());
    log_info(">>>>>>>>>>>>>>");
  }
  for(DrawingRequests::iterator i = drawingrequests.begin(); i != drawingrequests.end(); ++i)
  {
    //log_info("%1%", this << ": " << (*i)->get_z_pos());
    (*i)->render(fb, this_rect); // FIXME: Should we clip size against parent rect?
  }

  if (do_clipping)
    fb.pop_cliprect();
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
DrawingContext::draw(const Sprite& sprite, const Vector2i& pos, float z)
{
  draw(new SpriteDrawingRequest(sprite, pos + translate_stack.back(), z));
}

void
DrawingContext::draw(const Sprite& sprite, const Vector3f& pos)
{
  draw(new SpriteDrawingRequest(sprite, Vector2i(translate_stack.back().x + static_cast<int>(pos.x),
                                                 translate_stack.back().y + static_cast<int>(pos.y)),
                                pos.z));
}

void
DrawingContext::draw_line(const Vector2i& pos1, const Vector2i& pos2,
                          const Color& color, float z)
{
  draw(new LineDrawingRequest(pos1 + translate_stack.back(),
                              pos2 + translate_stack.back(),
                              color, z));
}

void
DrawingContext::draw_fillrect(const Rect& rect_, const Color& color_, float z_)
{
  draw(new RectDrawingRequest(Rect(int(rect_.left + translate_stack.back().x),
                                   int(rect_.top + translate_stack.back().y),
                                   int(rect_.right + translate_stack.back().x),
                                   int(rect_.bottom + translate_stack.back().y)),
                              color_,
                              true,
                              z_));
}

void
DrawingContext::draw_rect(const Rect& rect_, const Color& color_, float z_)
{
  draw(new RectDrawingRequest(Rect(int(rect_.left   + translate_stack.back().x),
                                   int(rect_.top    + translate_stack.back().y),
                                   int(rect_.right  + translate_stack.back().x),
                                   int(rect_.bottom + translate_stack.back().y)),
                              color_,
                              false,
                              z_));
}

void
DrawingContext::fill_screen(const Color& color)
{
  draw(new FillScreenDrawingRequest(color));
}

void
DrawingContext::translate(int x, int y)
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
  translate_stack.push_back(Vector2i(0, 0));
}

Rect
DrawingContext::get_world_clip_rect() const
{
  return Rect(Vector2i(static_cast<int>(-translate_stack.back().x),
                       static_cast<int>(-translate_stack.back().y)),
              Size(static_cast<int>(get_width()), static_cast<int>(get_height())));
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

int
DrawingContext::get_width() const
{
  return rect.get_width();
}

int
DrawingContext::get_height() const
{
  return rect.get_height();
}

void
DrawingContext::print_left(const Font& font_, const Vector2i& pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_,
                              origin_top_left,
                              pos + translate_stack.back(),
                              str,
                              z));
}

void
DrawingContext::print_center(const Font& font_, const Vector2i& pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_,
                              origin_top_center,
                              pos + translate_stack.back(),
                              str,
                              z));
}

void
DrawingContext::print_right(const Font& font_, const Vector2i& pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_,
                              origin_top_right,
                              pos + translate_stack.back(),
                              str,
                              z));
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
