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

#include <ClanLib/Core/Math/rect.h>
#include <ClanLib/Display/color.h>
#include <vector>
#include "drawing_request.hxx"
#include "../vector.hxx"

class CL_Font;
class CL_Sprite;
class CL_Surface;

namespace Pingus {

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

  std::vector<CL_Pointf> translate_stack;

public:
  DrawingContext();

  /** Draws everything in the drawing context to the screen */
  void render(CL_GraphicContext* gc);

  /** Empties the drawing context */
  void clear();

  /** Fills the screen with a given color, this is different from
      clear() in that it doesn't remove other DrawingRequest from the
      queue */
  void fill_screen(const CL_Color& color);

  /*{ */
  void draw(DrawingRequest* request);

  /** Inserts another DrawingContext into the pipeline, translation is
      ignored. DrawingContext ownership is transfered to this
      DrawingContext FIXME: Not such a good feeling with this, but
      worth a try */
  void draw(DrawingContext* dc, float z = 0);

  void draw(const CL_Surface&  surface, const Vector& pos);
  void draw(const CL_Sprite&   sprite,  float x, float y, float z = 0);
  void draw(const std::string& text,    float x, float y, float z = 0);

  void draw(const CL_Sprite&,   const Vector& pos, int frame = -1);

  void draw_line (float x1, float y1, float x2, float y2, 
		  const CL_Color& color, float z = 0);
  void draw_fillrect (float x1, float y1, float x2, float y2, 
		      const CL_Color& color, float z = 0);
  void draw_rect (float x1, float y1, float x2, float y2, 
		  const CL_Color& color, float z = 0);
  void draw_pixel (float x_pos, float y_pos, 
		   const CL_Color& color);
  void draw_circle (float x_pos, float y_pos, float radius,
                    const CL_Color& color);

  /** Draws an arc, starting from angle_start to angle_end in
      counterclockwise direction. Angles are taken in radian */
  void draw_arc (float x_pos, float y_pos, float radius, float angle_start, float angle_end,
                 const CL_Color& color);
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
  CL_Rect get_clip_rect();

  float get_width() const;
  float get_height() const;

  /** Print a text left aligned */
  void print_left (const CL_Font& font, float x_pos, float y_pos, const std::string& str);

  /** Print a text centred to the given position */
  void print_center (const CL_Font& font, float x_pos, float y_pos, const std::string& str);

  /** Print a text right aligned */
  void print_right (const CL_Font& font, float x_pos, float y_pos, const std::string& str);

  Vector screen_to_world (Vector pos);
  Vector world_to_screen (Vector pos);

private:
  DrawingContext (const DrawingContext&);
  DrawingContext& operator= (const DrawingContext&);
};

}

#endif

/* EOF */
