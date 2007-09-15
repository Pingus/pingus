//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_RESOURCE_HXX
#define HEADER_PINGUS_RESOURCE_HXX

#include <map>
#include <vector>
#include "sprite.hpp"
#include "surface.hpp"
#include "res_descriptor.hpp"
#include "resource_manager.hpp"
#include "collision_mask.hpp"
#include "font.hpp"

class CL_ResourceManager;

/** General Resource Managing class, it provides wrappers around
    CL_Surface::load(), CL_Font::load() and friends.  This class is
    needed to do a better handling of the resources. */
class Resource
{
public:
  static ResourceManager resmgr;

public:
  static void init();
  static void deinit();

  /** Loads a 48x48 size thumbnail of a sprite */
  static Sprite        load_thumb_sprite(const std::string&);

  static Sprite        load_sprite(const ResDescriptor&);
  static Sprite        load_sprite(const std::string& res_name);

  static CollisionMask load_collision_mask(const std::string& res_name);
  static CollisionMask load_collision_mask(const ResDescriptor&);

  static Surface       load_surface(const std::string& res_name);
  static Surface       load_surface(const ResDescriptor&);

  /** Load a font with res_name from datafile */
  static Font          load_font(const std::string& res_name);

  /** Cleanup all currently unused surfaces */
  static void cleanup ();

private:
  Resource (const Resource&);
  Resource& operator= (const Resource&);
};


#endif

/* EOF */
