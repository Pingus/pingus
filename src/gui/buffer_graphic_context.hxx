//  $Id: buffer_graphic_context.hxx,v 1.6 2004/04/02 18:13:00 grumbel Exp $
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

#ifndef HEADER_PINGUS_BUFFER_GRAPHIC_CONTEXT_HXX
#define HEADER_PINGUS_BUFFER_GRAPHIC_CONTEXT_HXX

#include <string>
#include "../vector.hxx"
#include <ClanLib/Core/Math/rect.h>
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/font.h>
#include "graphic_context.hxx"

class CL_Surface;
class CL_Font;

namespace Pingus {

class Sprite;
typedef CL_Font FontHandle;

/** Abstract interface */
class BufferGraphicContext : public GraphicContext
{
private:
  CL_PixelBuffer canvas; 

public:
  BufferGraphicContext(int width, int height);
  virtual ~BufferGraphicContext();
  
  Vector get_offset () { return Vector(); }
  void   set_offset (float, float) {}
  
  CL_Rect get_clip_rect ();

  float get_x_offset () { return 0; }
  float get_y_offset () { return 0; }

  int   get_width ()  { return canvas.get_width(); }
  int   get_height () { return canvas.get_height(); }

  float get_zoom () { return 1.0f; }
  void  set_zoom (float new_zoom) { UNUSED_ARG(new_zoom); }

  void zoom_to (const CL_Rect & rect) { UNUSED_ARG(rect); }
  void move (const Vector & delta) { UNUSED_ARG(delta); }

  Vector screen_to_world (Vector pos) { return pos; }
  Vector world_to_screen (Vector pos) { return pos; }

  void clear (float r, float g, float b);

  void draw(CL_Sprite&, const Pingus::Vector&);

  void draw (CL_Surface& sur, int x_pos, int y_pos);
  void draw (CL_Surface& sur, int x_pos, int y_pos, int frame);
  void draw (CL_Surface& sur, int x_pos, int y_pos,
             float size_x, float size_y, int frame);

  void draw_line (const Vector& /*pos1*/, const Vector& /*pos2*/,
                  float /*r*/, float /*g*/, float /*b*/, float a = 1.0f) { UNUSED_ARG(a); }
  void draw_line (int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/,
                  float /*r*/, float /*g*/, float /*b*/, float a = 1.0f) { UNUSED_ARG(a); }
  void draw_fillrect (int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/,
                      float /*r*/, float /*g*/, float /*b*/, float a = 1.0f) { UNUSED_ARG(a); }
  void draw_rect (int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/,
                  float /*r*/, float /*g*/, float /*b*/, float a = 1.0f) { UNUSED_ARG(a); }
  void draw_pixel (int /*x_pos*/, int /*y_pos*/,
                   float /*r*/, float /*g*/, float /*b*/, float a = 1.0f) { UNUSED_ARG(a); }
  void draw_circle (int /*x_pos*/, int /*y_pos*/, int /*radius*/,
                    float /*r*/, float /*g*/, float /*b*/, float a = 1.0f) { UNUSED_ARG(a); }

  void print_left (FontHandle /*font*/, int /*x_pos*/, int /*y_pos*/, const std::string& /*str*/) {}
  void print_center (FontHandle /*font*/, int /*x_pos*/, int /*y_pos*/, const std::string& /*str*/) {}
  void print_right (FontHandle /*font*/, int /*x_pos*/, int /*y_pos*/, const std::string& /*str*/) {}

  /** Write the current content of the buffer down to a pnm file */
  void write(const std::string& filename);
};

} // namespace Pingus

#endif

/* EOF */
