//  $Id: thumb_cache.cxx,v 1.5 2002/06/23 11:08:30 grumbel Exp $
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
#include <iostream>
#include <ClanLib/Core/IOData/outputsource_file.h>
#include <ClanLib/Core/IOData/inputsource_file.h>
#include <ClanLib/Core/System/error.h>
#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include "../globals.hxx"
#include "../blitter.hxx"
#include "../system.hxx"
#include "../pingus_resource.hxx"
#include "../math.hxx"
#include "thumb_cache.hxx"

using namespace Pingus;

const unsigned int thumbcache_version = 2;

/*
  ~/.pingus/cache/

  Format:
  uint32: Version
  uint32: width
  uint32: height
  uint32: mtime of parent image
  data:   ...

 */

CL_Surface
ThumbCache::load (const std::string & res_ident, const std::string & datafile)
{
  std::string filename = res_ident + "-" + datafile;

  for (unsigned int i = 0; i < filename.size (); i++)
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
	      return CL_Surface ();
	    }

	  unsigned int width  = in.read_uint32 ();
	  unsigned int height = in.read_uint32 ();

	  unsigned int timestamp = in.read_uint32 ();
	  // The thumbnail needs an update
	  if (timestamp != PingusResource::get_mtime (res_ident, datafile))
	    {
	      std::cout << "Thumbnail: file needs update" << std::endl;
	      return CL_Surface ();
	    }

	  CL_Canvas* canvas = new CL_Canvas (width, height);
	  canvas->lock ();
	  void* buffer = canvas->get_data ();
	  size_t buffer_size = width * height * 4;

	  size_t read_size = in.read (buffer, buffer_size);
	  
	  if (read_size != buffer_size)
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_EDITOR)
		std::cerr << "ThumbCache: " << filename << ": read error: wanted " << buffer_size << " got " << read_size << std::endl;
	      delete canvas;
	      return CL_Surface ();
	    }
	  canvas->unlock ();
	  return CL_Surface (canvas, true);
	}
      catch (CL_Error& err)
	{
	  std::cout << "ThumbCache: Read error: " << filename << " | " << err.message <<std::endl;
	  return CL_Surface ();
	}
    }

  // FIXME: This should return the correct surface
  return CL_Surface ();
}

void 
ThumbCache::cache (const CL_Surface& sur, const std::string & res_ident, const std::string & datafile)
{
  if (sur.get_provider ()->get_height () * sur.get_provider ()->get_width () < 50 * 50)
    {
      std::cout << "ThumbCache: image too small for cache: " << res_ident << std::endl;
      return;
    }
    
  std::string filename = res_ident + "-" + datafile;

  for (unsigned int i = 0; i < filename.size (); i++)
    if (filename[i] == '/')
      filename[i] = '_';

  filename = System::get_cachedir () + filename;

  unsigned int timestamp = PingusResource::get_mtime (res_ident, datafile);

  std::cout <<"ThumbCache: Writing cache file: " << filename 
	    << " timestamp: " << timestamp
	    << std::endl;
  
  try 
    {
      CL_OutputSource_File out(filename);

      unsigned int width  = Math::min((unsigned int)50, sur.get_width ());
      unsigned int height = Math::min((unsigned int)50, sur.get_height ());

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
      delete canvas;
    }
  catch (CL_Error& err) 
    {
      std::cout << "ThumbCache: Couldn't open file for writing: " << filename << std::endl;
    }
}

/* EOF */
