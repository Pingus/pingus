//  $Id: pingus_resource.hxx,v 1.15 2003/10/18 12:11:30 grumbel Exp $
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

#ifndef HEADER_PINGUS_PINGUS_RESOURCE_HXX
#define HEADER_PINGUS_PINGUS_RESOURCE_HXX

#include <map>

#include <ClanLib/display.h>
#include "res_descriptor.hxx"

class CL_Font;
class CL_ResourceManager;

/** General Resource Managing class, it provides wrappers around
    CL_Surface::load(), CL_Font::load() and friends.  This class is
    needed to do a better handling of the resources. */
class PingusResource
{
private:
  static std::map<std::string, CL_ResourceManager*> resource_map;
  static std::map<ResDescriptor, CL_Surface> surface_map;
  static std::map<ResDescriptor, CL_Font*> font_map;

  static CL_Surface load_from_source (const ResDescriptor& res_desc);
  static CL_Surface load_from_cache (const ResDescriptor& res_desc);
  static CL_Surface apply_modifier (const CL_Surface&, const ResDescriptor& res_desc);

public:
  static void init();
  static void deinit();

  /** */
  static unsigned int get_mtime (const std::string& res_name,
				 const std::string& datafile);

  /** Load a surface with res_name from datafile */
  static CL_Surface load_surface(const std::string& res_name,
				 const std::string& datafile,
				 ResourceModifierNS::ResourceModifier modifier
				 = ResourceModifierNS::ROT0);

  /** Load a surface from the ResDescriptor */
  static CL_Surface load_surface(const ResDescriptor&);

  /** Load a font with res_name from datafile */
  static CL_Font* load_font(const std::string& res_name,
			    const std::string& datafile);

  /** Load a font from the ResDescriptor */
  static CL_Font* load_font(const ResDescriptor&);

  /** Cleanup all currently unused surfaces */
  static void cleanup ();

  /** */
  static CL_ResourceManager* get(const std::string&);

private:
  PingusResource (const PingusResource&);
  PingusResource& operator= (const PingusResource&);
};

#endif

/* EOF */
