//  $Id: font_description.hpp 3090 2007-09-05 19:08:47Z grumbel $
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

#ifndef HEADER_FONT_DESCRIPTION_HPP
#define HEADER_FONT_DESCRIPTION_HPP

#include <string>
#include "pathname.hpp"

/** */
class FontDescription
{
public:
  Pathname    pathname;
  std::string name;

  /** Image file from which the basic surface is loaded */
  Pathname image;

  bool monospace;
  
  /** Length of the space character, ignored with monospace fonts */
  int  space_length; 

  /** Space between two characters */
  float char_spacing;

  float vertical_spacing;

  /** Minimum amount of alpha that is handled as character seperator */
  int  alpha_threshold;

  /** Characters in the font image */
  std::string characters;

  FontDescription(const Pathname& filename);
};

#endif

/* EOF */
