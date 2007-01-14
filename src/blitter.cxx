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
#include <ClanLib/Core/System/clanstring.h>
#include <ClanLib/Core/IOData/cl_endian.h>
#include <ClanLib/Core/IOData/datatypes.h>
#include <ClanLib/Display/palette.h>
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/pixel_format.h>

#include "pingus_error.hxx"
#include "globals.hxx"
#include "math.hxx"
#include "blitter.hxx"
#include "debug.hxx"
#include "blitter_impl.hxx"

/* Headers needed for i18n / gettext */
#include "gettext.h"

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
      PingusError::raise("Blitter:put_surface:Unknown color depth: " + CL_String::to(provider.get_format().get_depth()));
      break;
    }
}

void
Blitter::put_surface_8bit(CL_PixelBuffer target, CL_PixelBuffer source,
                          int x_pos, int y_pos)
{
  //std::cout << "8bit blit" << std::endl;

  assert(target.get_format().get_depth() == 32);

  target.lock();
  source.lock();

  int swidth  = source.get_width();
  int twidth  = target.get_width();

  int start_x = std::max(0, -x_pos);
  int start_y = std::max(0, -y_pos);

  int end_x = std::min(swidth,  twidth  - x_pos);
  int end_y = std::min(source.get_height(), target.get_height() - y_pos);

  if (end_x - start_x <= 0 || end_y - start_y <= 0)
    return;

  cl_uint8* target_buf = static_cast<cl_uint8*>(target.get_data());
  cl_uint8* source_buf = static_cast<cl_uint8*>(source.get_data());

  CL_Palette palette = source.get_palette();

  if (source.get_format().has_colorkey())
    {
      unsigned int colorkey = source.get_format().get_colorkey();

      for (int y = start_y; y < end_y; ++y)
        {
          cl_uint8* tptr = target_buf + 4*((twidth*(y+y_pos)) + x_pos + start_x);
          cl_uint8* sptr = source_buf + swidth*y + start_x;

          for (int x = start_x; x < end_x; ++x)
            { 
              if (*sptr != colorkey)
                {
				  if (!CL_Endian::is_system_big())
				  {
                    tptr[0] = 255;
                    tptr[1] = palette.colors[*sptr].get_blue();
                    tptr[2] = palette.colors[*sptr].get_green();
                    tptr[3] = palette.colors[*sptr].get_red();
				  }
				  else
				  {
                    tptr[3] = 255;
                    tptr[2] = palette.colors[*sptr].get_blue();
                    tptr[1] = palette.colors[*sptr].get_green();
                    tptr[0] = palette.colors[*sptr].get_red();
				  }
                }

              tptr += 4;
              sptr += 1;
            }
        }
    }
  else
    {
      for (int y = start_y; y < end_y; ++y)
        {
          cl_uint8* tptr = target_buf + 4*((twidth*(y+y_pos)) + x_pos + start_x);
          cl_uint8* sptr = source_buf + swidth*y + start_x;

          for (int x = start_x; x < end_x; ++x)
            {
              if (!CL_Endian::is_system_big())
              {
                tptr[0] = 255;
                tptr[1] = palette.colors[*sptr].get_blue();
                tptr[2] = palette.colors[*sptr].get_green();
                tptr[3] = palette.colors[*sptr].get_red();
              }
              else
              {
                tptr[3] = 255;
                tptr[2] = palette.colors[*sptr].get_blue();
                tptr[1] = palette.colors[*sptr].get_green();
                tptr[0] = palette.colors[*sptr].get_red();
              }

              tptr += 4;
              sptr += 1;
            }
        }
    }
  
  source.unlock();
  target.unlock();
}

