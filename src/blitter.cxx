//  $Id: blitter.cxx,v 1.22 2002/12/29 23:29:00 torangan Exp $
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

#include <stdio.h>
#include "pingus_error.hxx"
#include "color.hxx"
#include "string_converter.hxx"
#include "globals.hxx"
#include "math.hxx"
#include "blitter.hxx"
#include "debug.hxx"
#include "blitter_impl.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"

#define COMPILE_WITH_MEMORY_HOLE 0

void
Blitter::put_surface(CL_Canvas* canvas, const CL_Surface& sur,
		     int x, int y)
{
  //Blitter::put_surface(canvas, sur->get_provider(), x, y);
  /*  if (sur->get_provider()->get_depth() != 8)
      sur->put_target(x, y, 0, canvas);
      else*/
  assert (sur);
  assert (canvas);
  Blitter::put_surface(canvas, sur.get_provider(), x, y);
}

void 
Blitter::put_surface(CL_Canvas* canvas, CL_SurfaceProvider* provider,
		     int x, int y)
{
  assert (provider);
  assert (canvas);
  switch(provider->get_depth())
    {
    case  8:
      put_surface_8bit(canvas, provider, x, y);
      break;
    case 32:
      put_surface_32bit(canvas, provider, x, y);
      break;
    default: 
      PingusError::raise("Blitter:put_surface:Unknown color depth: " + to_string(provider->get_depth()));
      break;
    }    
}

void
Blitter::put_surface_8bit(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
			  int x, int y)
{
  assert (provider);
  assert (sprovider);
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;
  
  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  int x_offset, y_offset;

  provider->lock();
  sprovider->lock();

  tbuffer = static_cast<unsigned char*>(provider->get_data());
  sbuffer = static_cast<unsigned char*>(sprovider->get_data());

  //std::cout << "Colorkey: " << sprovider->get_src_colorkey() << std::endl;

  CL_Palette* cl_palette = sprovider->get_palette();

  if (!cl_palette)
    {
      char str[128];
      snprintf(str, 128, _("Couldn't find palette: %d"), sprovider->get_depth());
      PingusError::raise(str);
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

  unsigned char* palette = cl_palette->palette;
 
  if (sprovider->uses_src_colorkey ())
    {
      unsigned int colorkey = sprovider->get_src_colorkey();
      
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
		  tbuffer[i + 1] = palette[sbuffer[j] * 3 + 2]; // blue
		  tbuffer[i + 2] = palette[sbuffer[j] * 3 + 1]; // green
		  tbuffer[i + 3] = palette[sbuffer[j] * 3 + 0]; // red
		}
	    }
	}
    }
  else
    {
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
	      tbuffer[i + 1] = palette[sbuffer[j] * 3 + 2]; // blue
	      tbuffer[i + 2] = palette[sbuffer[j] * 3 + 1]; // green
	      tbuffer[i + 3] = palette[sbuffer[j] * 3 + 0]; // red
	    }
	}
    }

#if COMPILE_WITH_MEMORY_HOLE
#warning "FIXME: Blitter::put_surface_8bit(CL_Canvas* provider, CL_SurfaceProvider* sprovider, int x, int y) contains memory hole"
#else 
  sprovider->unlock();
  provider->unlock();  
#endif
}

void
Blitter::put_surface_32bit(CL_Canvas* canvas, CL_SurfaceProvider* provider,
			   const int x_pos, const int y_pos)
{
  assert (canvas);
  assert (provider);

  pout(PINGUS_DEBUG_BLITTER) << "Blitting: SurfaceProvider:" << provider->get_width () 
                             << "x" << provider->get_height () << ":" << provider 
                	     << " Canvas:" << canvas->get_width () << "x" 
                             << canvas->get_height () << ":" << canvas << std::endl;

  float red, green, blue, alpha;
  float tred, tgreen, tblue, talpha;

  int swidth = provider->get_width();
  int sheight = provider->get_height();

  int twidth = canvas->get_width();
  int theight = canvas->get_height();

  // Surface is out of the screen
  if (x_pos > twidth-1 || y_pos > theight-1)
    return;

  canvas->lock();
  provider->lock();
  if (1) // slow
    {
      for(int y = Math::max(0, -y_pos); y < sheight && (y + y_pos) < theight; ++y) 
	for(int x = Math::max(0,-x_pos); x < swidth && (x + x_pos) < twidth; ++x) 
	  {
	    provider->get_pixel(x, y, &red, &green, &blue, &alpha);
	    canvas->get_pixel(x + x_pos, y + y_pos, &tred, &tgreen, &tblue, &talpha);
	  
	    // FIXME: This doesn't give correct alpha values
	    canvas->draw_pixel(x + x_pos, y + y_pos, 
			       Math::mid(0.0, 1.0, (red * alpha) + (tred * (1.0-alpha))),
			       Math::mid(0.0, 1.0, (green * alpha) +(tgreen * (1.0-alpha))),
			       Math::mid(0.0, 1.0, (blue * alpha)  + (tblue * (1.0-alpha))),
			       Math::mid(0.0, 1.0, alpha * alpha + (talpha*(1.0-alpha))));
	  }
    }
  else // fast?!
    {
      
    }

#if COMPILE_WITH_MEMORY_HOLE
#warning "FIXME: Blitter::put_surface_32bit(CL_Canvas* canvas, CL_SurfaceProvider* provider, const int x_pos, const int y_pos) contains memory hole"
#else
  provider->unlock();
  canvas->unlock();
#endif
}

