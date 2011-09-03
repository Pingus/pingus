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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_SPRITE_DESCRIPTION_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_SPRITE_DESCRIPTION_HPP

#include <iostream>
#include <memory>

#include "math/origin.hpp"
#include "math/size.hpp"
#include "math/vector2i.hpp"
#include "util/file_reader.hpp"
#include "util/pathname.hpp"

class SpriteDescription;

typedef std::shared_ptr<SpriteDescription> SpriteDescriptionPtr;

class SpriteDescription
{
public:
  static SpriteDescriptionPtr from_file(const Pathname& path);

public:
  Pathname    filename;
  Vector2i    offset;
  Origin      origin;

  bool        loop;
  int         speed; 
  Size        array;
  Vector2i    frame_pos;
  Size        frame_size;  

  SpriteDescription() :
    filename(),
    offset(),
    origin(origin_top_left),
    loop(true),
    speed(100),
    array(1, 1),
    frame_pos(),
    frame_size(-1, -1)
  {}

  SpriteDescription(const FileReader& reader) :
    filename(),
    offset(),
    origin(origin_top_left),
    loop(true),
    speed(100),
    array(1, 1),
    frame_pos(),
    frame_size(-1, -1)
  {
    reader.read_int   ("speed",  speed);
    reader.read_bool  ("loop",   loop);
    reader.read_vector2i("offset", offset);

    reader.read_enum("origin", origin, string2origin);
    
    if (!reader.read_path("image-file",  filename))
    {
      std::cout << "Error: SpriteDescription: " << reader.get_name() << " 'image-file' missing" << std::endl;
    }    

    filename = Pathname("data/" + filename.get_raw_path(), filename.get_type()); // FIXME: Hack
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
