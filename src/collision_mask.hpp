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

#ifndef HEADER_COLLISION_MASK_HPP
#define HEADER_COLLISION_MASK_HPP

#include <string>
#include "surface.hpp"
#include "res_descriptor.hpp"

/** */
class CollisionMask
{
public:
  Surface   surface;
  uint8_t*  buffer;
  int       width;
  int       height;

public:
  CollisionMask();
  CollisionMask(const std::string& name);
  CollisionMask(const ResDescriptor& res_desc);
  ~CollisionMask();
  
  int get_width() const;
  int get_height() const;
  
  Surface get_surface() const;
  uint8_t* get_data() const;

private:
  void init(const ResDescriptor& res_desc);
};

#endif

/* EOF */
