//  $Id: pingus_resource.cxx,v 1.36 2003/12/13 16:23:39 grumbel Exp $
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

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/Display/font.h>
#include "system.hxx"
#include "path_manager.hxx"
#include "globals.hxx"
#include "pingus_resource.hxx"
#include "blitter.hxx"
#include "debug.hxx"

namespace Pingus {

std::map<std::string, CL_ResourceManager> PingusResource::resource_map;
std::map<ResDescriptor, CL_Surface>        PingusResource::surface_map;
std::map<ResDescriptor, CL_Font>          PingusResource::font_map;

void
PingusResource::init()
{
  // do nothing
}

void
PingusResource::deinit()
{
  resource_map.clear();
  surface_map.clear();
  font_map.clear();
}

CL_ResourceManager
PingusResource::get(const std::string& arg_filename)
{
  std::string filename = arg_filename + ".xml";

  std::map<std::string, CL_ResourceManager>::iterator i = resource_map.find(filename);

  if (i != resource_map.end())
    {
      return i->second;
    }
  else
    {
      std::string res_filename;

      res_filename = "data/" + filename;

      CL_ResourceManager res_manager = CL_ResourceManager(path_manager.complete(res_filename));
      
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

CL_Sprite
PingusResource::load_sprite(const ResDescriptor& desc)
{
  return load_sprite(desc.res_name, desc.datafile);
}

CL_Sprite
PingusResource::load_sprite(const std::string& res_name, 
                            const std::string& datafile)
{
  CL_ResourceManager res = get(datafile);
  try {
    CL_Sprite sprite(res_name, &res);
    return sprite;
  } catch (CL_Error& err) {
      std::cout << "PingusResource::load_sprite: CL_Error: '" << res_name << "', '" << datafile  << "'" << std::endl;
      std::cout << "CL_Error: " << err.message << std::endl;
    CL_ResourceManager res_mgr = get("core");
    return CL_Sprite("misc/404sprite", &res_mgr);
  }
}

CL_SpriteDescription
PingusResource::load_sprite_desc(const std::string& res_name,
                                 const std::string& datafile)
{
  CL_ResourceManager res = get(datafile);
  try {
    CL_SpriteDescription desc(res_name, &res);
    return desc;
  } catch(CL_Error& err) {
    std::cout << "PingusResource::load_sprite_desc: CL_Error: '" << res_name << "', '" << datafile  << "'" << std::endl;
    std::cout << "CL_Error: " << err.message << std::endl;
    res = get("core");
    return CL_SpriteDescription("misc/404sprite", &res);
  }
}

CL_PixelBuffer
PingusResource::load_pixelbuffer(const ResDescriptor& desc_)
{
  CL_SpriteDescription desc = load_sprite_desc(desc_.res_name, desc_.datafile);

  if (desc.get_frames().size() == 0)
    {
      std::cout << "Error: load_pixelbuffer: " << desc_.res_name << " " << desc_.datafile << std::endl;
      assert(0);
    }

  return apply_modifier_to_pixelbuffer(*desc.get_frames().begin()->first, desc_);
}

CL_PixelBuffer
PingusResource::load_pixelbuffer(const std::string& res_name,
                                 const std::string& datafile)
{
  return load_pixelbuffer(ResDescriptor(res_name, datafile));
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
  std::map<ResDescriptor, CL_Surface>::iterator i = surface_map.find(res_desc);
  if (i == surface_map.end())
    {
      return CL_Surface();
    }
  else
    {
      return i->second;
    }
}

CL_PixelBuffer
PingusResource::apply_modifier_to_pixelbuffer(CL_PixelBuffer prov, const ResDescriptor& res_desc)
{
  switch (res_desc.modifier)
    {
    case ResourceModifierNS::ROT0:
      return prov;

    case ResourceModifierNS::ROT90:
      return Blitter::rotate_90(prov);

    case ResourceModifierNS::ROT180:
      return Blitter::rotate_180(prov);

    case ResourceModifierNS::ROT270:
      return Blitter::rotate_270(prov);

    case ResourceModifierNS::ROT0FLIP:
      return Blitter::flip_horizontal(prov);

    case ResourceModifierNS::ROT90FLIP:
      return Blitter::rotate_90_flip(prov);

    case ResourceModifierNS::ROT180FLIP:
      return Blitter::rotate_180_flip(prov);

    case ResourceModifierNS::ROT270FLIP:
      return Blitter::rotate_270_flip(prov);

    default:
      perr << "PingusResource: Unhandled modifier: " << res_desc.modifier << std::endl;
      return prov;
    }
}

CL_Surface
PingusResource::apply_modifier (const CL_Surface& surf, const ResDescriptor& res_desc)
{
  CL_PixelBuffer prov = surf.get_pixeldata();

  switch (res_desc.modifier)
    {
    case ResourceModifierNS::ROT0:
      return CL_Surface(new CL_PixelBuffer(prov), true);

    case ResourceModifierNS::ROT90:
      return CL_Surface(new CL_PixelBuffer(Blitter::rotate_90(prov)), true);

    case ResourceModifierNS::ROT180:
      return CL_Surface(new CL_PixelBuffer(Blitter::rotate_180(prov)), true);
                        
    case ResourceModifierNS::ROT270:
      return CL_Surface(new CL_PixelBuffer(Blitter::rotate_270(prov)), true);

    case ResourceModifierNS::ROT0FLIP:
      return CL_Surface(new CL_PixelBuffer(Blitter::flip_horizontal(prov)), true);

    case ResourceModifierNS::ROT90FLIP:
      return CL_Surface(new CL_PixelBuffer(Blitter::rotate_90_flip(prov)), true);

    case ResourceModifierNS::ROT180FLIP:
      return CL_Surface(new CL_PixelBuffer(Blitter::rotate_180_flip(prov)), true);

    case ResourceModifierNS::ROT270FLIP:
      return CL_Surface(new CL_PixelBuffer(Blitter::rotate_270_flip(prov)), true);

    default:
      perr << "PingusResource: Unhandled modifier: " << res_desc.modifier << std::endl;
      return CL_Surface(new CL_PixelBuffer(prov), true);
    }
}

CL_Surface
PingusResource::load_from_source (const ResDescriptor& res_desc)
{
  switch(res_desc.type)
    {
    case ResDescriptor::RD_RESOURCE:
      try {
        CL_ResourceManager res_mgr = get(res_desc.datafile);
	return CL_Surface(res_desc.res_name.c_str(), &res_mgr);
      } catch (CL_Error err) {
        pout << "CL_Error: " << err.message << std::endl;
	pout << "PingusResource:" << res_desc
	     <<  ":-404-:" << err.message << std::endl;
	try {
          CL_ResourceManager res_mgr = get("core");
	  return CL_Surface ("misc/404", &res_mgr);
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

CL_Font
PingusResource::load_font(const std::string& res_name,
			  const std::string& datafile)
{
  return load_font(ResDescriptor(res_name, datafile,
				 ResDescriptor::RD_RESOURCE));
}

CL_Font
PingusResource::load_font(const ResDescriptor& res_desc)
{
  pout(PINGUS_DEBUG_RESOURCES) << "PingusResource: Loading font: " << res_desc << std::endl;

  CL_Font font = font_map[res_desc];

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
            CL_ResourceManager res_mgr = get(res_desc.datafile);
            font = CL_Font(res_desc.res_name, &res_mgr);
	  } catch (CL_Error err) {
            std::list<std::string> lst = get(res_desc.datafile).get_all_resources();
            for(std::list<std::string>::iterator i = lst.begin(); i != lst.end(); ++i)
              {
                std::cout << "Res: " << *i << std::endl;
              }
            
            std::cout << "DatafileRes: " << res_desc.datafile << std::endl;
	    pout << "PingusResource: CL_Error: " << err.message << std::endl
	         << "PingusResource: Couldn't load font: " << res_desc << std::endl;
            assert (!"PingusResource: Fatal error can't continue!");
	  }
	  font_map[res_desc] = font;
	  return font;

	case ResDescriptor::RD_FILE:
	  pout << "PingusResource: ResDescriptor::FILE not implemented" << std::endl;
	  return CL_Font();

	case ResDescriptor::RD_AUTO:
	  pout << "PingusResource: ResDescriptor::AUTO not implemented" << std::endl;
	  return CL_Font();

	default:
	  pout << "PingusResource: Unknown ResDescriptor::type: " << res_desc.type  << std::endl;
	  return CL_Font();
	}
    }
}

void
PingusResource::cleanup ()
{
#ifdef CLANLIB_0_6
  pout(PINGUS_DEBUG_RESOURCES) << "PingusResource::cleanup ()" << std::endl;

  for (std::map<ResDescriptor, CL_Surface>::iterator i = surface_map.begin ();
       i != surface_map.end (); ++i)
    {
	  pout(PINGUS_DEBUG_RESOURCES) << "XXXX Lookat Resource : " << i->first
	                               << " => " << i->second.get_reference_count () << std::endl;
      if (i->first.type == ResDescriptor::RD_FILE
	  && i->second.get_reference_count () == 1)
	{
          // FIXME:
	  //pout(PINGUS_DEBUG_RESOURCES) << "XXX Releasing File: " << i->first
          //                             << " => " << i->second.get_reference_count () << std::endl;
	  //surface_map.erase(i);
	}
      else if (i->first.type == ResDescriptor::RD_RESOURCE
	       && i->second.get_reference_count () == 2)
	{
	  pout(PINGUS_DEBUG_RESOURCES) << "XXX Releasing Resource : " << i->first
	                               << " => " << i->second.get_reference_count () << std::endl;
	  surface_map.erase(i);
	}
    }
#endif
}

unsigned int
PingusResource::get_mtime (const std::string& res_name,
			   const std::string& datafile)
{
#ifdef CLANLIB_0_6
  try
    {
      CL_ResourceManager res_man = PingusResource::get(datafile);

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
  catch (CL_Error& err)
    {
      std::cout << "PingusResource::get_mtime: CL_Error: " << err.message << std::endl;
      return 0;
    }
#endif
  return 0;
}

} // namespace Pingus

/* EOF */