void
Blitter::put_alpha_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider,
			   int x, int y)
{
  assert (provider);
  assert (sprovider);
  int start_i;
  unsigned char* tbuffer; // Target buffer
  int twidth, theight, tpitch;
  
  unsigned char* sbuffer; // Source buffer
  int swidth, sheight, spitch;

  CL_Palette* palette;
  int x_offset, y_offset;

  provider->lock();
  sprovider->lock();

  //  assert(sprovider->get_depth() == 8);
  if (sprovider->get_depth() != 8)
    {
      // FIXME: memory hole
#if COMPILE_WITH_MEMORY_HOLE
#warning "FIXME: Blitter::put_alpha_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider, int x, int y) contains memory hole"
#else
      sprovider->unlock ();
      provider->unlock ();
#endif
      PingusError::raise("Image has wrong color depth: " + to_string(sprovider->get_depth()));
    }
  //  assert(provider->get_pixel_format() == RGBA8888);

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

#if COMPILE_WITH_MEMORY_HOLE
#warning "FIXME: Blitter::put_alpha_surface(CL_Canvas* provider, CL_SurfaceProvider* sprovider, int x, int y) contains memory hole"
#else
  sprovider->unlock();
  provider->unlock();  
#endif
}

CL_Canvas*
Blitter::clear_canvas(CL_Canvas* canvas)
{
  assert (canvas);
  unsigned char* buffer;
  
  canvas->lock();
  buffer = static_cast<unsigned char*>(canvas->get_data());
  memset(buffer, 0, sizeof(unsigned char) * canvas->get_pitch() * canvas->get_height());
  // FIXME: memory hole
  //canvas->unlock();

  return canvas;
}

CL_Canvas*
Blitter::create_canvas(const CL_Surface& sur)
{
  assert (sur);
  return create_canvas(sur.get_provider());
}

