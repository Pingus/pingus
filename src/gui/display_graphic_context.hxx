//  $Id: display_graphic_context.hxx,v 1.8 2004/04/02 18:13:00 grumbel Exp $
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

#ifndef HEADER_DISPLAY_GRAPHIC_CONTEXT_HXX
#define HEADER_DISPLAY_GRAPHIC_CONTEXT_HXX

#include "../vector.hxx"
#include <vector>
#include <ClanLib/Core/Math/rect.h>

#include "graphic_context.hxx"

namespace Pingus {

/** A GraphicContext which represents the display and allows you to
    paint on it */
class DisplayGraphicContext : public GraphicContext
{
private:
  /** Position of the display on the screen */
  int x1, y1, x2, y2;

  /** scroll offset */
  Vector offset;

  /** center of the display */
  Vector center;

  /** Rectangles that need updating */
  std::vector<CL_Rect> change_rects;

  /** Transform worldcoordinate _x*/
  int w2s_x(float x) {
    return static_cast<int>((x + get_x_offset()) * offset.z + center.x);
  }

  int w2s_y(float y) {
    return static_cast<int>((y + get_y_offset()) * offset.z + center.y);
  }

  inline void add_change_rect(int x, int y, int width, int height)
  {
    change_rects.push_back(CL_Rect(x, y, x + width, y + height));
  }
public:
  DisplayGraphicContext (int x1, int y1, int x2, int y2,
			 int /*x_offset*/, int /*y_offset*/);
  virtual ~DisplayGraphicContext ();

  Vector get_offset ();

  float get_x_offset ();
  float get_y_offset ();

  void  set_offset (float x, float y);

  CL_Rect get_clip_rect();

  int   get_width ();
  int   get_height ();

  float get_zoom ();
  void  set_zoom (float new_zoom);

  /** Set the current zoom and offset, so that the given rectangle is
      completly visible on the screen and maximally zoomed. */
  void zoom_to (const CL_Rect & rect);

  /// Scroll the view by the given delta
  void move (const Vector & delta);

  /** Converts a given screen coordinate, as returned by
      CL_Mouse::get_x(), into the world coordinate system. */
  Vector screen_to_world (Vector pos);
  Vector world_to_screen (Vector pos);

  void clear (float r, float g, float b);

  void draw(Pingus::Sprite&, const Pingus::Vector&);
  void draw(Pingus::Sprite&, const Pingus::Vector&, int);

  void draw (CL_Sprite sprite, const Vector& pos);
  void draw (CL_Sprite, const Pingus::Vector&, int);

  void draw (CL_Surface sur, int x_pos, int y_pos);
  void draw (CL_Surface sur, int x_pos, int y_pos, int frame);

  /** Draw a scaled surface */
  void draw (CL_Surface sur, int x_pos, int y_pos,
	     float size_x, float size_y, int frame);

  /** Draw a line */
  void draw_line (const Vector& pos1, const Vector& pos2,
		  float r, float g, float b, float a = 1.0f);
  /** Draw a line */
  void draw_line (int x1, int y1, int x2, int y2,
		  float r, float g, float b, float a = 1.0f);

  /** Draw a filled rectangle (FIXME: [x1,x2] or [x1,x2[ ?) */
  void draw_fillrect (int x1, int y1, int x2, int y2,
		      float r, float g, float b, float a = 1.0f);

  /** Draw an unfilled rectangle (FIXME: [x1,x2] or [x1,x2[ ?) */
  void draw_rect (int x1, int y1, int x2, int y2,
		  float r, float g, float b, float a = 1.0f);

  /** Draw a singel pixel */
  void draw_pixel (int x_pos, int y_pos,
		   float r, float g, float b, float a = 1.0f);

  /** Draw a circle */
  void draw_circle (int x_pos, int y_pos, int radius,
		    float r, float g, float b, float a = 1.0f);

  void print_left (CL_Font font, int x_pos, int y_pos, const std::string& str);

  void print_center (CL_Font font, int x_pos, int y_pos, const std::string& str);

  void print_right (CL_Font font, int x_pos, int y_pos, const std::string& str);

private:
  DisplayGraphicContext (const DisplayGraphicContext&);
  DisplayGraphicContext& operator= (const DisplayGraphicContext&);
};

} // namespace Pingus

#endif

/* EOF */
