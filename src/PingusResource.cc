//  $Id: PingusResource.cc,v 1.6 2000/06/25 20:22:18 grumbel Exp $
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

PingusResource::PingusResource()
{
  std::cout << "PingusResource Constructor called" << std::endl;
}
/*
void
PingusResource::open(const std::string& filename)
{
  std::string file_n;

  if (verbose > 1)
    std::cout << "PingusResource: Searching for file: " << filename << std::endl;

  file_n = find_file(pingus_datadir, "data/" + filename);

  if (verbose > 1)
    std::cout << "PingusResource: Found file: " << file_n << std::endl;

  if (filename != file_n) {
    Res res;
    res.res = CL_ResourceManager::create(file_n.c_str(), true);
    res.filename = filename;
    
    resource.push_back(res);
  } else {
    throw PingusError("PingusResource: Couldn't find resource file: " + filename);
  }
}
*/
CL_ResourceManager*
PingusResource::get(const std::string& filename)
{
  CL_ResourceManager* res_manager;

  res_manager = resource_map[filename];

  if (res_manager)
    {
      return res_manager;
    }
  else
    {
      std::string res_filename = find_file(pingus_datadir, "data/" + filename);
      res_manager = CL_ResourceManager::create(res_filename.c_str(),
					       /* is_datafile = */ true);
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
  CL_Surface* surf = surface_map[res_desc];
  
  if (surf) 
    {
      return surf;
    }
  else
    {
      switch(res_desc.type)
	{
	case ResDescriptor::RESOURCE:
	  surf = CL_Surface::load(res_desc.res_name.c_str(),
				  get(res_desc.datafile + ".dat"));
	  surface_map[res_desc] = surf;
	  return surf;
	  break;
	  
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
	  font = CL_Font::load(res_desc.res_name.c_str(),
				  get(res_desc.datafile + ".dat"));
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
