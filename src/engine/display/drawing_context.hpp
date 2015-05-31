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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_DRAWING_CONTEXT_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_DRAWING_CONTEXT_HPP

#include <vector>

#include "engine/display/drawing_request.hpp"
#include "math/color.hpp"
#include "math/vector3f.hpp"

class Framebuffer;
class Font;
class Sprite;

/** The DrawingContext collects all DrawingRequests and allows you to
    flush them all down to the graphics card in one run, this has the
    advantage that it is possible to z-sort, texture-id sort or
    otherwise optimize the drawing. In addition to that it also allows
    you do render the drawing commands to multiple buffers which might
    be usefull for post-processing effects and such. */
class DrawingContext
{
private:
  typedef std::vector<DrawingRequest*> DrawingRequests;
  DrawingRequests drawingrequests;

  std::vector<Vector2i> translate_stack;

  /** The rectangle that the DrawingContext uses on the screen */
  Rect rect;

  bool do_clipping;

public:
  DrawingContext();
  DrawingContext(const Rect& rect, bool clip = true);
  virtual ~DrawingContext();

  /** Draws everything in the drawing context to the target */
  void render(Framebuffer& fb, const Rect& rect);

  /** Empties the drawing context */
  void clear();

  /*{ */
  void draw(DrawingRequest* request);

  /** Inserts another DrawingContext into the pipeline, translation is
      ignored. DrawingContext ownership is transfered to this
      DrawingContext FIXME: Not such a good feeling with this, but
      worth a try */
  void draw(DrawingContext& dc, float z = 0);

  void draw(const Sprite& sprite, const Vector2i& pos, float z = 0);
  void draw(const Sprite& sprite, const Vector3f& pos);

  /** Fills the screen with a given color, this is different from
      clear() in that it doesn't remove other DrawingRequest from the
      queue */
  void fill_screen(const Color& color);

  void draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color, float z = 0);
  void draw_fillrect(const Rect& rect, const Color& color, float z = 0);
  void draw_rect(const Rect& rect, const Color& color, float z = 0);
  /*} */

  /** Translate the drawing context */
  void translate(int x, int y);

  void push_modelview();
  void pop_modelview();
  void reset_modelview();

  /** Return the area of the screen that will be visible*/
  Rect get_world_clip_rect() const;

  void set_rect(const Rect& rect);
  Rect get_rect() const;

  int get_width() const;
  int get_height() const;

  /** Print a text left aligned */
  void print_left(const Font& font, const Vector2i& pos, const std::string& str, float z = 0.0f);

  /** Print a text centred to the given position */
  void print_center(const Font& font, const Vector2i& pos, const std::string& str, float z = 0.0f);

  /** Print a text right aligned */
  void print_right(const Font& font, const Vector2i& pos, const std::string& str, float z = 0.0f);

  Vector2i screen_to_world(const Vector2i pos);
  Vector2i world_to_screen(const Vector2i pos);

  void update_layout() {}

private:
  DrawingContext (const DrawingContext&);
  DrawingContext& operator= (const DrawingContext&);
};

#endif

/* EOF */
