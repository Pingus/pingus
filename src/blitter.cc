//  $Id: blitter.cc,v 1.3 2000/02/11 16:58:26 grumbel Exp $
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

#include <cstdio>
#include "PingusError.hh"
#include "blitter.hh"


void
put_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
	    int x, int y)
{
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;
  
  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  CL_Palette* palette;
  int x_offset, y_offset;

  if (sprovider->get_depth() != 8)
    {
      char str[1024];
      sprintf(str, "Image has wrong color depth: %d", sprovider->get_depth());
      throw PingusError(str);
    }
  //  assert(provider->get_pixel_format() == RGBA8888);

  provider->lock();
  sprovider->lock();

  tbuffer = static_cast<unsigned char*>(provider->get_data());
  sbuffer = static_cast<unsigned char*>(sprovider->get_data());
  
  palette = sprovider->get_palette();

  if (!palette)
    {
      char str[1024];
      sprintf(str, "Couldn't find palette: %d", sprovider->get_depth());
      throw PingusError(str);
    }

  twidth = provider->get_width();
  theight = provider->get_height();
  tpitch = provider->get_pitch();

  swidth = sprovider->get_width();
  sheight = sprovider->get_height();
  spitch = sprovider->get_pitch();

  if (y < 0)
    y_offset = 0-y;
  else 
    y_offset = 0;
  
  if (x < 0) 
    x_offset = -x;
  else
    x_offset = 0;

  for(int line=y_offset; line < sheight && (line + y) < theight; ++line) 
    {
      start_i = ((line + y) * tpitch) + (x*4);
      
      for(int i=start_i+(4*x_offset),j=line*spitch+x_offset; 
	  i < start_i + (4*swidth) && (i-start_i+(x*4)) < (4*twidth); i+=4,++j) {
	// FIXME: What is this in Clanlib 0.3.0
	if (sbuffer[j] != sprovider->get_src_colorkey()) 
	  {
	    tbuffer[i + 0] = 255;                                  // alpha
	    tbuffer[i + 1] = palette->palette[sbuffer[j] * 3 + 2]; // blue
	    tbuffer[i + 2] = palette->palette[sbuffer[j] * 3 + 1]; // green
	    tbuffer[i + 3] = palette->palette[sbuffer[j] * 3 + 0]; // red
	  }
      }
    }

  sprovider->unlock();
  provider->unlock();  
}


void
put_alpha_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
	    int x, int y)
{
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;
  
  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  CL_Palette* palette;
  int x_offset, y_offset;

  //  assert(sprovider->get_depth() == 8);
  if (sprovider->get_depth() != 8)
    {
      char str[1024];
      sprintf(str, "Image has wrong color depth: %d", sprovider->get_depth());
      throw PingusError(str);
    }
  //  assert(provider->get_pixel_format() == RGBA8888);

  provider->lock();
  sprovider->lock();

  tbuffer = static_cast<unsigned char*>(provider->get_data());
  sbuffer = static_cast<unsigned char*>(sprovider->get_data());
  
  palette = sprovider->get_palette();
  assert(palette);

  twidth = provider->get_width();
  theight = provider->get_height();
  tpitch = provider->get_pitch();

  swidth = sprovider->get_width();
  sheight = sprovider->get_height();
  spitch = sprovider->get_pitch();

  if (y < 0)
    y_offset = 0-y;
  else 
    y_offset = 0;
  
  if (x < 0) 
    x_offset = -x;
  else
    x_offset = 0;

  for(int line=y_offset; line < sheight && (line + y) < theight; ++line) {
    start_i = ((line + y) * tpitch) + (x*4);

    for(int i=start_i+(4*x_offset),j=line*spitch+x_offset; 
	i < start_i + (4*swidth) && (i-start_i+(x*4)) < (4*twidth); i+=4,++j) {
      if (sbuffer[j]) {
	tbuffer[i + 0] = 0;                                  // alpha
      }
    }
  }

  sprovider->unlock();
  provider->unlock();  
}

/*void
put_surface(CL_LockableSurface* tsurface, CL_Surface* ssurface,
	    int x, int y)
{
  put_surface(tsurface, ssurface->get_provider(), x, y);
}*/

