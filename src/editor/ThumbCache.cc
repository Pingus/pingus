//  $Id: ThumbCache.cc,v 1.1 2000/10/14 16:09:46 grumbel Exp $
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

#include "../blitter.hh"
#include "../System.hh"
#include "ThumbCache.hh"

CL_Surface* 
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
	  int buffer_size = 50 * 50 * 4;
	  int read_size = fread (buffer, sizeof (unsigned char*), buffer_size, in);
	  fclose (in);
	  canvas->unlock ();
	  return CL_Surface::create (canvas, true);
	}
      else
	{
	  std::cout << "ThumbCache: Read error: " << filename << std::endl;
	}
    }

  return 0;
}

void 
ThumbCache::cache (CL_Surface* sur, std::string res_ident, std::string datafile)
{
  if (sur->get_provider ()->get_pitch () * sur->get_provider ()->get_width () < 50 * 50 * 4)
    {
      std::cout << "ThumbCache: image to small for cache: " << res_ident << std::endl;
      return;
    }
    
  FILE* out;
  std::string filename = res_ident + "-" + datafile;

  for (unsigned int i = 0; i < filename.size (); i++)
    if (filename[i] == '/')
      filename[i] = '_';

  filename = System::get_cachedir () + filename;

  CL_Canvas* canvas = Blitter::scale_surface_to_canvas (sur, 50, 50);
  std::cout <<" Writting cache file: " << filename << std::endl;
  
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
      std::cout << "ThumbCache: Couldn't open file for writting: " << filename << std::endl;
    }
}

/* EOF */