void
Blitter::put_surface_32bit(CL_PixelBuffer target, CL_PixelBuffer source,
			   const int x_pos, const int y_pos)
{
  //std::cout << "32bit blit" << std::endl;

  target.lock();
  source.lock();

  int swidth  = source.get_width();
  int sheight = source.get_height();

  int twidth  = target.get_width();
  int theight = target.get_height();

  int start_x = std::max(0, -x_pos);
  int start_y = std::max(0, -y_pos);

  int end_x = std::min(swidth,  twidth  - x_pos);
  int end_y = std::min(sheight, theight - y_pos);

  if (end_x - start_x <= 0
      || end_y - start_y <= 0)
    return;

  /* Benchmarks: 
   * ===========
   * 6msec with memcpy
   * 10msec with uint32
   * 17msec with uint8
   */
      
  cl_uint8* target_buf = static_cast<cl_uint8*>(target.get_data());
  cl_uint8* source_buf = static_cast<cl_uint8*>(source.get_data());

  for (int y = start_y; y < end_y; ++y)
    {
      int tidx = 4*(twidth * (y_pos + y) + x_pos);
      int sidx = 4*(swidth * y);
      
			/*
      if (0)
        { // Fast but doesn't handle masks
          memcpy(target_buf + tidx + 4*start_x, source_buf + sidx + 4*start_x, 
                 sizeof(cl_uint32)*(end_x - start_x));
        }
      else if (0)
        { // doesn't handle masks either, but looks half correct
          cl_uint8* tptr = target_buf + tidx + 4*start_x;
          cl_uint8* sptr = source_buf + sidx + 4*start_x;

          for (int x = start_x; x < end_x; ++x)
            {
              if (!CL_Endian::is_system_big())
              {
                tptr[0] = sptr[3];
                tptr[1] = sptr[0];
                tptr[2] = sptr[1];
                tptr[3] = sptr[2];
              }
              else
              {
                tptr[3] = sptr[3];
                tptr[2] = sptr[0];
                tptr[1] = sptr[1];
                tptr[0] = sptr[2];
              }

              tptr += 4;
              sptr += 4;
            }
        }
      else */
        {
          // doesn't handle masks either, but looks half correct
          cl_uint8* tptr = target_buf + tidx + 4*start_x;
          cl_uint8* sptr = source_buf + sidx + 4*start_x;

          for (int x = start_x; x < end_x; ++x)
            {
              if (!CL_Endian::is_system_big())
              {
								float a = sptr[0]/255.0f;
                tptr[0] = Math::mid(0, int((1.0f - a) * tptr[0] + a * sptr[0]), 255);
                tptr[1] = Math::mid(0, int((1.0f - a) * tptr[1] + a * sptr[1]), 255);
                tptr[2] = Math::mid(0, int((1.0f - a) * tptr[2] + a * sptr[2]), 255);
                tptr[3] = Math::mid(0, int((1.0f - a) * tptr[3] + a * sptr[3]), 255);
              }
              else
              {
                float a = sptr[3]/255.0f;
                tptr[3] = Math::mid(0, int((1.0f - a) * tptr[3] + a * sptr[3]), 255);
                tptr[2] = Math::mid(0, int((1.0f - a) * tptr[2] + a * sptr[0]), 255);
                tptr[1] = Math::mid(0, int((1.0f - a) * tptr[1] + a * sptr[1]), 255);
                tptr[0] = Math::mid(0, int((1.0f - a) * tptr[0] + a * sptr[2]), 255);
              }

              tptr += 4;
              sptr += 4;
            }
        }
    }
  
  source.unlock();
  target.unlock();
}

