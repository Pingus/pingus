//  $Id: PingusResource.cc,v 1.23 2001/12/04 12:18:50 grumbel Exp $
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

#include <assert.h>
#include <iostream>
#include <ClanLib/png.h>

#include "PathManager.hh"
#include "PingusError.hh"
#include "globals.hh"
#include "algo.hh"
#include "PingusResource.hh"

std::map<std::string, CL_ResourceManager*> PingusResource::resource_map;
std::map<ResDescriptor, CL_Surface>       PingusResource::surface_map;
std::map<ResDescriptor, CL_Font*>          PingusResource::font_map;


std::string
suffix_fixer(const std::string& filename)
{
  std::string wrong_suffix;
  std::string right_suffix;

  // Using compiled datafiles, they load faster, but are larger
  if (use_datafile)
    {
      right_suffix = ".dat";
      wrong_suffix = ".scr";
    }
  else
    {
      right_suffix = ".scr";
      wrong_suffix = ".dat";
    }

  // Filename ends with ".dat", replace it with ".scr" and return.
  if (filename.substr(filename.size() - 4, std::string::npos) == wrong_suffix)
    {
      //std::cout << "PingusResource: Filename with \"" << wrong_suffix << "\" found: " << filename << std::endl;
      return filename.substr(0, filename.size() - 4) + right_suffix;
    }

  // Filename does not end with ".scr", than add it and return
  if (filename.substr(filename.size() - 4, std::string::npos) != right_suffix)
    {
      //std::cout << "PingusResource: filename doesn't contain \"" << right_suffix << "\", fixing: " << filename << std::endl;
      return (filename + right_suffix);
    }

  // Everything should be all right, just return.
  return filename;
}

PingusResource::PingusResource()
{
  //std::cout << "PingusResource Constructor called" << std::endl;
}

CL_ResourceManager*
PingusResource::get(const std::string& arg_filename)
{
  std::string filename = suffix_fixer(arg_filename);
  //std::cout << "PingusResource: getting: " << filename << std::endl;

  CL_ResourceManager* res_manager;

  res_manager = resource_map[filename];

  if (res_manager)
    {
      return res_manager;
    }
  else
    {
      std::string res_filename;

      if (broken_clanlib_resource_handling)
	res_filename = filename;
      else
	res_filename = "data/" + filename;

      // FIXME: Memory hole... 
      res_manager = new CL_ResourceManager(path_manager.complete (res_filename.c_str()),
      					   /* is_datafile = */ use_datafile);
      
      resource_map[filename] = res_manager;
      return res_manager;
    }
}

CL_Surface
PingusResource::load_surface(const std::string& res_name, 
			     const std::string& datafile)
{
  return load_surface(ResDescriptor(res_name, datafile, 
				    ResDescriptor::RD_RESOURCE));
}

CL_Surface
PingusResource::load_surface(const ResDescriptor& res_desc)
{
  if (pingus_debug_flags & PINGUS_DEBUG_LOADING)
    std::cout << "PingusResource: Loading surface: " << res_desc << std::endl;

  CL_Surface surf(surface_map[res_desc]);
  
  if (surf) 
    {
      return surf;
    }
  else
    {
      //std::cout << "PingusResource: Loading resource: " << res_desc.type << ":" 
      //<< res_desc.datafile << " - " << res_desc.res_name << std::endl;

      switch(res_desc.type)
	{
	case ResDescriptor::RD_RESOURCE:
	  try {
	    surf = CL_Surface (res_desc.res_name.c_str(), get(suffix_fixer(res_desc.datafile)));
	    //	    CL_Surface::load(res_desc.res_name.c_str(),
	    //  get(suffix_fixer(res_desc.datafile)));
	  } catch (CL_Error err) {
	    std::cout << "PingusResource:" << res_desc
		      <<  ":-404-:" << err.message << std::endl;
	    try {
	      surf = CL_Surface ("misc/404", get(suffix_fixer("core")));
	    } catch (CL_Error err2) {
	      std::cout << "PingusResource: Fatal error, important gfx files (404.pcx) couldn't be loaded!" << std::endl;
	      throw err;
	    }
	  }
	  surface_map[res_desc] = surf;
	  return surf;
	  
	case ResDescriptor::RD_FILE:
	  // FIXME: Memory leak?
	  std::cout << "PingusResource::load_surface(" << res_desc.res_name << ")" << std::endl;
	  surf = CL_Surface(new CL_PNGProvider(res_desc.res_name, NULL), false);
	  std::cout << "DONE" << std::endl;
	  surface_map[res_desc] = surf;
	  return surf;
	  
	case ResDescriptor::RD_AUTO:
	  std::cerr << "PingusResource: ResDescriptor::AUTO not implemented" << std::endl;
	  assert (false);

	default:
	  std::cerr << "PingusResource: Unknown ResDescriptor::type: " << res_desc.type  << std::endl;
	  assert (false);
	  return CL_Surface();
	}
    }
}

CL_Font* 
PingusResource::load_font(const std::string& res_name,
			  const std::string& datafile)
{
  return load_font(ResDescriptor(res_name, datafile, 
				 ResDescriptor::RD_RESOURCE));
}

CL_Font* 
PingusResource::load_font(const ResDescriptor& res_desc)
{
  if (pingus_debug_flags & PINGUS_DEBUG_LOADING)
    std::cout << "PingusResource: Loading font: " << res_desc << std::endl;

  CL_Font* font = font_map[res_desc];
  
  if (font) 
    {
      return font;
    }
  else
    {
      switch(res_desc.type)
	{
	case ResDescriptor::RD_RESOURCE:
	  try {
	  font = CL_Font::load(res_desc.res_name.c_str(),
			       get(suffix_fixer(res_desc.datafile)));
	  } catch (CL_Error err) {
	    std::cout << "PingusResource: " << err.message << std::endl;
	    std::cout << "PingusResource: Couldn't load font: " << res_desc << std::endl;
	    assert (!"PingusResource: Fatal error can't continue!");
	  }
	  font_map[res_desc] = font;
	  return font;
	  
	case ResDescriptor::RD_FILE:
	  std::cout << "PingusResource: ResDescriptor::FILE not implemented" << std::endl;
	  return 0;
	  
	case ResDescriptor::RD_AUTO:
	  std::cout << "PingusResource: ResDescriptor::AUTO not implemented" << std::endl;
	  return 0;

	default:
	  std::cout << "PingusResource: Unknown ResDescriptor::type: " << res_desc.type  << std::endl;
	  return 0;
	}
    }
}

/* EOF */
