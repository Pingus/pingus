//  $Id: EditorView.cc,v 1.1 2001/05/19 14:22:02 grumbel Exp $
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

#include "EditorView.hh"

EditorView::EditorView (int x1, int y1, int x2, int y2, 
			int x_offset, int y_offset)
  : x1 (x1), y1 (y1), x2 (x2), y2 (y2), offset (-(x2 - x1)/2.0f, -(y2-x1)/2.0f, 1.0f)
{
  center = CL_Vector ((x2 - x1)/2.0f,
		      (y2 - y1)/2.0f);
  std::cout << "View: " << x1 << ", " << y1 << ", " << x2 << ", " << y2 
	    << std::endl;
}

EditorView::~EditorView ()
{
}

CL_Vector
EditorView::get_offset ()
{
  return offset;
}

float
EditorView::get_zoom ()
{
  return offset.z;
}

void
EditorView::set_zoom (float new_zoom)
{
  float factor = new_zoom / offset.z;
  int width  = x2 - x1;
  int height = y2 - y1;
 
  /*
  offset.x += (((width  * offset.z) - (width  * new_zoom)) / 2.0f);
  offset.y += (((height * offset.z) - (height * new_zoom)) / 2.0f);

  std::cout << "Zoom offset: " 
	    << ((width * offset.z) - (width * new_zoom)) / 2.0f
	    << std::endl;
  */
  offset.z = new_zoom;
  std::cout << "Factor:   " << factor << std::endl;
  //d::cout << "New Zoom: " << new_zoom << std::endl;
}

void 
EditorView::zoom_to (int zx1, int zy1, int zx2, int zy2)
{
  std::cout << "EditorView::zoom_to (): not implemented" << std::endl;
}

int 
EditorView::get_width ()
{
  return x2 - x1;
}

int
EditorView::get_height ()
{
  return y2 - y1;
}

void 
EditorView::move (CL_Vector delta)
{
  offset += delta;
}

CL_Vector
EditorView::screen_to_world (CL_Vector pos)
{
  return (pos * (1.0f/offset.z)) - offset;
}

CL_Vector
EditorView::world_to_screen (CL_Vector pos)
{
  return pos * offset.z + offset;
}

float 
EditorView::get_x_offset ()
{
  return offset.x;
}

float 
EditorView::get_y_offset ()
{
  return offset.y;
}

void 
EditorView::draw (CL_Surface& sur, const CL_Vector& pos)
{
  if (offset.z == 1.0)
    {   
      sur.put_screen (int(pos.x + get_x_offset () + center.x),
		      int(pos.y + get_y_offset () + center.y));
    }
  else
    {
      sur.put_screen (int(pos.x + get_x_offset ()) * offset.z + center.x,
		      int(pos.y + get_y_offset ()) * offset.z + center.y,
		      offset.z, offset.z);
    }
  //CL_Display::draw_line (x1, y1, x2, y2, 1.0, 1.0, 0.0);
  //CL_Display::draw_line (x1, y2, x2, y1, 1.0, 1.0, 0.0);
}

void 
EditorView::draw (CL_Surface& sur, int x_pos, int y_pos)
{
  if (offset.z == 1.0)
    {
      sur.put_screen (x_pos + get_x_offset () + center.x,
		      y_pos + get_y_offset () + center.y);
    }
  else
    {
      sur.put_screen ((x_pos + get_x_offset ()) * offset.z + center.x,
		      (y_pos + get_y_offset ()) * offset.z + center.y,
		      offset.z, offset.z);
    }
}

void 
EditorView::draw (CL_Surface& sur, int x_pos, int y_pos, int frame)
{
  if (offset.z == 1.0)
    {
      sur.put_screen (x_pos + get_x_offset () + center.x,
		      y_pos + get_y_offset () + center.y, frame);  
    }
  else
    {
      sur.put_screen ((x_pos + get_x_offset ()) * offset.z + center.x,
		      (y_pos + get_y_offset ()) * offset.z + center.y,
		      offset.z, offset.z,
		      frame);  
    }
}

void 
EditorView::draw (CL_Surface& sur, int x_pos, int y_pos, 
	    float size_x, float size_y, int frame)
{
  sur.put_screen (x_pos + get_x_offset () + center.x,
		  y_pos + get_y_offset () + center.y,
		  size_x * offset.z, 
		  size_y * offset.z, frame); 
}

void 
EditorView::draw_line (int x1, int y1, int x2, int y2, 
		float r, float g, float b, float a)
{
  if (offset.z == 1.0)
    {
      CL_Display::draw_line (x1 + get_x_offset (), y1 + get_y_offset () + center.x,
			     x2 + get_x_offset (), y2 + get_y_offset () + center.y,
			     r, g, b, a);
    }
  else
    {
      CL_Display::draw_line ((x1 + get_x_offset ()) * offset.z + center.x,
			     (y1 + get_y_offset ()) * offset.z + center.y,
			     (x2 + get_x_offset ()) * offset.z + center.x,
			     (y2 + get_y_offset ()) * offset.z + center.y,
			     r, g, b, a);
    }
}

void 
EditorView::draw_fillrect (int x1, int y1, int x2, int y2, 
		    float r, float g, float b, float a)
{
  CL_Display::fill_rect ((x1 + get_x_offset ()) * offset.z + center.x,
			 (y1 + get_y_offset ()) * offset.z + center.y, 
			 (x2 + get_x_offset ()) * offset.z + center.x,
			 (y2 + get_y_offset ()) * offset.z + center.y,
			 r, g, b, a);
}

void 
EditorView::draw_rect (int x1, int y1, int x2, int y2, 
		 float r, float g, float b, float a)
{
  CL_Display::draw_rect ((x1 + get_x_offset ()) * offset.z + center.x,
			 (y1 + get_y_offset ()) * offset.z + center.y, 
			 (x2 + get_x_offset ()) * offset.z + center.x,
			 (y2 + get_y_offset ()) * offset.z + center.y,
			 r, g, b, a);
}

void 
EditorView::draw_pixel (int x_pos, int y_pos, 
		   float r, float g, float b, float a)
{
  //CL_Display::put_pixel (x1 + get_x_offset (),
  //			 y1 + get_y_offset (), r, g, b, a);
  std::cout << "View::draw_pixel () not implemented" << std::endl;
}

void 
EditorView::draw_circle (int x_pos, int y_pos, int radius,
		   float r, float g, float b, float a)
{
  // FIXME: Probally not the fast circle draw algo on this world...
  const float pi = 3.1415927f * 2.0f;
  const float steps = 8;
  CL_Vector current (radius, 0);
  CL_Vector next = current.rotate (pi/steps, CL_Vector (0, 0, 1.0f));

  for (int i = 0; i < steps; ++i)
    {
      draw_line (x_pos + current.x, y_pos + current.y,
		 x_pos + next.x, y_pos + next.y,
		 r, g, b, a);
      current = next;
      next = next.rotate (pi/8, CL_Vector (0, 0, 1.0f));
    }
}

/* EOF */
