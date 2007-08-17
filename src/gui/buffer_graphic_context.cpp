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

#include <iostream>
#include "../pingus_error.hpp"
#include "../blitter.hpp"
#include "../screenshot.hpp"
#include "buffer_graphic_context.hpp"


BufferGraphicContext::BufferGraphicContext(int width, int height)
  : canvas(width, height, width*4, CL_PixelFormat::rgba8888)
{
}

BufferGraphicContext::~BufferGraphicContext()
{
}

CL_Rect
BufferGraphicContext::get_clip_rect()
{
  return CL_Rect(0,0, get_width(), get_height());
}

void
BufferGraphicContext::clear (float r, float g, float b)
{
  UNUSED_ARG(r);
  UNUSED_ARG(g);
  UNUSED_ARG(b);
  std::cout << "BufferGraphicContext: clear" << std::endl;
}

void 
BufferGraphicContext::draw(CL_Sprite, const Vector3f&)
{
}

void
BufferGraphicContext::draw(CL_Sprite, const Vector3f&, int)
{
}

void
BufferGraphicContext::draw (CL_Surface sur, int x_pos, int y_pos)
{
  //std::cout << "BufferGraphicContext: " << x_pos << ", " << y_pos  << std::endl;
  try {
    Blitter::put_surface(canvas, sur.get_pixeldata(), x_pos, y_pos);
  } catch (PingusError& err) {
    std::cout << "BufferGraphicContext: " << err.get_message() << std::endl;
  }
}

void
BufferGraphicContext::draw (CL_Surface sur, int x_pos, int y_pos, int frame)
{
  //std::cout << "BufferGraphicContext: " << x_pos << ", " << y_pos  << " frame: " << frame << std::endl;
  try {
    Blitter::put_surface(canvas, sur.get_pixeldata(), x_pos, y_pos);
  } catch (PingusError& err) {
    std::cout << "BufferGraphicContext: " << err.get_message() << std::endl;
  }
  UNUSED_ARG(frame);
}

void
BufferGraphicContext::draw (CL_Surface sur, int x_pos, int y_pos,
                            float size_x, float size_y, int frame)
{
  std::cout << "BufferGraphicContext: unsupported" << std::endl;
  UNUSED_ARG(sur);
  UNUSED_ARG(x_pos);
  UNUSED_ARG(y_pos);
  UNUSED_ARG(size_x);
  UNUSED_ARG(size_y);
  UNUSED_ARG(frame);
}

void
BufferGraphicContext::write(const std::string& filename)
{
  std::cout << "BufferGraphicContext::write: " << filename << std::endl;
  canvas.lock();
  Screenshot::save_target_to_file(canvas, filename);
  canvas.unlock();
}


/* EOF */