void
Blitter::fill_rect(CL_PixelBuffer target, const CL_Rect& rect, const CL_Color& color)
{
  if (target.get_format().get_depth() != 32
      && target.get_format().get_depth() != 24)
    {
      std::cout << "Blitter::fill_rect: depth must be 32 but is " << target.get_format().get_depth() << std::endl;
      return;
    }

  target.lock();
  
  int twidth  = target.get_width();
  int swidth  = rect.get_width();

  int start_x = std::max(0, -rect.left);
  int start_y = std::max(0, -rect.top);

  int end_x = std::min(swidth,  twidth  - rect.left);
  int end_y = std::min(rect.get_height(), target.get_height() - rect.top);

  if (end_x - start_x <= 0 || end_y - start_y <= 0)
    return;

  cl_uint8* target_buf = static_cast<cl_uint8*>(target.get_data());

  if (target.get_format().get_depth() == 24)
    {
      if (color.get_alpha() == 255)
        {
          for (int y = start_y; y < end_y; ++y)
            {
              cl_uint8* tptr = target_buf + 3*((twidth*(y + rect.top)) + rect.left + start_x);

              for (int x = start_x; x < end_x; ++x)
                { 
                  if (!CL_Endian::is_system_big())
                  {
                    tptr[0] = color.get_red();
                    tptr[1] = color.get_green();
                    tptr[2] = color.get_blue();
                  }
                  else
                  {
                    tptr[2] = color.get_red();
                    tptr[1] = color.get_green();
                    tptr[0] = color.get_blue();
                  }
                  tptr += 3;
                }
            }
        }
      else
        {
          float a = color.get_alpha()/255.0f;

          for (int y = start_y; y < end_y; ++y)
            {
              cl_uint8* tptr = target_buf + 3*((twidth*(y + rect.top)) + rect.left + start_x);

              for (int x = start_x; x < end_x; ++x)
                { 
                  if (!CL_Endian::is_system_big())
                  {
                    tptr[0] = Math::mid(0, int(((1.0f - a) * (tptr[0])) + a * color.get_blue()) , 255); //blue
                    tptr[1] = Math::mid(0, int(((1.0f - a) * (tptr[1])) + a * color.get_green()), 255); //green
                    tptr[2] = Math::mid(0, int(((1.0f - a) * (tptr[2])) + a * color.get_red()), 255); //red
                  }
                  else
                  {
                    tptr[2] = Math::mid(0, int(((1.0f - a) * (tptr[2])) + a * color.get_blue()) , 255); //blue
                    tptr[1] = Math::mid(0, int(((1.0f - a) * (tptr[1])) + a * color.get_green()), 255); //green
                    tptr[0] = Math::mid(0, int(((1.0f - a) * (tptr[0])) + a * color.get_red()), 255); //red
                  }

                  tptr += 3;
                }
            }
        }
    }
  else if (target.get_format().get_depth() == 32)
    {
      if (color.get_alpha() == 255)
        {
          for (int y = start_y; y < end_y; ++y)
            {
              cl_uint8* tptr = target_buf + 4*((twidth*(y + rect.top)) + rect.left + start_x);

              for (int x = start_x; x < end_x; ++x)
                { 
                  if (!CL_Endian::is_system_big())
                  {
                    tptr[0] = 255;
                    tptr[1] = color.get_blue();
                    tptr[2] = color.get_green();
                    tptr[3] = color.get_red();
                  }
                  else
                  {
                    tptr[3] = 255;
                    tptr[2] = color.get_blue();
                    tptr[1] = color.get_green();
                    tptr[0] = color.get_red();
                  }
                  tptr += 4;
                }
            }
        }
      else
        {
          for (int y = start_y; y < end_y; ++y)
            {
              cl_uint8* tptr = target_buf + 4*((twidth*(y + rect.top)) + rect.left + start_x);

              for (int x = start_x; x < end_x; ++x)
                { 
                  float a = color.get_alpha()/255.0f;

                  if (!CL_Endian::is_system_big())
                  {
                    tptr[0] = Math::mid(0, int(tptr[0] + a * color.get_alpha()), 255);
                    tptr[1] = Math::mid(0, int((1.0f - a) * tptr[1] + a * color.get_blue()) , 255);
                    tptr[2] = Math::mid(0, int((1.0f - a) * tptr[2] + a * color.get_green()), 255);
                    tptr[3] = Math::mid(0, int((1.0f - a) * tptr[3] + a * color.get_red())  , 255);
                  }
                  else
                  {
                    tptr[3] = Math::mid(0, int(tptr[3] + a * color.get_alpha()), 255);
                    tptr[2] = Math::mid(0, int((1.0f - a) * tptr[2] + a * color.get_blue()) , 255);
                    tptr[1] = Math::mid(0, int((1.0f - a) * tptr[1] + a * color.get_green()), 255);
                    tptr[0] = Math::mid(0, int((1.0f - a) * tptr[0] + a * color.get_red())  , 255);
                  }
                }
            }
        }
    }
  
  target.unlock();
}

void
Blitter::clear_canvas(CL_PixelBuffer canvas, CL_Color color)
{
  unsigned char* buffer;

  canvas.lock();
  buffer = static_cast<unsigned char*>(canvas.get_data());
  memset(buffer, color.color, sizeof(unsigned char) * canvas.get_pitch() * canvas.get_height());
  canvas.unlock();
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
	    if (!CL_Endian::is_system_big())
        {
          tbuffer[ti + 0] = 255; // Alpha
          tbuffer[ti + 1] = sbuffer[si + 0];
          tbuffer[ti + 2] = sbuffer[si + 1];
          tbuffer[ti + 3] = sbuffer[si + 2];
        }
        else
        {
          tbuffer[ti + 3] = 255; // Alpha
          tbuffer[ti + 2] = sbuffer[si + 0];
          tbuffer[ti + 1] = sbuffer[si + 1];
          tbuffer[ti + 0] = sbuffer[si + 2];
        }
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
  return CL_Surface(CL_PixelBuffer(buf));
}