/*
void
put_surface(CL_LockableSurface* tsurface, CL_SurfaceProvider* sprovider,
	    int x, int y)
{
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;
  
  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  CL_Palette* palette;
  int x_offset, y_offset;

  assert(sprovider->get_pixel_format() == PAL8);
  //  assert(provider->get_pixel_format() == RGBA8888);

  tsurface->lock();
  sprovider->lock();

  tbuffer = static_cast<unsigned char*>(tsurface->get_data());
  sbuffer = static_cast<unsigned char*>(sprovider->get_data());
  
  palette = sprovider->get_palette();
  assert(palette);

  twidth  = tsurface->get_width();
  theight = tsurface->get_height();
  tpitch  = tsurface->get_pitch();

  swidth  = sprovider->get_width();
  sheight = sprovider->get_height();
  spitch  = sprovider->get_pitch();

  if (y < 0)
    y_offset = 0-y;
  else 
    y_offset = 0;
  
  if (x < 0) 
    x_offset = -x;
  else
    x_offset = 0;

  for(int line=y_offset; line < sheight && (line + y) < theight; ++line) {
    //    std::cout << "Line: " << line << std::endl;
    start_i = ((line + y) * tpitch) + (x*4);

    for(int i=start_i+(4*x_offset),j=line*spitch+x_offset; 
	i < start_i + (4*swidth) && (i-start_i+(x*4)) < (4*twidth); i+=4,++j) {
      if (sbuffer[j] != sprovider->get_transcol()) {
	tbuffer[i + 0] = 255;                                  // alpha
	tbuffer[i + 1] = palette->palette[sbuffer[j] * 3 + 2]; // blue
	tbuffer[i + 2] = palette->palette[sbuffer[j] * 3 + 1]; // green
	tbuffer[i + 3] = palette->palette[sbuffer[j] * 3 + 0]; // red
      }
    }
  }

  sprovider->unlock();
  tsurface->unlock();  
}
*/
// Converts a SurfaceProvider based surface, to a Canvas
// based one. The old one will not be deleted.
CL_Surface*
convert_to_emptyprovider(CL_Surface* ssurf)
{
  CL_Canvas* tprov = convert_to_emptyprovider(ssurf->get_provider());
  return CL_Surface::create(tprov, true);
}

// Converts a SurfaceProvider, to an Canvas and returns
// the newly allocated provider, you need to delete it yourself.
CL_Canvas*
convert_to_emptyprovider(CL_SurfaceProvider* sprov)
{
  CL_Canvas* tprov;
  CL_Palette* palette;
  unsigned char* sbuffer;
  unsigned char* tbuffer;
  int i;

  sprov->lock();
  switch(sprov->get_depth()) 
    {
    case 32:
      tprov = new CL_Canvas(sprov->get_width(),
			    sprov->get_height());
      tprov->lock();

      sbuffer = static_cast<unsigned char*>(sprov->get_data());
      tbuffer = static_cast<unsigned char*>(tprov->get_data());

      for(i=0; i < (tprov->get_height() * tprov->get_pitch()); ++i)
	{
	  tbuffer[i] = sbuffer[i];
	}

      tprov->unlock();
      break;
    case 8:
      tprov = new CL_Canvas(sprov->get_width(),
			    sprov->get_height());
      palette = sprov->get_palette();
      tprov->lock();
      
      sbuffer = static_cast<unsigned char*>(sprov->get_data());
      tbuffer = static_cast<unsigned char*>(tprov->get_data());    

      for(i=0; i < (sprov->get_height() * sprov->get_pitch()); ++i)
	{
	  tbuffer[i * 4 + 0] = 255;
	  tbuffer[i * 4 + 1] = palette->palette[sbuffer[i] * 3 + 2];
	  tbuffer[i * 4 + 2] = palette->palette[sbuffer[i] * 3 + 1];
	  tbuffer[i * 4 + 3] = palette->palette[sbuffer[i] * 3 + 0];
	}
      
      tprov->unlock();      
      break;
    default:
      std::cout << "convert_to_emptyprovider(): Wrong source format: " 
	   << static_cast<int>(sprov->get_depth()) << std::endl;
      assert(false);
      break;
    }
  sprov->unlock();
  
  return tprov;
}

/* EOF */
