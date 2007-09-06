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

#ifndef HEADER_DRAWING_CONTEXT_HXX
#define HEADER_DRAWING_CONTEXT_HXX

//#include <ClanLib/Core/Math/rect.h>
//#include <ClanLib/Display/color.h>
#include "SDL.h"
#include <vector>
#include "drawing_request.hpp"
#include "../math/vector3f.hpp"
#include "../math/rect.hpp"
#include "../math/color.hpp"

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

  std::vector<Vector3f> translate_stack;

  /** The rectangle that the DrawingContext uses on the screen */
  Rect rect;

  bool do_clipping;

public:
  DrawingContext();
  DrawingContext(const Rect& rect, bool clip = true);
  virtual ~DrawingContext();

  /** Draws everything in the drawing context to the screen */
  void render(SDL_Surface* screen, const Rect& rect);

  /** Empties the drawing context */
  void clear();

  /*{ */
  void draw(DrawingRequest* request);

  /** Inserts another DrawingContext into the pipeline, translation is
      ignored. DrawingContext ownership is transfered to this
      DrawingContext FIXME: Not such a good feeling with this, but
      worth a try */
  void draw(DrawingContext& dc, float z = 0);

  void draw(const Sprite& sprite, const Vector3f& pos);
  void draw(const Sprite& sprite, float x, float y, float z = 0);
  
  /** Fills the screen with a given color, this is different from
      clear() in that it doesn't remove other DrawingRequest from the
      queue */
  void fill_screen(const Color& color);

  void draw_line (float x1, float y1, float x2, float y2, 
		  const Color& color, float z = 0);
  void draw_fillrect (float x1, float y1, float x2, float y2, 
		      const Color& color, float z = 0);
  void draw_rect (float x1, float y1, float x2, float y2, 
		  const Color& color, float z = 0);
  void draw_pixel (float x_pos, float y_pos, 
		   const Color& color);
  void draw_circle (float x_pos, float y_pos, float radius,
                    const Color& color);

  /** Draws an arc, starting from angle_start to angle_end in
      counterclockwise direction. Angles are taken in radian */
  void draw_arc (float x_pos, float y_pos, float radius, float angle_start, float angle_end,
                 const Color& color);
  /*} */

  /** Translate the drawing context */
  void translate(float x, float y);

  /** Set the rotation of the drawing context */
  void rotate(float angel);

  /** Set the scaling of the drawing context */
  void scale(float x, float y);

  void push_modelview();
  void pop_modelview();
  void reset_modelview();

  /** Return the area of the screen that will be visible*/
  Rect get_clip_rect() const;

  float get_width() const;
  float get_height() const;

  /** Print a text left aligned */
  void print_left (const Font& font, float x_pos, float y_pos, const std::string& str, float z = 0.0f);

  /** Print a text centred to the given position */
  void print_center (const Font& font, float x_pos, float y_pos, const std::string& str, float z = 0.0f);

  /** Print a text right aligned */
  void print_right (const Font& font, float x_pos, float y_pos, const std::string& str, float z = 0.0f);

  Vector3f screen_to_world (Vector3f pos);
  Vector3f world_to_screen (Vector3f pos);

private:
  DrawingContext (const DrawingContext&);
  DrawingContext& operator= (const DrawingContext&);
};

#endif

/* EOF */
