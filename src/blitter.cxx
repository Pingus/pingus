//  $Id: blitter.cxx,v 1.34 2003/10/21 21:37:05 grumbel Exp $
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

#include <config.h>
#include <stdio.h>
#include <assert.h>
#include <ClanLib/Core/IOData/datatypes.h>
#include <ClanLib/Display/palette.h>
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/pixel_format.h>

#include "pingus_error.hxx"
#include "color.hxx"
#include "string_converter.hxx"
#include "globals.hxx"
#include "math.hxx"
#include "blitter.hxx"
#include "debug.hxx"
#include "blitter_impl.hxx"

/* Headers needed for i18n / gettext */
#include "gettext.h"

#define COMPILE_WITH_MEMORY_HOLE 0

namespace Pingus {

void
Blitter::put_surface(CL_PixelBuffer canvas, const CL_Surface& sur,
		     int x, int y)
{
  Blitter::put_surface(canvas, sur.get_pixeldata(), x, y);
}

void
Blitter::put_surface(CL_PixelBuffer canvas, CL_PixelBuffer provider,
		     int x, int y)
{
  switch(provider.get_format().get_depth())
    {
    case  8:
      put_surface_8bit(canvas, provider, x, y);
      break;
    case 32:
      put_surface_32bit(canvas, provider, x, y);
      break;
    default:
      PingusError::raise("Blitter:put_surface:Unknown color depth: " + to_string(provider.get_format().get_depth()));
      break;
    }
}

void
Blitter::put_surface_8bit(CL_PixelBuffer provider, CL_PixelBuffer sprovider,
			  int x, int y)
{
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;

  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  int x_offset, y_offset;

  provider.lock();
  sprovider.lock();

  tbuffer = static_cast<unsigned char*>(provider.get_data());
  sbuffer = static_cast<unsigned char*>(sprovider.get_data());

  //std::cout << "Colorkey: " << sprovider.get_src_colorkey() << std::endl;

  CL_Palette palette = sprovider.get_palette();

  twidth  = provider.get_width();
  theight = provider.get_height();
  tpitch  = provider.get_pitch();

  swidth  = sprovider.get_width();
  sheight = sprovider.get_height();
  spitch  = sprovider.get_pitch();

  if (y < 0)
    y_offset = 0-y;
  else
    y_offset = 0;

  if (x < 0)
    x_offset = -x;
  else
    x_offset = 0;

  if (sprovider.get_format().has_colorkey ())
    {
      std::cout << "8bit blit: " << twidth << "x" << theight << " " << swidth << "x" << sheight << std::endl;

      unsigned int colorkey = sprovider.get_format().get_colorkey();

      for(int line=y_offset;
	  line < sheight && (line + y) < theight;
	  ++line)
	{
	  start_i = ((line + y) * tpitch) + (x*4);

	  for(int i=start_i+(4*x_offset),j=line*spitch+x_offset;
	      (i < start_i + (4*swidth))
		&& ((i-start_i+(x*4)) < (4*twidth));
	      i += 4, ++j)
	    {
	      if (sbuffer[j] != colorkey)
		{
		  tbuffer[i + 0] = 255;                                  // alpha
		  tbuffer[i + 1] = palette.colors[cl_uint8(sbuffer[j] * 3)].get_blue(); // blue
		  tbuffer[i + 2] = palette.colors[cl_uint8(sbuffer[j] * 3)].get_green(); // green
		  tbuffer[i + 3] = palette.colors[cl_uint8(sbuffer[j] * 3)].get_red(); // red
		}
	    }
	}
    }
  else
    {
      std::cout << "8bit blit: " << twidth << "x" << theight << " " << swidth << "x" << sheight << std::endl;

      for(int line=y_offset;
	  line < sheight && (line + y) < theight;
	  ++line)
	{
	  start_i = ((line + y) * tpitch) + (x*4);

	  for(int i=start_i+(4*x_offset),j=line*spitch+x_offset;
	      (i < start_i + (4*swidth))
		&& ((i-start_i+(x*4)) < (4*twidth));
	      i += 4, ++j)
	    {
	      tbuffer[i + 0] = 255;                                  // alpha
	      tbuffer[i + 1] = palette[cl_uint8(sbuffer[j] * 3 + 2)]; // blue
	      tbuffer[i + 2] = palette[cl_uint8(sbuffer[j] * 3 + 1)]; // green
	      tbuffer[i + 3] = palette[cl_uint8(sbuffer[j] * 3 + 0)]; // red
	    }
	}
    }

  sprovider.unlock();
  provider.unlock();
}

void
Blitter::put_surface_32bit(CL_PixelBuffer canvas, CL_PixelBuffer provider_,
			   const int x_pos, const int y_pos)
{
  CL_PixelBuffer provider = provider_;

  pout(PINGUS_DEBUG_BLITTER) << "Blitting: SurfaceProvider:" << provider.get_width ()
                             << "x" << provider.get_height () << ":" 
                	     << " Canvas:" << canvas.get_width () << "x"
                             << canvas.get_height () << ":" <<  std::endl;

  int swidth = provider.get_width();
  int sheight = provider.get_height();

  int twidth = canvas.get_width();
  int theight = canvas.get_height();

  // Surface is out of the screen
  if (x_pos > twidth-1 || y_pos > theight-1)
    return;

  canvas.lock();
  provider.lock();

  if (1) // slow
    {
      for(int y = Math::max(0, -y_pos); y < sheight && (y + y_pos) < theight; ++y)
	for(int x = Math::max(0,-x_pos); x < swidth && (x + x_pos) < twidth; ++x)
	  {
            CL_Color sc = provider.get_pixel(x, y);
            CL_Color tc = canvas.get_pixel(x + x_pos, y + y_pos);

            // FIXME: This doesn't give correct alpha values
	    canvas.draw_pixel(x + x_pos, y + y_pos,
                              CL_Color(Math::mid(0, 255, 
                                                 int((sc.get_red()   * sc.get_alpha()) + (tc.get_red()   * (255 - sc.get_alpha())))),
                                       Math::mid(0, 255, 
                                                 int((sc.get_green() * sc.get_alpha()) + (tc.get_green() * (255 - sc.get_alpha())))),
                                       Math::mid(0, 255,
                                                 int((sc.get_blue()  * sc.get_alpha()) + (tc.get_blue()  * (255 - sc.get_alpha())))),
                                       Math::mid(0, 255,
                                                 int((sc.get_alpha() * sc.get_alpha()  + (tc.get_alpha() * (255 - sc.get_alpha())))))));
	  }
    }
  else // fast?!
    {

    }

  provider.unlock();
  canvas.unlock();
}

void
Blitter::put_alpha_surface(CL_PixelBuffer provider, CL_PixelBuffer sprovider,
			   int x, int y)
{
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;

  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  CL_Palette palette;
  int x_offset, y_offset;

  provider.lock();
  sprovider.lock();

  //  assert(sprovider.get_format().get_depth() == 8);
  if (sprovider.get_format().get_depth() != 8)
    {
      sprovider.unlock ();
      provider.unlock ();
      PingusError::raise("Image has wrong color depth: " + to_string(sprovider.get_format().get_depth()));
    }
  //  assert(provider.get_pixel_format() == RGBA8888);

  tbuffer = static_cast<unsigned char*>(provider.get_data());
  sbuffer = static_cast<unsigned char*>(sprovider.get_data());

  palette = sprovider.get_palette();
  
  twidth  = provider.get_width();
  theight = provider.get_height();
  tpitch  = provider.get_pitch();

  swidth  = sprovider.get_width();
  sheight = sprovider.get_height();
  spitch  = sprovider.get_pitch();

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

  sprovider.unlock();
  provider.unlock();
}

CL_PixelBuffer
Blitter::clear_canvas(CL_PixelBuffer canvas)
{
  unsigned char* buffer;

  canvas.lock();
  buffer = static_cast<unsigned char*>(canvas.get_data());
  memset(buffer, 0, sizeof(unsigned char) * canvas.get_pitch() * canvas.get_height());
  canvas.unlock();

  return canvas;
}

CL_PixelBuffer
Blitter::create_canvas(const CL_Surface& sur)
{
  return create_canvas(sur.get_pixeldata());
}

CL_PixelBuffer
Blitter::create_canvas(CL_PixelBuffer prov)
{
  CL_PixelBuffer canvas(prov.get_width(), prov.get_height(), prov.get_width()*4, CL_PixelFormat::rgba8888);

  switch (prov.get_format().get_depth())
    {
      // RGB888
    case 24:
      {
	canvas.lock();
	prov.lock();

	int buffer_size = prov.get_pitch () * prov.get_height ();
	unsigned char* sbuffer = static_cast<unsigned char*>(prov.get_data ());
	unsigned char* tbuffer = static_cast<unsigned char*>(canvas.get_data ());

	for (int si = 0, ti = 0; si < buffer_size; si += 3, ti += 4)
	  {
	    tbuffer[ti + 0] = 255; // Alpha
	    tbuffer[ti + 1] = sbuffer[si + 0];
	    tbuffer[ti + 2] = sbuffer[si + 1];
	    tbuffer[ti + 3] = sbuffer[si + 2];
	  }

	// -FIXME: memory hole
	prov.unlock();
	canvas.unlock();
      }
      break;

      // RGBA8888
    case 32:
      canvas.lock();
      prov.lock();
      memcpy(canvas.get_data(), prov.get_data(),
 	     sizeof(unsigned char) * prov.get_height() * prov.get_pitch());
      prov.unlock();
      canvas.unlock();
      break;

    default:
      put_surface(canvas, prov, 0, 0);
      break;
    }
  return canvas;
}

CL_Surface
Blitter::scale_surface (const CL_Surface& sur, int width, int height)
{
  CL_PixelBuffer buf = Blitter::scale_surface_to_canvas(sur, width, height);
  return CL_Surface(new CL_PixelBuffer(buf), true);
}

CL_PixelBuffer
Blitter::scale_surface_to_canvas(const CL_Surface& sur, int width, int height)
{
  CL_PixelBuffer provider = sur.get_pixeldata();
  CL_PixelBuffer canvas(width, height, width*4, CL_PixelFormat::rgba8888);

  provider.lock ();
  canvas.lock ();

  unsigned char* sbuffer = static_cast<unsigned char*>(provider.get_data ());
  unsigned char* tbuffer = static_cast<unsigned char*>(canvas.get_data ());
  int pwidth = provider.get_width ();
  int pheight = provider.get_height ();

  if (provider.get_format().get_type() ==  pixelformat_index)
    {
      Color color;
  
      // Slow but generic, using get_data () would be better, but would
      // require quite a bit of work
      for (int y = 0; y < height; ++y)
	{
	  for (int x = 0; x < width; ++x)
	    {
	      unsigned char pixel = *(static_cast<unsigned char*>(provider.get_data ())
				      + (y * pheight/height) * provider.get_pitch() + (x * pwidth/width));

	      color.red   = provider.get_palette().colors[pixel*3 +0] / 255.0f;
	      color.green = provider.get_palette().colors[pixel*3 +1] / 255.0f;
	      color.blue  = provider.get_palette().colors[pixel*3 +2] / 255.0f;

	      if (provider.get_format().has_colorkey()
                  && provider.get_format().get_colorkey() == pixel)
		color.alpha = 0.0f;
	      else
		color.alpha = 1.0f;
              
	      // FIXME: ignoring the source alpha due to get_pixel brokeness... no time to test the patch
	      canvas.draw_pixel (x, y, color.to_cl_color());
	    }
	}
    }
  else
    {
      switch (provider.get_format().get_depth())
	{
	case 24:
	  {
	    // We assume that we have the data in RGB888, which might not be
	    // the case
	    for (int y = 0; y < height; ++y)
	      for (int x = 0; x < width; ++x)
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
	case 32:
	  {
	    // We assume that we have the data in RGBA8888, which might not be
	    // the case
	    for (int y = 0; y < height; ++y)
	      for (int x = 0; x < width; ++x)
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
	  for (int y = 0; y < height; ++y)
	    for (int x = 0; x < width; ++x)
	      {
		// std::cout << "X: " << x << " Y: " << y << std::endl;
		CL_Color color = provider.get_pixel(x * provider.get_width () / width,
                                                    y * provider.get_height () / height);
		// FIXME: ignoring the source alpha due to get_pixel
		// brokeness... no time to test the patch
		canvas.draw_pixel(x, y, color);
              }
	  break;
	}
    }

  canvas.unlock ();
  provider.unlock ();

  return canvas;
}

/*
// Converts a SurfaceProvider based surface, to a Canvas
// based one. The old one will not be deleted.
CL_Surface
Blitter::convert_to_emptyprovider(CL_Surface ssurf)
{
CL_PixelBuffer& tprov = convert_to_emptyprovider(ssurf.get_provider());
return CL_Surface::create(tprov, true);
}

// Converts a SurfaceProvider, to an Canvas and returns
// the newly allocated provider, you need to delete it yourself.
CL_PixelBuffer&
Blitter::convert_to_emptyprovider(CL_PixelBuffer& sprov)
{
  CL_PixelBuffer* tprov;
  CL_Palette* palette;
  unsigned char* sbuffer;
  unsigned char* tbuffer;
  int i;

  sprov.lock();
  switch(sprov.get_format().get_depth())
    {
    case 32:
      tprov = new CL_PixelBuffer(sprov.get_width(),
			    sprov.get_height());
      tprov.lock();

      sbuffer = static_cast<unsigned char*>(sprov.get_data());
      tbuffer = static_cast<unsigned char*>(tprov.get_data());

      for(i=0; i < (tprov.get_height() * tprov.get_pitch()); ++i)
	{
	  tbuffer[i] = sbuffer[i];
	}

      tprov.unlock();
      break;
    case 8:
      tprov = new CL_PixelBuffer(sprov.get_width(),
			    sprov.get_height());
      palette = sprov.get_palette();
      tprov.lock();

      sbuffer = static_cast<unsigned char*>(sprov.get_data());
      tbuffer = static_cast<unsigned char*>(tprov.get_data());

      for(i=0; i < (sprov.get_height() * sprov.get_pitch()); ++i)
	{
	  tbuffer[i * 4 + 0] = 255;
	  tbuffer[i * 4 + 1] = palette.palette[sbuffer[i] * 3 + 2];
	  tbuffer[i * 4 + 2] = palette.palette[sbuffer[i] * 3 + 1];
	  tbuffer[i * 4 + 3] = palette.palette[sbuffer[i] * 3 + 0];
	}

      tprov.unlock();
      break;
    default:
      std::cout << "convert_to_emptyprovider(): Wrong source format: "
		<< static_cast<int>(sprov.get_format().get_depth()) << std::endl;
      assert(false);
      break;
    }
  sprov.unlock();

  return tprov;
}
*/

/** Flip a surface horizontal */
CL_Surface
Blitter::flip_horizontal (const CL_Surface& sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_flip());
}

/** Flip a surface vertical */
CL_Surface
Blitter::flip_vertical (const CL_Surface& sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180_flip());
}

/** Rotate a surface 90 degrees */
CL_Surface
Blitter::rotate_90 (const CL_Surface& sur)
{
#ifdef CLANLIB_0_6
  CL_PixelBuffer prov = sur.get_pixeldata();

  if (prov.get_format().get_type() ==  pixelformat_index)
    {
      //std::cout << "Using indexed blitter" << std::endl;
      int pwidth  = prov.get_width();
      int pheight = prov.get_height();

      IndexedCanvas* canvas = new IndexedCanvas(pheight, pwidth);
      if (prov->uses_src_colorkey())
        canvas->set_src_colorkey(prov->get_src_colorkey());

      prov.lock();
      canvas.lock();

      canvas.set_palette(prov.get_palette());

      unsigned char* source_buf = static_cast<unsigned char*>(prov.get_data());
      unsigned char* target_buf = static_cast<unsigned char*>(canvas.get_data());

      for (int y = 0; y < pheight; ++y)
        for (int x = 0; x < pwidth; ++x)
          {
            target_buf[x * pheight + (pheight - y - 1)] = source_buf[y * pwidth + x];
          }

      canvas.unlock();
      prov.unlock();
      return CL_Surface(canvas, true);
    }
  else
    {
      CL_PixelBuffer* canvas = new CL_PixelBuffer (sur.get_height (), sur.get_width ());

      prov.lock ();
      canvas.lock ();

      CL_Color color;
      for (int y = 0; y < sur.get_height (); ++y)
        for (int x = 0; x < sur.get_width (); ++x)
          {
            color = prov->get_pixel (x, y);
            canvas.draw_pixel (sur.get_height () - 1 - y, x , color);
          }

      canvas.unlock ();
      prov.unlock ();
      return CL_Surface(canvas, true);
    }
#endif
  return CL_Surface();
}


CL_Surface
Blitter::rotate_180 (const CL_Surface& sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180());
}

CL_Surface
Blitter::rotate_270 (const CL_Surface& sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot270());
}

CL_Surface
Blitter::rotate_90_flip (const CL_Surface& sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot90_flip());
  //return Blitter::flip_horizontal(Blitter::rotate_90(sur));
}

CL_Surface
Blitter::rotate_180_flip (const CL_Surface& sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180_flip());
  //return Blitter::flip_horizontal(Blitter::rotate_180(sur));
}

CL_Surface
Blitter::rotate_270_flip (const CL_Surface& sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot270_flip());
  //return Blitter::flip_horizontal(Blitter::rotate_270(sur));
}

} // namespace Pingus

/* EOF */
