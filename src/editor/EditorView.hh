//  $Id: EditorView.hh,v 1.6 2002/06/04 13:43:19 torangan Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef EDITORVIEW_HH
#define EDITORVIEW_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>

class Sprite;

class EditorView
{
private:
  int x1, y1, x2, y2;
  CL_Vector center;
  
  /** The current view position, x and y component are the current
      position in the level in world coordinates and the z componet is
      the zoom of the level (1.0 is default)*/
  CL_Vector offset;

public:
  EditorView (int x1, int y1, int x2, int y2, 
	      int x_offset = 0, int y_offset = 0);
  virtual ~EditorView ();
  
  CL_Vector get_offset ();

  float get_x_offset ();
  float get_y_offset ();

  int get_width ();
  int get_height ();

  float     get_zoom ();
  void      set_zoom (float new_zoom);

  /** Set the current zoom and offset, so that the given rectangle is
      completly visible on the screen and maximally zoomed. */
  void zoom_to (const CL_Rect & rect);

  /// Scroll the view by the given delta
  void move (const CL_Vector & delta);

  /** Converts a given screen coordinate, as returned by
      CL_Mouse::get_x(), into the world coordinate system. */
  CL_Vector screen_to_world (CL_Vector pos);
  CL_Vector world_to_screen (CL_Vector pos);

  void draw (Sprite& sprite, const CL_Vector& pos);
  void draw (Sprite& sprite, const CL_Vector& pos, int frame);
  void draw (CL_Surface& sur, const CL_Vector& pos);
  void draw (CL_Surface& sur, const CL_Vector& pos, int frame);
  void draw (CL_Surface& sur, int x_pos, int y_pos);
  void draw (CL_Surface& sur, int x_pos, int y_pos, int frame);
  void draw (CL_Surface& sur, int x_pos, int y_pos, 
	     float size_x, float size_y, int frame);

  void draw_line (const CL_Vector& pos1, const CL_Vector& pos2,
		  float r, float g, float b, float a = 1.0f);
  void draw_line (int x1, int y1, int x2, int y2, 
		  float r, float g, float b, float a = 1.0f);
  void draw_fillrect (int x1, int y1, int x2, int y2, 
		      float r, float g, float b, float a = 1.0f);
  void draw_rect (int x1, int y1, int x2, int y2, 
		  float r, float g, float b, float a = 1.0f);
  void draw_pixel (int x_pos, int y_pos, 
		   float r, float g, float b, float a = 1.0f);
  void draw_circle (int x_pos, int y_pos, int radius,
		   float r, float g, float b, float a = 1.0f);
};

#endif

/* EOF */
