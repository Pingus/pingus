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

#include <geom/offset.hpp>
#include <logmich/log.hpp>

#include "engine/display/display.hpp"
#include "engine/display/font.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/display/sprite.hpp"

namespace pingus {

struct DrawingRequestsSorter
{
  bool operator()(DrawingRequest* a, DrawingRequest* b) {
    return a->z_index() < b->z_index();
  }
};

class FontDrawingRequest : public DrawingRequest
{
private:
  Font font;
  Origin origin;
  std::string text;

public:
  FontDrawingRequest(const Font& font_, Origin origin_, const Vector2i& pos_, const std::string& text_, float z_)
    : DrawingRequest(pos_, z_),
      font(font_),
      origin(origin_),
      text(text_)
  {
  }

  ~FontDrawingRequest() override {}

  void render(Framebuffer& fb, const Rect& rect) override {
    font.render(origin, pos.x() + rect.left(), pos.y() + rect.top(), text, fb);
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

  ~SpriteDrawingRequest() override {}

  void render(Framebuffer& fb, const Rect& rect) override {
    sprite.render(pos.x() + rect.left(), pos.y() + rect.top(), fb);
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
  ~FillScreenDrawingRequest() override {}

  void render(Framebuffer& fb, const Rect& rect) override {
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

  void render(Framebuffer& fb, const Rect& rect) override
  {
    fb.draw_line(pos1 + geom::ioffset(rect.left(), rect.top()),
                 pos2 + geom::ioffset(rect.left(), rect.top()), color);
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

  void render(Framebuffer& fb, const Rect& rect) override
  {
    if (filled)
    {
      fb.fill_rect(Rect(geom::ipoint(d_rect.left() + rect.left(),
                                 d_rect.top()  + rect.top()),
                        d_rect.size()),
                   color);
    }
    else
    {
      fb.draw_rect(Rect(geom::ipoint(d_rect.left() + rect.left(),
                                 d_rect.top()  + rect.top()),
                        d_rect.size()),
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

  ~DrawingContextDrawingRequest() override
  {
  }

  void render(Framebuffer& fb, const Rect& rect) override {
    dc.render(fb, rect);
  }
};

DrawingContext::DrawingContext(const Rect& rect_, bool clip) :
  drawingrequests(),
  translate_stack(),
  rect(rect_),
  do_clipping(clip)
{
  translate_stack.emplace_back(0, 0);
}

DrawingContext::DrawingContext() :
  drawingrequests(),
  translate_stack(),
  rect(0, 0, Display::get_width(), Display::get_height()),
  do_clipping(false)
{
  translate_stack.emplace_back(0, 0);
}

DrawingContext::~DrawingContext()
{
  if (!drawingrequests.empty())
    clear();
}

void
DrawingContext::render(Framebuffer& fb, const Rect& parent_rect)
{
  Rect this_rect(std::max(rect.left()   + parent_rect.left(), parent_rect.left()),
                 std::max(rect.top()    + parent_rect.top(),  parent_rect.top()),
                 std::min(rect.right()  + parent_rect.left(), parent_rect.right()),
                 std::min(rect.bottom() + parent_rect.top(),  parent_rect.bottom()));

  if (do_clipping)
    fb.push_cliprect(this_rect);

  std::stable_sort(drawingrequests.begin(), drawingrequests.end(), DrawingRequestsSorter());

  if (0)
  {
    log_info("<<<<<<<<<<<<<<");
    for(DrawingRequests::iterator i = drawingrequests.begin(); i != drawingrequests.end(); ++i)
      log_info("{}", (*i)->z_index());
    log_info(">>>>>>>>>>>>>>");
  }
  for(DrawingRequests::iterator i = drawingrequests.begin(); i != drawingrequests.end(); ++i)
  {
    //log_info("{}", this << ": " << (*i)->z_index());
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
DrawingContext::draw(const Sprite& sprite, const Vector2f& pos, float z_index)
{
  draw(new SpriteDrawingRequest(sprite, Vector2i(translate_stack.back().x() + static_cast<int>(pos.x()),
                                                 translate_stack.back().y() + static_cast<int>(pos.y())),
                                z_index));
}

void
DrawingContext::draw_line(const Vector2i& pos1, const Vector2i& pos2,
                          const Color& color, float z)
{
  draw(new LineDrawingRequest(pos1.as_vec() + translate_stack.back().as_vec(),
                              pos2.as_vec() + translate_stack.back().as_vec(),
                              color, z));
}

void
DrawingContext::draw_fillrect(const Rect& rect_, const Color& color_, float z_)
{
  draw(new RectDrawingRequest(Rect(rect_.left() + translate_stack.back().x(),
                                   rect_.top() + translate_stack.back().y(),
                                   rect_.right() + translate_stack.back().x(),
                                   rect_.bottom() + translate_stack.back().y()),
                              color_,
                              true,
                              z_));
}

void
DrawingContext::draw_rect(const Rect& rect_, const Color& color_, float z_)
{
  draw(new RectDrawingRequest(Rect(rect_.left()   + translate_stack.back().x(),
                                   rect_.top()    + translate_stack.back().y(),
                                   rect_.right()  + translate_stack.back().x(),
                                   rect_.bottom() + translate_stack.back().y()),
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
  translate_stack.back() = translate_stack.back() + geom::ioffset(x, y);
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
  translate_stack.emplace_back(0, 0);
}

Rect
DrawingContext::get_world_clip_rect() const
{
  return Rect(geom::ipoint(-translate_stack.back().x(),
                           -translate_stack.back().y()),
              Size(get_width(), get_height()));
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
  return rect.width();
}

int
DrawingContext::get_height() const
{
  return rect.height();
}

void
DrawingContext::print_left(const Font& font_, const Vector2i& pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_,
                              Origin::TOP_LEFT,
                              pos + translate_stack.back(),
                              str,
                              z));
}

void
DrawingContext::print_center(const Font& font_, const Vector2i& pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_,
                              Origin::TOP_CENTER,
                              pos + translate_stack.back(),
                              str,
                              z));
}

void
DrawingContext::print_right(const Font& font_, const Vector2i& pos, const std::string& str, float z)
{
  draw(new FontDrawingRequest(font_,
                              Origin::TOP_RIGHT,
                              pos + translate_stack.back(),
                              str,
                              z));
}

Vector2i
DrawingContext::screen_to_world(const Vector2i& pos)
{
  return pos - geom::ioffset(translate_stack.back().x() + rect.left(),
                             translate_stack.back().y() + rect.top());
}

Vector2i
DrawingContext::world_to_screen(const Vector2i& pos)
{
  return pos + geom::ioffset(translate_stack.back().x() + rect.left(),
                             translate_stack.back().y() + rect.top());
}

} // namespace pingus

/* EOF */
