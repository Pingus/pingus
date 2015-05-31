// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_RESOURCE_HPP
#define HEADER_PINGUS_PINGUS_RESOURCE_HPP

#include "engine/display/font.hpp"
#include "engine/display/sprite.hpp"
#include "engine/display/surface.hpp"
#include "pingus/res_descriptor.hpp"
#include "pingus/resource_manager.hpp"

class Resource
{
public:
  static ResourceManager resmgr;

public:
  static void init();
  static void deinit();

  /** Loads a 48x48 size thumbnail of a sprite */
  static Sprite        load_thumb_sprite(const std::string&);

  static SpriteDescription* load_sprite_desc(const std::string& res_name);

  static Surface       load_surface(const std::string& res_name);
  static Surface       load_surface(const ResDescriptor&);

  /** Load a font with res_name from datafile */
  static Font          load_font(const std::string& res_name);

private:
  Resource (const Resource&);
  Resource& operator= (const Resource&);
};

#endif

/* EOF */
