// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_COLLISION_MASK_HPP
#define HEADER_PINGUS_PINGUS_COLLISION_MASK_HPP

#include <memory>

#include "engine/display/surface.hpp"

class ResDescriptor;

class CollisionMask
{
public:
  Surface   surface;
  std::unique_ptr<uint8_t[]>  buffer;
  int       width;
  int       height;

public:
  CollisionMask();
  CollisionMask(const std::string& gfx_name, const std::string& col_name);
  CollisionMask(const std::string& name);
  CollisionMask(const ResDescriptor& res_desc);
  ~CollisionMask();

  int get_width() const;
  int get_height() const;

  Surface  get_surface() const;
  uint8_t* get_data() const;

private:
  void init_colmap(const Surface& surf, const std::string& surface_res);

private:
  CollisionMask(const CollisionMask&);
  CollisionMask & operator=(const CollisionMask&);
};

#endif

/* EOF */