CL_PixelBuffer
Blitter::scale_surface_to_canvas (CL_PixelBuffer provider, int width, int height)
{
  CL_PixelBuffer canvas(width, height, width*4, CL_PixelFormat::rgba8888);

  provider.lock ();
  canvas.lock ();

  unsigned char* sbuffer = static_cast<unsigned char*>(provider.get_data ());
  unsigned char* tbuffer = static_cast<unsigned char*>(canvas.get_data ());
  int pwidth = provider.get_width ();
  int pheight = provider.get_height ();
  int pitch = provider.get_pitch();

	if (provider.get_format().get_type() ==  pixelformat_index)
	{
		CL_Color color;
		pout(PINGUS_DEBUG_ACTIONS) << 
			"Blitter::scale_surface_to_canvas() - Scaling indexed image" << std::endl;
		  
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
			{
				unsigned offset = (y * pheight/height) * pitch + (x * pwidth/width);

				color = provider.get_palette().colors[sbuffer[offset]];

				// Detrmine alpha channel
				if (provider.get_format().has_colorkey()
					&& provider.get_format().get_colorkey() == sbuffer[offset])
					color.set_alpha(0);
				else
					color.set_alpha(255);
     
				canvas.draw_pixel(x, y, color);
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
	      pout(PINGUS_DEBUG_ACTIONS) << 
	      	"Blitter::scale_surface_to_canvas() - Scaling 24 bit image" << std::endl;
	    for (int y = 0; y < height; ++y)
	      for (int x = 0; x < width; ++x)
		{
		  int ti = (y * width + x) * 4;
		  int si = ((y * pheight / height) * pwidth
			    + (x * pwidth / width)) * 3;

          if (!CL_Endian::is_system_big())
          {
            tbuffer[ti + 0] = 255; // alpha
            tbuffer[ti + 1] = sbuffer[(si + 0)]; // blue
            tbuffer[ti + 2] = sbuffer[(si + 1)]; // green
            tbuffer[ti + 3] = sbuffer[(si + 2)]; // red
          }
          else
          {
            tbuffer[ti + 3] = 255; // alpha
            tbuffer[ti + 0] = sbuffer[(si + 0)]; // blue
            tbuffer[ti + 1] = sbuffer[(si + 1)]; // green
            tbuffer[ti + 2] = sbuffer[(si + 2)]; // red
          }
		}
	  }
	  break;
	case 32:
	  {
	    // We assume that we have the data in RGBA8888, which might not be
	    // the case
		pout(PINGUS_DEBUG_ACTIONS) << 
			"Blitter::scale_surface_to_canvas() - Scaling 32 bit image" << std::endl;
			
	    for (int y = 0; y < height; ++y)
	      for (int x = 0; x < width; ++x)
		{
		  int ti = (y * width + x) * 4;
		  int si = ((y * pheight / height) * pwidth
			    + (x * pwidth / width)) * 4;

          if (!CL_Endian::is_system_big())
          {
            tbuffer[ti + 0] = sbuffer[(si + 0)]; // alpha
            tbuffer[ti + 1] = sbuffer[(si + 1)]; // blue
            tbuffer[ti + 2] = sbuffer[(si + 2)]; // green
            tbuffer[ti + 3] = sbuffer[(si + 3)]; // red
          }
          else
          {
            tbuffer[ti + 3] = sbuffer[(si + 0)]; // alpha
            tbuffer[ti + 2] = sbuffer[(si + 1)]; // blue
            tbuffer[ti + 1] = sbuffer[(si + 2)]; // green
            tbuffer[ti + 0] = sbuffer[(si + 3)]; // red
          }
		}
	  }
	  break;
	default:
		// Slow but generic, using get_data () would be better, but would
		// require quite a bit of work
		pout(PINGUS_DEBUG_ACTIONS) << 
			"Blitter::scale_surface_to_canvas() - Scaling image using default method" 
			<< std::endl;

		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
			{
				CL_Color color = provider.get_pixel(x * pwidth / width,
					y * pheight / height);
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

CL_PixelBuffer
Blitter::scale_surface_to_canvas(const CL_Surface& sur, int width, int height)
{
  return Blitter::scale_surface_to_canvas(sur.get_pixeldata(), width, height);
}

/** Flip a surface horizontal */
CL_PixelBuffer
Blitter::flip_horizontal (CL_PixelBuffer prov)
{
  return BlitterImpl::modify(prov, BlitterImpl::transform_flip());
}

/** Flip a surface vertical */
CL_PixelBuffer
Blitter::flip_vertical (CL_PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180_flip());
}

/** Rotate a surface 90 degrees */
CL_PixelBuffer
Blitter::rotate_90 (CL_PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot90());
}


CL_PixelBuffer
Blitter::rotate_180 (CL_PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180());
}

CL_PixelBuffer
Blitter::rotate_270 (CL_PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot270());
}

CL_PixelBuffer
Blitter::rotate_90_flip (CL_PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot90_flip());
}

CL_PixelBuffer
Blitter::rotate_180_flip (CL_PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot180_flip());
}

CL_PixelBuffer
Blitter::rotate_270_flip (CL_PixelBuffer sur)
{
  return BlitterImpl::modify(sur, BlitterImpl::transform_rot270_flip());
}

/* EOF */
