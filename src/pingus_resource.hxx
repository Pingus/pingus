//  $Id: pingus_resource.hxx,v 1.3 2002/06/20 16:48:11 grumbel Exp $
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

#ifndef PINGUSRESOURCE_HH
#define PINGUSRESOURCE_HH

#include <map>

#include <ClanLib/Display/Display/surface.h>
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

public:
  /** This array contains possible modifications of a surface */
  typedef enum {
    VFLIP  = (1<<0),
    HVLIP  = (1<<1),
    
    ROT90  = (1<<2),
    ROT180 = (1<<3),
    ROT270 = (1<<4)
  } Modifier;

  ///
  PingusResource();
  
  /** */
  static unsigned int get_mtime (const std::string& res_name,
				 const std::string& datafile);

  /** Load a surface with res_name from datafile */
  static CL_Surface load_surface(const std::string& res_name,
				 const std::string& datafile);
  
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
};

#endif

/* EOF */

