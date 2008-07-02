//  $Id: font.hpp 2987 2007-08-17 16:25:49Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_FONT_HPP
#define HEADER_FONT_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include "SDL.h"
#include "math/origin.hpp"
#include "math/rect.hpp"
#include "math/size.hpp"

class FontImpl;
class FontDescription;

/** */
class Font
{
public:
  Font();
  Font(const FontDescription& desc);

  void draw(int x, int y, const std::string& text, SDL_Surface* target = 0);
  void draw(Origin origin, int x, int y, const std::string& text, SDL_Surface* target = 0);

  int  get_height() const;
  int  get_width(char) const;
  int  get_width(const std::string& ) const;
  Size get_size(const std::string& str) const;
  Rect bounding_rect(int , int, const std::string& str) const;

private:
  boost::shared_ptr<FontImpl> impl;
};

#endif

/* EOF */
