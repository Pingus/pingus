//  $Id: editor_view.cxx,v 1.11 2003/10/18 23:17:27 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Core/Math/rect.h>
#include <ClanLib/display.h>
#include "../sprite.hxx"
#include "../gui/display.hxx"
#include "editor_view.hxx"
#include "../math.hxx"

namespace Pingus {
namespace EditorNS {

EditorView::EditorView (int x1_, int y1_, int x2_, int y2_,
			int x_offset, int y_offset)
  : x1(x1_), y1(y1_), x2(x2_), y2(y2_), offset(-(x2_ - x1_)/2.0f, -(y2_-x1_)/2.0f, 1.0f)
{
  center = Vector ((x2 - x1)/2.0f + x1,
                   (y2 - y1)/2.0f + y1);
  //std::cout << "View: " << x1 << ", " << y1 << ", " << x2 << ", " << y2
  //<< std::endl;
  UNUSED_ARG(x_offset);
  UNUSED_ARG(y_offset);
}

EditorView::~EditorView ()
{
}

Vector
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
  offset.z = new_zoom;
}

void
EditorView::zoom_out(int screen_x, int screen_y)
{
  float old_zoom = offset.z;
  set_zoom(old_zoom / 1.2f);
  if (0)
    { // FIXME: Code below doesn't work because of 'center'
      offset.x += screen_x/offset.z - screen_x/old_zoom;
      offset.y += screen_y/offset.z - screen_y/old_zoom;
    }
}

void
EditorView::zoom_in (int screen_x, int screen_y)
{
  float old_zoom = offset.z;
  set_zoom(old_zoom * 1.2f);
  if (0)
    {  // FIXME: Code below doesn't work because of 'center'
      offset.x += screen_x/offset.z - screen_x/old_zoom;
      offset.y += screen_y/offset.z - screen_y/old_zoom;
    }
}

void
EditorView::zoom_to (const CL_Rect & arg_rect)
{
  CL_Rect rect;

  rect.left = Math::min(arg_rect.left, arg_rect.right);
  rect.right = Math::max(arg_rect.left, arg_rect.right);
  rect.top = Math::min(arg_rect.top, arg_rect.bottom);
  rect.bottom = Math::max(arg_rect.top, arg_rect.bottom);

  Vector pos1 = screen_to_world (Vector(rect.left, rect.top));
  Vector pos2 = screen_to_world (Vector(rect.right, rect.bottom));

  Vector center_ = (pos2 + pos1) * 0.5f;
  offset = -center_;

  float width  = pos2.x - pos1.x;
  float height = pos2.y - pos1.y;

  if (width < 10 && height < 10)
    return ;

  float screen_relation = float(get_width ()) / float(get_height ());
  float rect_reation = width / height;

  if (rect_reation > screen_relation)
    {
      set_zoom (get_width () / (pos2.x - pos1.x));
    }
  else
    {
      set_zoom (get_height () / (pos2.y - pos1.y));
    }
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
EditorView::move (const Vector & delta)
{
  offset += delta;
}

Vector
EditorView::screen_to_world (Vector pos)
{
  return ((pos - center) * (1.0f/offset.z)) - offset;
}

Vector
EditorView::world_to_screen (Vector pos)
{
  return ((pos + offset) * offset.z) + center;
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
EditorView::draw (Sprite& sprite, const Vector& pos)
{
  CL_Surface sur (sprite.get_surface ());
  draw (sur,
	(int) pos.x + sprite.get_x_align (),
	(int) pos.y + sprite.get_y_align ());
}

void
EditorView::draw (Sprite& sprite, const Vector& pos, int frame)
{
  CL_Surface sur (sprite.get_surface ());
  draw (sur,
	(int) pos.x + sprite.get_x_align (),
	(int) pos.y + sprite.get_y_align (),
	frame);
}

void
EditorView::draw (CL_Surface& sur, const Vector& pos)
{
  if (offset.z == 1.0)
    {
      sur.draw(int(pos.x + get_x_offset () + center.x),
               int(pos.y + get_y_offset () + center.y));
    }
  else
    {
      sur.draw(int((pos.x + get_x_offset ()) * offset.z + center.x),
               int((pos.y + get_y_offset ()) * offset.z + center.y),
               offset.z, offset.z);
    }
  //CL_Display::draw_line (x1, y1, x2, y2, 1.0, 1.0, 0.0);
  //CL_Display::draw_line (x1, y2, x2, y1, 1.0, 1.0, 0.0);
}

void
EditorView::draw (CL_Surface& sur, const Vector& pos, int frame)
{
  draw (sur, int(pos.x), int(pos.y), frame);
}

void
EditorView::draw (CL_Surface& sur, int x_pos, int y_pos)
{
  if (offset.z == 1.0)
    {
      sur.draw(int(x_pos + get_x_offset () + center.x),
               int(y_pos + get_y_offset () + center.y));
    }
  else
    {
      sur.draw(int((x_pos + get_x_offset ()) * offset.z + center.x),
               int((y_pos + get_y_offset ()) * offset.z + center.y),
               offset.z, offset.z);
    }
}

void
EditorView::draw (CL_Surface& sur, int x_pos, int y_pos, int frame)
{
  if (offset.z == 1.0)
    {
      sur.draw(int(x_pos + get_x_offset () + center.x),
               int(y_pos + get_y_offset () + center.y),
               frame);
    }
  else
    {
      sur.draw(int((x_pos + get_x_offset ()) * offset.z + center.x),
               int((y_pos + get_y_offset ()) * offset.z + center.y),
               offset.z, offset.z,
               frame);
    }
}

void
EditorView::draw (CL_Surface& sur, int x_pos, int y_pos,
                  float size_x, float size_y, int frame)
{
  sur.draw(int(x_pos + get_x_offset () + center.x),
           int(y_pos + get_y_offset () + center.y),
           size_x * offset.z,
           size_y * offset.z, frame);
}

void
EditorView::draw_line (const Vector& pos1, const Vector& pos2,
		       float r, float g, float b, float a)
{
  draw_line (int(pos1.x), int(pos1.y), int(pos2.x), int(pos2.y), r, g, b, a);
}

void
EditorView::draw_line (int x1_, int y1_, int x2_, int y2_,
		       float r, float g, float b, float a)
{
  CL_Display::draw_line (static_cast<int>((x1_ + get_x_offset()) * offset.z + center.x),
			 static_cast<int>((y1_ + get_y_offset()) * offset.z + center.y),
			 static_cast<int>((x2_ + get_x_offset()) * offset.z + center.x),
			 static_cast<int>((y2_ + get_y_offset()) * offset.z + center.y),
			 Display::to_color(r, g, b, a));
}

void
EditorView::draw_fillrect (int x1_, int y1_, int x2_, int y2_,
			   float r, float g, float b, float a)
{
  CL_Display::fill_rect(CL_Rect(static_cast<int>((x1_ + get_x_offset()) * offset.z + center.x),
                                static_cast<int>((y1_ + get_y_offset()) * offset.z + center.y),
                                static_cast<int>((x2_ + get_x_offset()) * offset.z + center.x),
                                static_cast<int>((y2_ + get_y_offset()) * offset.z + center.y)),
                        Display::to_color(r, g, b, a));
}

void
EditorView::draw_rect (int x1_, int y1_, int x2_, int y2_,
		       float r, float g, float b, float a)
{
  CL_Display::draw_rect(CL_Rect(static_cast<int>((x1_ + get_x_offset()) * offset.z + center.x),
                                static_cast<int>((y1_ + get_y_offset()) * offset.z + center.y),
                                static_cast<int>((x2_ + get_x_offset()) * offset.z + center.x),
                                static_cast<int>((y2_ + get_y_offset()) * offset.z + center.y)),
			Display::to_color(r, g, b, a));
}

void
EditorView::draw_pixel (int /*x_pos*/, int /*y_pos*/,
                        float /*r*/, float /*g*/, float /*b*/, float /*a*/)
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
  Vector current (radius, 0);
  Vector next = current.rotate (pi/steps, Vector (0, 0, 1.0f));

  for (int i = 0; i < steps; ++i)
    {
      draw_line (int(x_pos + current.x), int(y_pos + current.y),
		 int(x_pos + next.x), int(y_pos + next.y),
		 r, g, b, a);
      current = next;
      next = next.rotate (pi/8, Vector (0, 0, 1.0f));
    }
}

} // namespace EditorNS
} // namespace Pingus

/* EOF */
