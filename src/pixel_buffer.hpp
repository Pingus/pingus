//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PIXEL_BUFFER_HPP
#define HEADER_PIXEL_BUFFER_HPP

#include "SDL.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include "math/color.hpp"

class PixelBufferImpl;

/** */
class PixelBuffer
{
public:
  PixelBuffer();
  PixelBuffer(const std::string& name);
  /** Create an empty RGBA PixelBuffer */
  PixelBuffer(int width, int height);

  /** Create an empty Indexed PixelBuffer (8bit) */
  PixelBuffer(int width, int height, SDL_Palette* palette, int colorkey = -1);
  /** Create a PixelBuffer from a SDL_Surface */
  PixelBuffer(SDL_Surface* surface);
  ~PixelBuffer();

  uint8_t* get_data() const;
  void lock();
  void unlock();

  int get_width()  const;
  int get_height() const;
  int get_pitch()  const;

  void blit(const PixelBuffer& source, int x, int y);

  Color get_pixel(int x, int y) const;

  SDL_Surface* get_surface() const;

  operator bool() const;

protected:
  boost::shared_ptr<PixelBufferImpl> impl;
};

#endif

/* EOF */
