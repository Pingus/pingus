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
#include "pixel_buffer.hpp"
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
#if 0
  static std::map<ResDescriptor, CL_Surface> surface_map;

  static CL_Surface load_from_source (const ResDescriptor& res_desc);
  static CL_Surface load_from_cache (const ResDescriptor& res_desc);
  static CL_Surface apply_modifier (const CL_Surface&, const ResDescriptor& res_desc);

  /** Returns a list of resources for the given section.
      Returns all if blank */
  static std::vector<std::string> get_resources(const std::string &type,
                                                const std::string &section = "");

  /** Returns a list of sections under the given section.
      Returns all sections if blank */
  static std::vector<std::string> get_sections(const std::string &section = std::string());
#endif
  static PixelBuffer apply_modifier_to_pixelbuffer(PixelBuffer, const ResDescriptor& res_desc);

public:
  static void init();
  static void deinit();

  /** */
  static unsigned int get_mtime (const std::string& res_name);

  static Sprite        load_sprite(const ResDescriptor&);
  static Sprite        load_sprite(const std::string& res_name);
  static CollisionMask load_collision_mask(const std::string& res_name);
  static CollisionMask load_collision_mask(const ResDescriptor&);
  static PixelBuffer   load_pixelbuffer(const std::string& res_name);
  static PixelBuffer   load_pixelbuffer(const ResDescriptor&);

  /** Load a font with res_name from datafile */
  static Font load_font(const std::string& res_name);

  /** Cleanup all currently unused surfaces */
  static void cleanup ();

private:
  Resource (const Resource&);
  Resource& operator= (const Resource&);
};


#endif

/* EOF */
