//  $Id: PinguBmpMap.cc,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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

#include "PinguBmpMap.hh"
#include "globals.hh"
#include "PingusError.hh"
#include "PingusResource.hh"

PinguBmpMap::PinguBmpMap()
{
  surface = 0;
  colmap  = 0;
}

PinguBmpMap::PinguBmpMap(ResDescriptor fg)
{
  colmap = 0;
  if (verbose > 1)
    cout << "Creating BMP Map" << endl;

  surface = CL_Surface::load(fg.res_name.c_str(), PingusResource::get(fg.filename));
  
  if (!surface) {
    throw PingusError(fg.res_name + ": Could not open file\n");;
  }
}

PinguBmpMap::~PinguBmpMap()
{
  /* BUG: Segfault
  if (surface)
    delete surface;
  */
}

int
PinguBmpMap::get_width()
{
  return surface->get_width();
}

int
PinguBmpMap::get_height()
{
  return surface->get_height();  
}

void
PinguBmpMap::draw_offset(int x, int y, float s)
{
  if (s == 1.0)
    surface->put_screen(x,y);	  
  else
    surface->put_screen((int)(x * s), (int)(y * s),s , s);
}

ColMap*
PinguBmpMap::get_colmap()
{
  unsigned char* buffer;
  void* vbuffer;
  CL_SurfaceProvider* provider;

  cout << "PinguBmpMap::get_colmap" << endl;

  assert(surface);

  if (colmap) {
    return colmap;
  } else {
    provider = surface->get_provider();
    assert(provider);
    provider->lock();

    vbuffer = provider->get_data();
    assert(vbuffer);
    if (provider->get_depth() != 8)
      throw PingusError("PingusBmpMap::get_colmap: Surface has wrong pixel format, need 8bpp!");
    
    buffer = new unsigned char [provider->get_pitch() * provider->get_height()];
    memcpy(buffer, vbuffer, provider->get_pitch() *  provider->get_height());

    colmap = new ColMap();
    colmap->load(buffer, get_width(), get_height());

    provider->unlock();
    return colmap;
  }

  //  return 0;
}

CL_Surface*
PinguBmpMap::get_surface(void)
{
  return surface;
}

/* EOF */
