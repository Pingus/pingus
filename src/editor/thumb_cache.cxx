//  $Id: thumb_cache.cxx,v 1.13 2003/04/19 10:23:18 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <stdio.h>
#include <ClanLib/Core/IOData/outputsource_file.h>
#include <ClanLib/Core/IOData/inputsource_file.h>
#include <ClanLib/Core/System/error.h>
#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include "../globals.hxx"
#include "../blitter.hxx"
#include "../system.hxx"
#include "../debug.hxx"
#include "../pingus_resource.hxx"
#include "../math.hxx"
#include "thumb_cache.hxx"

namespace EditorNS {

const unsigned int thumbcache_version = 3;

/*
  ~/.pingus/cache/

  Format:
  uint32: Version
  uint32: width
  uint32: height
  uint32: mtime of parent image
  data:   (RGBA8888)...

 */

CL_Surface
ThumbCache::uncached_load (const std::string & res_ident, const std::string & datafile)
{
  CL_Surface sur = PingusResource::load_surface (res_ident, datafile);

  if (maintainer_mode)
    pout << "ThumbCache: Loading: " << res_ident << " (" << datafile << ")"  << std::endl;

  // Add object to cache
  return ThumbCache::cache (sur, res_ident, datafile);
}

CL_Surface
ThumbCache::load (const std::string & res_ident, const std::string & datafile)
{
  std::string filename = res_ident + "-" + datafile;

  for (unsigned int i = 0; i < filename.size (); ++i)
    if (filename[i] == '/')
      filename[i] = '_';

  filename = System::get_cachedir () + filename;

  //  std::cout <<" Loading cache file: " << filename << std::endl;

  if (System::exist (filename))
    {
      //FILE* in = fopen (filename.c_str (), "r");
      try
	{
	  CL_InputSource_File in(filename);

	  unsigned int version   = in.read_uint32 ();
	  if (version != thumbcache_version)
	    {
	      std::cout << "Thumbnail: version mismatch" << std::endl;
	      return uncached_load (res_ident, datafile);
	    }

	  unsigned int width  = in.read_uint32 ();
	  unsigned int height = in.read_uint32 ();

	  unsigned int timestamp = in.read_uint32 ();
	  // The thumbnail needs an update
	  if (timestamp != PingusResource::get_mtime (res_ident, datafile))
	    {
	      std::cout << "Thumbnail: file needs update" << std::endl;
	      return uncached_load (res_ident, datafile);
	    }

	  CL_Canvas* canvas = new CL_Canvas (width, height);
	  canvas->lock ();
	  void* buffer = canvas->get_data ();
	  size_t buffer_size = width * height * 4;

	  size_t read_size = in.read (buffer, buffer_size);

	  if (read_size != buffer_size)
	    {
	      perr(PINGUS_DEBUG_EDITOR) << "ThumbCache: " << filename << ": read error: wanted "
	                                << buffer_size << " got " << read_size << std::endl;
	      delete canvas;
	      return uncached_load (res_ident, datafile);
	    }
	  canvas->unlock ();
	  return CL_Surface (canvas, true);
	}
      catch (CL_Error& err)
	{
	  std::cout << "ThumbCache: Read error: " << filename << " | " << err.message <<std::endl;
	  return uncached_load (res_ident, datafile);
	}
    }

  return uncached_load (res_ident, datafile);
}

CL_Surface
ThumbCache::cache (const CL_Surface& sur, const std::string & res_ident, const std::string & datafile)
{
  if (sur.get_provider ()->get_height () < 50
      && sur.get_provider ()->get_width () < 50)
    {
      // If the image is smaller than the thumbnail, there is no need to cache it
      if (maintainer_mode)
        pout << "ThumbCache: image too small for cache: " << res_ident << std::endl;

      return sur;
    }

  std::string filename = res_ident + "-" + datafile;

  for (unsigned int i = 0; i < filename.size (); ++i)
    if (filename[i] == '/')
      filename[i] = '_';

  filename = System::get_cachedir () + filename;

  unsigned int timestamp = PingusResource::get_mtime (res_ident, datafile);

  if (maintainer_mode)
    pout << "ThumbCache: Writing cache file: " << filename
         << " timestamp: " << timestamp << std::endl;

  try
    {
      CL_OutputSource_File out(filename);

      unsigned int width  = Math::min((unsigned int)50, sur.get_width ());
      unsigned int height = Math::min((unsigned int)50, sur.get_height ());

      // Caller is responsible to delete the canvas
      CL_Canvas* canvas = Blitter::scale_surface_to_canvas (sur, width, height);
      canvas->lock ();
      void* buffer = canvas->get_data();
      int buffer_size = canvas->get_height () * canvas->get_pitch ();

      // Versionnumber of the thumbnail format
      out.write_uint32 (thumbcache_version);

      out.write_uint32 (width);
      out.write_uint32 (height);

      // Modification time  of the parent file
      out.write_uint32 (timestamp);

      // Surface data
      // FIXME: Endian issue here?!
      out.write (buffer, buffer_size);

      canvas->unlock ();
      // Canvas will get deleted on the end of the lifetime of this surface
      return CL_Surface (canvas, true);
    }
  catch (CL_Error&)
    {
      perr << "ThumbCache: Couldn't open file for writing: " << filename << std::endl;

      // If writing the surface fails, we return the surface without
      // writing it to the cache
      unsigned int width  = Math::min((unsigned int)50, sur.get_width ());
      unsigned int height = Math::min((unsigned int)50, sur.get_height ());
      return Blitter::scale_surface (sur, width, height);
    }
}

} // namespace EditorNS

/* EOF */
