//  $Id: resource.cxx,v 1.36 2003/12/13 16:23:39 grumbel Exp $
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
#include "resource.hxx"
#include "blitter.hxx"
#include "debug.hxx"

namespace Pingus {

CL_ResourceManager Resource::resmgr;
std::map<ResDescriptor, CL_Surface>       Resource::surface_map;

std::string 
fix_file(std::string res_name, std::string datafile)
{
  if (datafile.empty())
    {
      return res_name;
    }
  else
    {
  for (std::string::size_type i = 0; i != datafile.size(); ++i)
    if (datafile[i] == '-')
      datafile[i] = '/';

  return datafile + "/" + res_name;
    }
}

void
Resource::init()
{
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/core.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/editor.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/entrances.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/exits.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/fonts.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/game.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/global.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/groundpieces-bridge.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/groundpieces-ground.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/groundpieces-remove.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/groundpieces-solid.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/groundpieces-transparent.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/hotspots.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/liquids.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/pingus.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/story.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/textures.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/traps.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/worldmaps.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/worldobjs.xml")));
  resmgr.add_resources(CL_ResourceManager(path_manager.complete("data/alias.xml")));

}

void
Resource::deinit()
{
  surface_map.clear();
}

CL_Surface
Resource::load_surface(const std::string& res_name,
                       const std::string& datafile,
                       ResourceModifierNS::ResourceModifier modifier)
{
  return load_surface(ResDescriptor(res_name, datafile,
				    modifier));
}

CL_Sprite
Resource::load_sprite(const ResDescriptor& desc)
{
  return load_sprite(desc.res_name, desc.datafile);
}

CL_Sprite
Resource::load_sprite(const std::string& res_name, 
                      const std::string& datafile)
{
  try {
    return CL_Sprite(fix_file(res_name, datafile), &resmgr);
  } catch (CL_Error& err) {
    std::cout << "Resource::load_sprite: CL_Error: '" << res_name << "', '" << datafile  << "'" << std::endl;
    std::cout << "CL_Error: " << err.message << std::endl;
    return CL_Sprite("core/misc/404sprite", &resmgr);
  }
}

CL_SpriteDescription
Resource::load_sprite_desc(const std::string& res_name,
                           const std::string& datafile)
{
  try {
    return CL_SpriteDescription(fix_file(res_name, datafile), &resmgr);
  } catch(CL_Error& err) {
    std::cout << "Resource::load_sprite_desc: CL_Error: '" << res_name << "', '" << datafile  << "'" << std::endl;
    std::cout << "CL_Error: " << err.message << std::endl;
    return CL_SpriteDescription("core/misc/404sprite", &resmgr);
  }
}

CL_PixelBuffer
Resource::load_pixelbuffer(const ResDescriptor& desc_)
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
Resource::load_pixelbuffer(const std::string& res_name,
                           const std::string& datafile)
{
  return load_pixelbuffer(ResDescriptor(res_name, datafile));
}

CL_Surface
Resource::load_surface(const ResDescriptor& res_desc)
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
	  pout(PINGUS_DEBUG_RESOURCES) << "Resource: Loading surface from cache 1/2: " << res_desc << std::endl;
	  surf = apply_modifier (surf, res_desc);

	  surface_map[res_desc] = surf;
	}
      else // never loaded, need to load it from source
	{
	  desc = res_desc;
	  desc.modifier = ResourceModifierNS::ROT0;

	  pout(PINGUS_DEBUG_RESOURCES) << "Resource: Loading surface from source: " << res_desc << std::endl;
	  surf = load_from_source (desc);
	  surface_map[desc] = surf; // add to cache

	  surf = apply_modifier (surf, res_desc);
	  surface_map[res_desc] = surf; // add modified version to cache
	}
    }
  else
    {
      pout(PINGUS_DEBUG_RESOURCES) << "Resource: Loading surface from cache: " << res_desc << std::endl;
    }

  return surf;
}

CL_Surface
Resource::load_from_cache (const ResDescriptor& res_desc)
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
Resource::apply_modifier_to_pixelbuffer(CL_PixelBuffer prov, const ResDescriptor& res_desc)
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
      perr << "Resource: Unhandled modifier: " << res_desc.modifier << std::endl;
      return prov;
    }
}

CL_Surface
Resource::apply_modifier (const CL_Surface& surf, const ResDescriptor& res_desc)
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
      perr << "Resource: Unhandled modifier: " << res_desc.modifier << std::endl;
      return CL_Surface(new CL_PixelBuffer(prov), true);
    }
}

CL_Surface
Resource::load_from_source (const ResDescriptor& res_desc)
{
  try {
    return CL_Surface(fix_file(res_desc.res_name, res_desc.datafile), &resmgr);
  } catch (CL_Error err) {
    pout << "CL_Error: " << err.message << std::endl;
    pout << "Resource:" << res_desc
         <<  ":-404-:" << err.message << std::endl;
    try {
      return CL_Surface ("core/misc/404", &resmgr);
    } catch (CL_Error err2) {
      pout << "Resource: Fatal error, important gfx files (404.pcx) couldn't be loaded!" << std::endl;
      throw err;
    }
  }
}

CL_Font
Resource::load_font(const std::string& res_name)
{
  return CL_Font(res_name, &resmgr);
}

void
Resource::cleanup ()
{
#ifdef CLANLIB_0_6
  pout(PINGUS_DEBUG_RESOURCES) << "Resource::cleanup ()" << std::endl;

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
Resource::get_mtime (const std::string& res_name,
                     const std::string& datafile)
{
#ifdef CLANLIB_0_6
  try
    {
      CL_ResourceManager res_man = Resource::get(datafile);

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
      std::cout << "Resource::get_mtime: CL_Error: " << err.message << std::endl;
      return 0;
    }
#endif
  return 0;
}

} // namespace Pingus

/* EOF */
