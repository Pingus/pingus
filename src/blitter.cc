//  $Id: blitter.cc,v 1.20 2000/10/09 19:17:30 grumbel Exp $
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

#include <algorithm>
#include <cstdio>
#include <assert.h>
#include "PingusError.hh"
#include "blitter.hh"
#include "Color.hh"

void
Blitter::put_surface(CL_Canvas* canvas, CL_Surface* sur,
		     int x, int y)
{
  //Blitter::put_surface(canvas, sur->get_provider(), x, y);
  /*  if (sur->get_provider()->get_depth() != 8)
      sur->put_target(x, y, 0, canvas);
  else*/
  Blitter::put_surface(canvas, sur->get_provider(), x, y);
}

void 
Blitter::put_surface(CL_Canvas* canvas, CL_SurfaceProvider* provider,
		     int x, int y)
{
  char str[32];
  
  switch(provider->get_depth())
    {
    case  8:
      put_surface_8bit(canvas, provider, x, y);
      break;
    case 32:
      put_surface_32bit(canvas, provider, x, y);
      break;
    default: 
      sprintf(str, "%d", provider->get_depth());
      throw PingusError("Blitter:put_surface:Unknow color depth: " + std::string(str));
      break;
    }    
}

void
Blitter::put_surface_8bit(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
			  int x, int y)
{
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;
  
  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  CL_Palette* palette;
  int x_offset, y_offset;

  provider->lock();
  sprovider->lock();

  tbuffer = static_cast<unsigned char*>(provider->get_data());
  sbuffer = static_cast<unsigned char*>(sprovider->get_data());

  //std::cout << "Colorkey: " << sprovider->get_src_colorkey() << std::endl;
  
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
	  i < start_i + (4*swidth) && (i-start_i+(x*4)) < (4*twidth);
	  i += 4, ++j) 
	{
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
Blitter::put_surface_32bit(CL_Canvas* canvas, CL_SurfaceProvider* provider,
			   int x_pos, int y_pos)
{
  //std::cout << "Blitter::put_surface_32bit() --- not implemented" << std::endl;
  //return;
  float red, green, blue, alpha;
  float tred, tgreen, tblue, talpha;

  int swidth = provider->get_width();
  int sheight = provider->get_height();

  int twidth = canvas->get_width();
  int theight = canvas->get_height();

  canvas->lock();
  provider->lock();
  for(int y=max(0, -y_pos); y < sheight && (y + y_pos) < theight ; y++) {
    for(int x=max(0,-x_pos); x < swidth && (x + x_pos) < twidth; x++) {
      provider->get_pixel(x, y, &red, &green, &blue, &alpha);
      canvas->get_pixel(x + x_pos, y + y_pos, &tred, &tgreen, &tblue, &talpha);

      canvas->draw_pixel(x + x_pos, y + y_pos, 
			 max(0.0, min(1.0, (red * alpha) + (tred * (1.0-alpha)))),
			 max(0.0, min(1.0, (green * alpha) +(tgreen * (1.0-alpha)))),
			 max(0.0, min(1.0, (blue * alpha)  + (tblue * (1.0-alpha)))),
			 max(0.0, min(1.0, alpha * alpha + (talpha*(1.0-alpha)))));
    }
  }
  provider->unlock();
  canvas->unlock();

  /*
  unsigned char* tbuffer = static_cast<unsigned char*>(canvas->get_data());
  unsigned char* sbuffer = static_cast<unsigned char*>(provider->get_data());
  

  std::cout << "SRegion to draw: W = " << sx2 - sx1 << std::endl;
  std::cout << "                H = " << sy2 - sy1 << std::endl;
  std::cout << "TRegion to draw: W = " << tx2 - tx1 << std::endl;
  std::cout << "                H = " << ty2 - ty1 << std::endl;

  int width = tx2 - tx1;
  int height = ty2 - ty1;

  canvas->lock();
  provider->lock();
  for (int iy = 0; iy < height; iy++) 
    {
      int sstart_pos = ((sy1 + iy) * swidth) + sx1;
      int tstart_pos = ((ty1 + iy) * twidth) + tx1;
      
      for (int i = 0; i < width; i++)
	{
	  tbuffer[tstart_pos + i] = sbuffer[sstart_pos + i];
	}
    }
  provider->unlock();
  canvas->unlock();*/
}

void
Blitter::put_alpha_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
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

CL_Canvas*
Blitter::clear_canvas(CL_Canvas* canvas)
{
  unsigned char* buffer;
  
  canvas->lock();
  buffer = static_cast<unsigned char*>(canvas->get_data());

  /*  for (int i=0; i < canvas->get_pitch() * canvas->get_height(); i += 4)
    {
      buffer[i + 0] = 155;
      buffer[i + 1] = 155;
      buffer[i + 2] = 155;
      buffer[i + 3] = 155;
      }*/
  memset(buffer, 0, sizeof(unsigned char) * canvas->get_pitch() * canvas->get_height());

  canvas->unlock();

  return canvas;
}

CL_Canvas*
Blitter::create_canvas(CL_Surface* sur)
{
  return create_canvas(sur->get_provider());
}

CL_Canvas*
Blitter::create_canvas(CL_SurfaceProvider* prov)
{
  CL_Canvas* canvas = new CL_Canvas(prov->get_width(), prov->get_height());

  switch (prov->get_bytes_per_pixel())
    {
    case 3:
      {
	canvas->lock();
	prov->lock();
	
	int buffer_size = prov->get_pitch () * prov->get_height ();
	unsigned char* sbuffer = static_cast<unsigned char*>(prov->get_data ());
	unsigned char* tbuffer = static_cast<unsigned char*>(canvas->get_data ());
	
	for (int si = 0, ti = 0; si < buffer_size; si += 3, ti += 4)
	  {
	    tbuffer[ti + 0] = 255;
	    tbuffer[ti + 1] = sbuffer[si + 0];
	    tbuffer[ti + 2] = sbuffer[si + 1];
	    tbuffer[ti + 3] = sbuffer[si + 2];
	  }
	  
	prov->unlock();
	canvas->unlock();
      }

    case 4:
      canvas->lock();
      prov->lock();
      memcpy(canvas->get_data(), prov->get_data(),
 	     sizeof(unsigned char) * prov->get_height() * prov->get_pitch());
      prov->unlock();
      canvas->unlock();
      break;

    default:
      put_surface(canvas, prov, 0, 0);
      break;
    }
  return canvas;
}

CL_Surface* 
Blitter::scale_surface (CL_Surface* sur, int width, int height)
{
  Color color;
  CL_SurfaceProvider* provider = sur->get_provider ();
  CL_Canvas* canvas = new CL_Canvas (width, height);

  provider->lock ();
  canvas->lock ();

  unsigned char* sbuffer = static_cast<unsigned char*>(provider->get_data ());
  unsigned char* tbuffer = static_cast<unsigned char*>(canvas->get_data ());
  int pwidth = provider->get_width ();
  int pheight = provider->get_height ();
  
  std::cout << "starting scale..." << std::endl;
  
  switch (provider->get_bytes_per_pixel ())
    {
    case 3:
      {
	// We assume that we have the data in RGB888, which might not be
	// the case
	for (int y = 0; y < height; y++)
	  for (int x = 0; x < width; x++)
	    {
	      int ti = (y * width + x) * 4;
	      int si = ((y * pheight / height) * pwidth
			+ (x * pwidth / width)) * 3;
		
	      tbuffer[ti + 0] = 255; // alpha
	      tbuffer[ti + 1] = sbuffer[(si + 0)]; // blue
	      tbuffer[ti + 2] = sbuffer[(si + 1)]; // green
	      tbuffer[ti + 3] = sbuffer[(si + 2)]; // red
	    }
      }
      break;
    case 4:
      {
	// We assume that we have the data in RGBA8888, which might not be
	// the case
	for (int y = 0; y < height; y++)
	  for (int x = 0; x < width; x++)
	    {
	      int ti = (y * width + x) * 4;
	      int si = ((y * pheight / height) * pwidth
			+ (x * pwidth / width)) * 4;
		
	      tbuffer[ti + 0] = sbuffer[(si + 0)]; // alpha
	      tbuffer[ti + 1] = sbuffer[(si + 1)]; // blue
	      tbuffer[ti + 2] = sbuffer[(si + 2)]; // green
	      tbuffer[ti + 3] = sbuffer[(si + 3)]; // red
	    }
      }
      break;
    default:
      // Slow but generic, using get_data () would be better, but would
      // require quite a bit of work
      for (int y = 0; y < height; y++)
	for (int x = 0; x < width; x++)
	  {
	    // std::cout << "X: " << x << " Y: " << y << std::endl;
	    provider->get_pixel (x * provider->get_width () / width ,
				 y * provider->get_height () / height,
				 &color.red, &color.green, &color.blue, &color.alpha);
	    // ignoring the source alpha due to get_pixel brokeness... no time to test the patch
	    canvas->draw_pixel (x, y, color.red, color.green, color.blue, color.alpha);
	  }
      break;
    }

  std::cout << "scale done" << std::endl;
  canvas->unlock ();
  provider->unlock ();

  return CL_Surface::create (canvas, true);
}

/*
// Converts a SurfaceProvider based surface, to a Canvas
// based one. The old one will not be deleted.
CL_Surface*
Blitter::convert_to_emptyprovider(CL_Surface* ssurf)
{
  CL_Canvas* tprov = convert_to_emptyprovider(ssurf->get_provider());
  return CL_Surface::create(tprov, true);
}

// Converts a SurfaceProvider, to an Canvas and returns
// the newly allocated provider, you need to delete it yourself.
CL_Canvas*
Blitter::convert_to_emptyprovider(CL_SurfaceProvider* sprov)
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
*/ 

/* EOF */
