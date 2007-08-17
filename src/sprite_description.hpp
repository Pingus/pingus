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

#ifndef HEADER_SPRITE_DESCRIPTION_HPP
#define HEADER_SPRITE_DESCRIPTION_HPP

#include "math/vector2i.hpp"
#include "math/size.hpp"
#include "math/origin.hpp"
#include "file_reader.hpp"

/** */
class SpriteDescription
{
public:
  std::string filename;
  Vector2i    offset;
  Origin      origin;

  bool        loop;
  int         speed; 
  Size        array;
  Vector2i    frame_pos;
  Size        frame_size;  

  SpriteDescription() 
    : origin(origin_top_left),
      loop(true),
      speed(100),
      array(1, 1),
      frame_size(-1, -1)
  {}

  SpriteDescription(const FileReader& reader)
    : origin(origin_top_left),
      loop(true),
      speed(100),
      array(1, 1),
      frame_size(-1, -1)
  {
    reader.read_int   ("speed",  speed);
    reader.read_bool  ("loop",   loop);
    reader.read_vector2i("offset", offset);

    reader.read_enum("origin", origin, string2origin);
    
    reader.read_string("image-file",  filename);
    filename = "data/data/" + filename;// FIXME: Hack
    reader.read_size("image-array", array);
    reader.read_vector2i("image-pos",   frame_pos);
    reader.read_size("image-size",  frame_size);
  }

  ~SpriteDescription()
  {  
  }
};

#endif

/* EOF */