CL_Canvas*
Blitter::create_canvas(CL_SurfaceProvider* prov)
{
  assert (prov);
  CL_Canvas* canvas = new CL_Canvas(prov->get_width(), prov->get_height());

  switch (prov->get_bytes_per_pixel())
    {
      // RGB888
    case 3:
      {
	canvas->lock();
	prov->lock();
	
	int buffer_size = prov->get_pitch () * prov->get_height ();
	unsigned char* sbuffer = static_cast<unsigned char*>(prov->get_data ());
	unsigned char* tbuffer = static_cast<unsigned char*>(canvas->get_data ());
	
	for (int si = 0, ti = 0; si < buffer_size; si += 3, ti += 4)
	  {
	    tbuffer[ti + 0] = 255; // Alpha
	    tbuffer[ti + 1] = sbuffer[si + 0];
	    tbuffer[ti + 2] = sbuffer[si + 1];
	    tbuffer[ti + 3] = sbuffer[si + 2];
	  }
	  
	// -FIXME: memory hole
	prov->unlock();
	canvas->unlock();
      }
      break;

      // RGBA8888
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

CL_Surface 
Blitter::scale_surface (const CL_Surface& sur, int width, int height)
{
  assert (sur);
  return CL_Surface(Blitter::scale_surface_to_canvas(sur, width, height), true);
}

CL_Canvas* 
Blitter::scale_surface_to_canvas (const CL_Surface& sur, int width, int height)
{
  assert (sur);

  Color color;
  CL_SurfaceProvider* provider = sur.get_provider ();
  CL_Canvas* canvas = new CL_Canvas (width, height);

  provider->lock ();
  canvas->lock ();

  unsigned char* sbuffer = static_cast<unsigned char*>(provider->get_data ());
  unsigned char* tbuffer = static_cast<unsigned char*>(canvas->get_data ());
  int pwidth = provider->get_width ();
  int pheight = provider->get_height ();

  if (provider->is_indexed ())
    {
      // Slow but generic, using get_data () would be better, but would
      // require quite a bit of work
      for (int y = 0; y < height; ++y)
	{
	  for (int x = 0; x < width; ++x)
	    {
	      unsigned char pixel = *(static_cast<unsigned char*>(provider->get_data ()) 
				      + (y * pheight/height) * provider->get_pitch() + (x * pwidth/width));

	      color.red   = provider->get_palette()->palette[pixel*3 +0] / 255.0f;
	      color.green = provider->get_palette()->palette[pixel*3 +1] / 255.0f;
	      color.blue  = provider->get_palette()->palette[pixel*3 +2] / 255.0f;

	      if (provider->get_src_colorkey () == pixel)
		color.alpha = 0.0f;
	      else
		color.alpha = 1.0f;

	      // FIXME: ignoring the source alpha due to get_pixel brokeness... no time to test the patch
	      canvas->draw_pixel (x, y, color.red, color.green, color.blue, color.alpha);
	    }
	}
    }
  else
    {
      switch (provider->get_bytes_per_pixel ())
	{
	case 3:
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
	case 4:
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
		provider->get_pixel (x * provider->get_width () / width,
				     y * provider->get_height () / height,
				     &color.red, &color.green, &color.blue, &color.alpha);
		// FIXME: ignoring the source alpha due to get_pixel brokeness... no time to test the patch
		canvas->draw_pixel (x, y, color.red, color.green, color.blue, color.alpha);
	      }
	  break;
	}
    }

#if COMPILE_WITH_MEMORY_HOLE
#warning "FIXME: Blitter::scale_surface_to_canvas (const CL_Surface& sur, int width, int height) contains memory leak"
#else
  canvas->unlock ();
  provider->unlock ();
#endif

  return canvas;
}

/*
// Converts a SurfaceProvider based surface, to a Canvas
// based one. The old one will not be deleted.
CL_Surface
Blitter::convert_to_emptyprovider(CL_Surface ssurf)
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

/** Flip a surface horizontal */
CL_Surface
Blitter::flip_horizontal (const CL_Surface& sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_flip>(sur);
}

/** Flip a surface vertical */
CL_Surface
Blitter::flip_vertical (const CL_Surface& sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot180_flip>(sur);
}

/** Rotate a surface 90 degrees */
CL_Surface
Blitter::rotate_90 (const CL_Surface& sur)
{
  CL_SurfaceProvider* prov = sur.get_provider ();

  if (prov->is_indexed())
    {
      std::cout << "Using indexed blitter" << std::endl;
      int pwidth  = prov->get_width();
      int pheight = prov->get_height();

      IndexedCanvas* canvas = new IndexedCanvas(pheight, pwidth);
      if (prov->uses_src_colorkey())
        canvas->set_src_colorkey(prov->get_src_colorkey());
      
      prov->lock ();
      canvas->lock ();

      canvas->set_palette(prov->get_palette());

      unsigned char* source_buf = static_cast<unsigned char*>(prov->get_data());
      unsigned char* target_buf = static_cast<unsigned char*>(canvas->get_data());

      for (int y = 0; y < pheight; ++y)
        for (int x = 0; x < pwidth; ++x)
          {
            target_buf[x * pheight + (pheight - y - 1)] = source_buf[y * pwidth + x];
          }

      canvas->unlock ();
      prov->unlock ();
      return CL_Surface(canvas, true);     
    }
  else
    {
      CL_Canvas* canvas = new CL_Canvas (sur.get_height (), sur.get_width ());

      prov->lock ();
      canvas->lock ();

      float r, b, g, a;
      for (unsigned int y = 0; y < sur.get_height (); ++y)
        for (unsigned int x = 0; x < sur.get_width (); ++x)
          {
            prov->get_pixel (x, y, &r, &g, &b, &a);
            canvas->draw_pixel (sur.get_height () - 1 - y, x , r, g, b, a);
          }

      canvas->unlock ();
      prov->unlock ();
      return CL_Surface(canvas, true);
    }
}


CL_Surface
Blitter::rotate_180 (const CL_Surface& sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot180>(sur);
}

CL_Surface
Blitter::rotate_270 (const CL_Surface& sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot270>(sur);
}

CL_Surface
Blitter::rotate_90_flip (const CL_Surface& sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot90_flip>(sur);
  //return Blitter::flip_horizontal(Blitter::rotate_90(sur));
}

CL_Surface
Blitter::rotate_180_flip (const CL_Surface& sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot180_flip>(sur);
  //return Blitter::flip_horizontal(Blitter::rotate_180(sur));
}

CL_Surface
Blitter::rotate_270_flip (const CL_Surface& sur)
{
  return BlitterImpl::modify<BlitterImpl::transform_rot270_flip>(sur);
  //return Blitter::flip_horizontal(Blitter::rotate_270(sur));
}

/* EOF */
