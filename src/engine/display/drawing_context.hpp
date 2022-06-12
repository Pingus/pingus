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

#include <geom/offset.hpp>
#include <geom/point.hpp>

#include "engine/display/drawing_request.hpp"
#include "math/color.hpp"

namespace pingus {

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

  std::vector<geom::ioffset> translate_stack;

  /** The rectangle that the DrawingContext uses on the screen */
  geom::irect rect;

  bool do_clipping;

public:
  DrawingContext();
  DrawingContext(geom::irect const& rect, bool clip = true);
  virtual ~DrawingContext();

  /** Draws everything in the drawing context to the target */
  void render(Framebuffer& fb, geom::irect const& rect);

  /** Empties the drawing context */
  void clear();

  /*{ */
  void draw(DrawingRequest* request);

  /** Inserts another DrawingContext into the pipeline, translation is
      ignored. DrawingContext ownership is transfered to this
      DrawingContext FIXME: Not such a good feeling with this, but
      worth a try */
  void draw(DrawingContext& dc, float z = 0);

  void draw(Sprite const& sprite, geom::ipoint const& pos, float z = 0);
  void draw(Sprite const& sprite, geom::fpoint const& pos, float z = 0);

  /** Fills the screen with a given color, this is different from
      clear() in that it doesn't remove other DrawingRequest from the
      queue */
  void fill_screen(Color const& color);

  void draw_line(geom::ipoint const& pos1, geom::ipoint const& pos2, Color const& color, float z = 0);
  void draw_fillrect(geom::irect const& rect, Color const& color, float z = 0);
  void draw_rect(geom::irect const& rect, Color const& color, float z = 0);
  /*} */

  /** Translate the drawing context */
  void translate(int x, int y);

  void push_modelview();
  void pop_modelview();
  void reset_modelview();

  /** Return the area of the screen that will be visible*/
  geom::irect get_world_clip_rect() const;

  void set_rect(geom::irect const& rect);
  geom::irect get_rect() const;

  int get_width() const;
  int get_height() const;

  /** Print a text left aligned */
  void print_left(Font const& font, geom::ipoint const& pos, std::string const& str, float z = 0.0f);

  /** Print a text centred to the given position */
  void print_center(Font const& font, geom::ipoint const& pos, std::string const& str, float z = 0.0f);

  /** Print a text right aligned */
  void print_right(Font const& font, geom::ipoint const& pos, std::string const& str, float z = 0.0f);

  geom::ipoint screen_to_world(geom::ipoint const& pos);
  geom::ipoint world_to_screen(geom::ipoint const& pos);

  void update_layout() {}

private:
  DrawingContext (DrawingContext const&);
  DrawingContext& operator= (DrawingContext const&);
};

} // namespace pingus

#endif

/* EOF */
