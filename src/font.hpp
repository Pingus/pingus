/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

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
