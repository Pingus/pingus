//  $Id: pingus_resource.cxx,v 1.23 2003/03/03 20:32:18 grumbel Exp $
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

#ifndef WIN32
#  include <unistd.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#endif

#include <assert.h>

#include <ClanLib/png.h>
#include <ClanLib/Display/Font/font.h>
#include "system.hxx"
#include "path_manager.hxx"
#include "globals.hxx"
#include "pingus_resource.hxx"
#include "blitter.hxx"
#include "debug.hxx"
#include "plf.hxx"
#include "debug.hxx"

std::map<std::string, CL_ResourceManager*> PingusResource::resource_map;
std::map<ResDescriptor, CL_Surface>        PingusResource::surface_map;
std::map<ResDescriptor, CL_Font*>          PingusResource::font_map;
PingusResource::PLFMap PingusResource::plf_map;

void 
PingusResource::init()
{
  // do nothing
}

void
PingusResource::deinit()
{
  for (std::map<std::string, CL_ResourceManager*>::iterator i = resource_map.begin(); 
       i != resource_map.end ();
       ++i)
    delete i->second;
  
  for (std::map<ResDescriptor, CL_Font*>::iterator i = font_map.begin();
       i != font_map.end (); 
       ++i)
    delete i->second;
}

CL_ResourceManager*
PingusResource::get(const std::string& arg_filename)
{
  std::string filename = arg_filename + ".scr";

  CL_ResourceManager* res_manager;

  res_manager = resource_map[filename];

  if (res_manager)
    {
      return res_manager;
    }
  else
    {
      std::string res_filename;

      res_filename = "data/" + filename;

      // FIXME: Memory hole... 
      res_manager = new CL_ResourceManager(path_manager.complete (res_filename.c_str()),
      					   /* is_datafile = */false);
      
      resource_map[filename] = res_manager;
      return res_manager;
    }
}

CL_Surface
PingusResource::load_surface(const std::string& res_name, 
			     const std::string& datafile,
			     ResourceModifierNS::ResourceModifier modifier)
{
  return load_surface(ResDescriptor(res_name, datafile, 
				    ResDescriptor::RD_RESOURCE,
				    modifier));
}

CL_Surface
PingusResource::load_surface(const ResDescriptor& res_desc)
{

  // try to load from cache
  CL_Surface surf = load_from_cache(res_desc);

  if (!surf) // not in cache
    {
      ResDescriptor desc = res_desc;
      desc.modifier = ResourceModifierNS::ROT0;

      // Try to an unmodified version from cache
      surf = load_from_cache(desc);

      if (surf) // found unmodified version in cache
	{
	  pout(PINGUS_DEBUG_RESOURCES) << "PingusResource: Loading surface from cache 1/2: " << res_desc << std::endl;
	  surf = apply_modifier (surf, res_desc);
      
	  // add to cache (FIXME: doesn't work)
	  surface_map[res_desc] = surf;
	}
      else // never loaded, need to load it from source
	{
	  desc = res_desc;
	  desc.modifier = ResourceModifierNS::ROT0;

	  pout(PINGUS_DEBUG_RESOURCES) << "PingusResource: Loading surface from source: " << res_desc << std::endl;
	  surf = load_from_source (desc);
	  surface_map[desc] = surf; // add to cache

	  surf = apply_modifier (surf, res_desc);
	  surface_map[res_desc] = surf; // add modified version to cache
	}
    }
  else
    {
      pout(PINGUS_DEBUG_RESOURCES) << "PingusResource: Loading surface from cache: " << res_desc << std::endl;
    }

  return surf;
}

CL_Surface
PingusResource::load_from_cache (const ResDescriptor& res_desc)
{
  return surface_map[res_desc];
}

CL_Surface
PingusResource::apply_modifier (const CL_Surface& surf, const ResDescriptor& res_desc)
{
  switch (res_desc.modifier)
    {
      // FIXME: muahhhaa... I write slower code than you....
    case ResourceModifierNS::ROT0:
      return surf;
      
    case ResourceModifierNS::ROT90:
      return Blitter::rotate_90(surf);

    case ResourceModifierNS::ROT180:
      return Blitter::rotate_180(surf);

    case ResourceModifierNS::ROT270:
      return Blitter::rotate_270(surf);

    case ResourceModifierNS::ROT0FLIP:
      return Blitter::flip_horizontal(surf);

    case ResourceModifierNS::ROT90FLIP:
      return Blitter::rotate_90_flip(surf);

    case ResourceModifierNS::ROT180FLIP:
      return Blitter::rotate_180_flip(surf);

    case ResourceModifierNS::ROT270FLIP:
      return Blitter::rotate_270_flip(surf);

    default:
      perr << "PingusResource: Unhandled modifier: " << res_desc.modifier << std::endl;
      return surf;
    }
}

