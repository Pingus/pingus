//  $Id: graphic_context.hxx,v 1.2 2002/09/05 11:26:35 grumbel Exp $
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

#ifndef HEADER_PINGUS_GRAPHIC_CONTEXT_HXX
#define HEADER_PINGUS_GRAPHIC_CONTEXT_HXX

#include <string>
#include <ClanLib/Core/Math/cl_vector.h>
#include <ClanLib/Core/Math/rect.h>

class Sprite;
class CL_Surface;
class CL_Font;

typedef CL_Font* FontHandle;

/** Abstract interface */
class GraphicContext
{
private:
public:
  virtual CL_Vector get_offset () =0;

  /** Return the rectandle which represents the visible part of the
      world, so that objects outsite it can be cliped away easily */
  virtual CL_Rect get_clip_rect () =0;

  virtual float get_x_offset () =0;
  virtual float get_y_offset () =0;

  virtual int   get_width ()  =0;
  virtual int   get_height () =0;

  virtual float get_zoom () =0;
  virtual void  set_zoom (float new_zoom) =0;

  /** Set the current zoom and offset, so that the given rectangle is
      completly visible on the screen and maximally zoomed. */
  virtual void zoom_to (const CL_Rect & rect) =0;

  /// Scroll the view by the given delta
  virtual void move (const CL_Vector & delta) =0;

  /** Converts a given screen coordinate, as returned by
      CL_Mouse::get_x(), into the world coordinate system. */
  virtual CL_Vector screen_to_world (CL_Vector pos) =0;
  virtual CL_Vector world_to_screen (CL_Vector pos) =0;

  virtual void draw (Sprite& sprite, const CL_Vector& pos) =0;
  virtual void draw (Sprite& sprite, const CL_Vector& pos, int frame) =0;
  virtual void draw (CL_Surface& sur, const CL_Vector& pos) =0;
  virtual void draw (CL_Surface& sur, const CL_Vector& pos, int frame) =0;

  virtual void draw (CL_Surface& sur, int x_pos, int y_pos) =0;
  virtual void draw (CL_Surface& sur, int x_pos, int y_pos, int frame) =0;

  /** Draw a scaled surface */
  virtual void draw (CL_Surface& sur, int x_pos, int y_pos, 
		     float size_x, float size_y, int frame) =0;

  /** Draw a line */
  virtual void draw_line (const CL_Vector& pos1, const CL_Vector& pos2,
			  float r, float g, float b, float a = 1.0f) =0;
  /** Draw a line */
  virtual void draw_line (int x1, int y1, int x2, int y2, 
			  float r, float g, float b, float a = 1.0f) =0;

  /** Draw a filled rectangle (FIXME: [x1,x2] or [x1,x2[ ?) */
  virtual void draw_fillrect (int x1, int y1, int x2, int y2, 
			      float r, float g, float b, float a = 1.0f) =0;

  /** Draw an unfilled rectangle (FIXME: [x1,x2] or [x1,x2[ ?) */
  virtual void draw_rect (int x1, int y1, int x2, int y2, 
			  float r, float g, float b, float a = 1.0f) =0;

  /** Draw a singel pixel */
  virtual void draw_pixel (int x_pos, int y_pos, 
			   float r, float g, float b, float a = 1.0f) =0;

  /** Draw a circle */
  virtual void draw_circle (int x_pos, int y_pos, int radius,
			    float r, float g, float b, float a = 1.0f) =0;

  // Font handling routines
  /** Print a text left aligned */
  virtual void print_left (FontHandle font, int x_pos, int y_pos, const std::string& str) =0;

  /** Print a text centred to the given position */
  virtual void print_center (FontHandle font, int x_pos, int y_pos, const std::string& str) =0;
  
  /** Print a text right aligned */
  virtual void print_right (FontHandle font, int x_pos, int y_pos, const std::string& str) =0;
};

#endif

/* EOF */
