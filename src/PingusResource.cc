//  $Id: PingusResource.cc,v 1.13 2000/10/30 16:17:50 grumbel Exp $
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

#include <iostream>

#include "PingusError.hh"
#include "globals.hh"
#include "algo.hh"
#include "PingusResource.hh"

std::map<std::string, CL_ResourceManager*> PingusResource::resource_map;
std::map<ResDescriptor, CL_Surface*>       PingusResource::surface_map;
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

  string filename = suffix_fixer(arg_filename);
  //std::cout << "PingusResource: getting: " << filename << std::endl;

  CL_ResourceManager* res_manager;

  res_manager = resource_map[filename];

  if (res_manager)
    {
      return res_manager;
    }
  else
    {
      std::string res_filename = "data/" + filename;

      res_manager = CL_ResourceManager::create(res_filename.c_str(),
					       /* is_datafile = */ use_datafile);
      resource_map[filename] = res_manager;
      return res_manager;
    }
}

CL_Surface*
PingusResource::load_surface(const std::string& res_name, 
			     const std::string& datafile)
{
  return load_surface(ResDescriptor(res_name, datafile, 
				    ResDescriptor::RESOURCE));
}

CL_Surface*
PingusResource::load_surface(const ResDescriptor& res_desc)
{
  if (pingus_debug_flags & PINGUS_DEBUG_LOADING)
    std::cout << "PingusResource: Loading: " << res_desc << std::endl;

  CL_Surface* surf = surface_map[res_desc];
  
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
	case ResDescriptor::RESOURCE:
	  try {
	    surf = CL_Surface::load(res_desc.res_name.c_str(),
				    get(suffix_fixer(res_desc.datafile)));
	  } catch (CL_Error err) {
	    std::cout << "PingusResource: -404- CL_Error: " << err.message << std::endl;
	    try {
	      surf = CL_Surface::load("misc/404", get(suffix_fixer("core")));
	    } catch (CL_Error err2) {
	      std::cout << "PingusResource: Fatal error, important gfx files (404.pcx) couldn't be loaded!" << std::endl;
	      throw err;
	    }
	  }
	  surface_map[res_desc] = surf;
	  return surf;
	  
	case ResDescriptor::FILE:
	  std::cout << "PingusResource: ResDescriptor::FILE not implemented" << std::endl;
	  surf = CL_PCXProvider::create(find_file("", res_desc.res_name), 0);
	  surface_map[res_desc] = surf;	  
	  return surf;
	  
	case ResDescriptor::AUTO:
	  std::cout << "PingusResource: ResDescriptor::AUTO not implemented" << std::endl;
	  return 0;

	default:
	  std::cout << "PingusResource: Unknown ResDescriptor::type: " << res_desc.type  << std::endl;
	  return 0;
	}
    }
}

CL_Font* 
PingusResource::load_font(const std::string& res_name,
			  const std::string& datafile)
{
  return load_font(ResDescriptor(res_name, datafile, 
				 ResDescriptor::RESOURCE));
}

CL_Font* 
PingusResource::load_font(const ResDescriptor& res_desc)
{
  CL_Font* font = font_map[res_desc];
  
  if (font) 
    {
      return font;
    }
  else
    {
      switch(res_desc.type)
	{
	case ResDescriptor::RESOURCE:
	  try {
	  font = CL_Font::load(res_desc.res_name.c_str(),
			       get(suffix_fixer(res_desc.datafile)));
	  } catch (CL_Error err) {
	    std::cout << "PingusResource: CL_Error: Couldn't load font: " << res_desc << std::endl;
	    assert (!"PingusResource: Fatal error can't continue!");
	  }
	  font_map[res_desc] = font;
	  return font;
	  
	case ResDescriptor::FILE:
	  std::cout << "PingusResource: ResDescriptor::FILE not implemented" << std::endl;
	  return 0;
	  
	case ResDescriptor::AUTO:
	  std::cout << "PingusResource: ResDescriptor::AUTO not implemented" << std::endl;
	  return 0;

	default:
	  std::cout << "PingusResource: Unknown ResDescriptor::type: " << res_desc.type  << std::endl;
	  return 0;
	}
    }
}

/* EOF */