CL_Surface
PingusResource::load_from_source (const ResDescriptor& res_desc)
{
  switch(res_desc.type)
    {
    case ResDescriptor::RD_RESOURCE:
      try {
	return CL_Surface (res_desc.res_name.c_str(), get(res_desc.datafile));
      } catch (CL_Error err) {
	pout << "PingusResource:" << res_desc
	     <<  ":-404-:" << err.message << std::endl;
	try {
	  return CL_Surface ("misc/404", get("core"));
	} catch (CL_Error err2) {
	  pout << "PingusResource: Fatal error, important gfx files (404.pcx) couldn't be loaded!" << std::endl;
	  throw err;
	}
      }
      break;
	  
    case ResDescriptor::RD_FILE:
      {
	std::string filename = System::get_statdir() + "images/" + res_desc.res_name;
	// FIXME: Memory leak?
	pout << "PingusResource::load_surface(" << res_desc.res_name << ")" << std::endl;
	// FIXME: Add pcx, jpeg, tga support here 
	pout << "DONE" << std::endl;
	return CL_Surface(new CL_PNGProvider(filename, NULL), true);
      }
	  
    case ResDescriptor::RD_AUTO:
      perr << "PingusResource: ResDescriptor::AUTO not implemented" << std::endl;
      assert (false);

    default:
      perr << "PingusResource: Unknown ResDescriptor::type: " << res_desc.type  << std::endl;
      assert (false);
      return CL_Surface();
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
  pout(PINGUS_DEBUG_RESOURCES) << "PingusResource: Loading font: " << res_desc << std::endl;

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
				 get(res_desc.datafile));
	  } catch (CL_Error err) {
	    pout << "PingusResource: " << err.message << std::endl
	         << "PingusResource: Couldn't load font: " << res_desc << std::endl;
	    assert (!"PingusResource: Fatal error can't continue!");
	  }
	  font_map[res_desc] = font;
	  return font;
	  
	case ResDescriptor::RD_FILE:
	  pout << "PingusResource: ResDescriptor::FILE not implemented" << std::endl;
	  return 0;
	  
	case ResDescriptor::RD_AUTO:
	  pout << "PingusResource: ResDescriptor::AUTO not implemented" << std::endl;
	  return 0;

	default:
	  pout << "PingusResource: Unknown ResDescriptor::type: " << res_desc.type  << std::endl;
	  return 0;
	}
    }
}

void
PingusResource::cleanup ()
{
  pout(PINGUS_DEBUG_RESOURCES) << "PingusResource::cleanup ()" << std::endl;
  
  for (std::map<ResDescriptor, CL_Surface>::iterator i = surface_map.begin ();
       i != surface_map.end (); ++i)
    {
      if (i->first.type == ResDescriptor::RD_FILE
	  && i->second.get_reference_count () == 1)
	{
	  pout(PINGUS_DEBUG_RESOURCES) << "XXX Releasing File: " << i->first
	                               << " => " << i->second.get_reference_count () << std::endl;
	  surface_map.erase(i);
	}
      else if (i->first.type == ResDescriptor::RD_RESOURCE
	       && i->second.get_reference_count () == 2)
	{
	  pout(PINGUS_DEBUG_RESOURCES) << "XXX Releasing Resource : " << i->first
	                               << " => " << i->second.get_reference_count () << std::endl;
	  surface_map.erase(i);
	}
    }
}

unsigned int
PingusResource::get_mtime (const std::string& res_name,
			   const std::string& datafile)
{
  try 
    {
      CL_ResourceManager* res_man = PingusResource::get(datafile);
      
      if (!res_man->is_from_source ())
	{
	  // FIXME: not implemented
	  return 0;
	}
      else
	{
	  CL_Resource& res = res_man->get_resource(res_name);
  
	  std::string filename = res.get_full_location();

#ifndef WIN32
	  struct stat stat_buf;
	  if (stat(filename.c_str(), &stat_buf) == 0)
	    return stat_buf.st_mtime;
	  else
	    return 0;
#else
	  // FIXME: Win32 mtime getter not implemented
	  return 0;
#endif
	}
    } 
  catch (CL_Error& err) 
    {
      std::cout << "PingusResource::get_mtime: CL_Error: " << err.message << std::endl;
      return 0;
    }
}

PLFHandle
PingusResource::load_plf_raw(const std::string& filename)
{
  std::string res_name = System::basename(filename);
  res_name = res_name.substr(0, res_name.length()-4);
  
  pout(PINGUS_DEBUG_LOADING) << "PingusResource: Loading level: '" << res_name << "' -> '" << filename << "'" << std::endl;

  PLFMap::iterator i = plf_map.find(res_name);

  if (i == plf_map.end ())
    {
      PLF* plf = PLF::create(filename);
      plf_map[res_name]  = plf;
      return PLFHandle (plf);
    }
  else
    {
      return PLFHandle (i->second);
    }
}

PLFHandle
PingusResource::load_plf(const std::string& res_name)
{
  PLFMap::iterator i   = plf_map.find(res_name);
  std::string filename = path_manager.complete("levels/" + res_name + ".xml");

  pout(PINGUS_DEBUG_LOADING) << "PingusResource: Loading level: '" << res_name << "' -> '" << filename << "'" << std::endl;

  if (i == plf_map.end ())
    {
      PLF* plf = PLF::create(filename);
      plf_map[res_name]  = plf;
      return PLFHandle (plf);
    }
  else
    {
      return PLFHandle (i->second);
    }

}

/* EOF */
