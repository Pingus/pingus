//  $Id: ThumbCache.cc,v 1.4 2001/05/18 19:17:08 grumbel Exp $
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

#include "../globals.hh"
#include "../blitter.hh"
#include "../System.hh"
#include "ThumbCache.hh"

CL_Surface
ThumbCache::load (std::string res_ident, std::string datafile)
{
  std::string filename = res_ident + "-" + datafile;

  for (unsigned int i = 0; i < filename.size (); i++)
    if (filename[i] == '/')
      filename[i] = '_';

  filename = System::get_cachedir () + filename;

  //  std::cout <<" Loading cache file: " << filename << std::endl;

  if (System::exist (filename))
    {
      FILE* in = fopen (filename.c_str (), "r");
      
      if (in)
	{
	  CL_Canvas* canvas = new CL_Canvas (50, 50);
	  canvas->lock ();
	  unsigned char* buffer = static_cast<unsigned char*>(canvas->get_data ());
	  size_t buffer_size = 50 * 50 * 4;
	  size_t read_size = fread (buffer, sizeof (unsigned char*), buffer_size, in);
	  if (read_size != buffer_size)
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_EDITOR)
		std::cerr << "ThumbCache: " << filename << ": read error: wanted " << buffer_size << " got " << read_size << std::endl;
	    }
	  fclose (in);
	  canvas->unlock ();
	  return CL_Surface (canvas, true);
	}
      else
	{
	  std::cout << "ThumbCache: Read error: " << filename << std::endl;
	}
    }

  return CL_Surface ();
}

void 
ThumbCache::cache (const CL_Surface& sur, std::string res_ident, std::string datafile)
{
  if (sur.get_provider ()->get_pitch () * sur.get_provider ()->get_width () < 50 * 50 * 4)
    {
      std::cout << "ThumbCache: image too small for cache: " << res_ident << std::endl;
      return;
    }
    
  FILE* out;
  std::string filename = res_ident + "-" + datafile;

  for (unsigned int i = 0; i < filename.size (); i++)
    if (filename[i] == '/')
      filename[i] = '_';

  filename = System::get_cachedir () + filename;

  CL_Canvas* canvas = Blitter::scale_surface_to_canvas (sur, 50, 50);
  std::cout <<" Writing cache file: " << filename << std::endl;
  
  out = fopen (filename.c_str (), "w");
  
  if (out)
    {
      canvas->lock ();
      unsigned char* buffer = static_cast<unsigned char*>(canvas->get_data());
      int buffer_size = canvas->get_height () * canvas->get_pitch ();

      fwrite (buffer, sizeof (unsigned char), buffer_size, out);

      fclose (out);
      canvas->unlock ();
    }
  else
    {
      std::cout << "ThumbCache: Couldn't open file for writing: " << filename << std::endl;
    }
}

/* EOF */
