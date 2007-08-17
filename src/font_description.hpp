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

#ifndef HEADER_FONT_DESCRIPTION_HPP
#define HEADER_FONT_DESCRIPTION_HPP

#include <string>

/** */
class FontDescription
{
public:
  std::string filename;
  std::string name;

  /** Image file from which the basic surface is loaded */
  std::string image;

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

  FontDescription(const std::string& filename);
};

#endif

/* EOF */
