//  $Id: PinguRandomMap.cc,v 1.3 2000/02/11 16:58:25 grumbel Exp $
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

#include "PinguRandomMap.hh"

PinguRandomMap::PinguRandomMap()
{
  unsigned char* buffer;
  int width;
  int height;
  int y_h = 300;
  need_reload = false;
  sur_provider = new CL_Canvas(6000, 480, 1);

  sur_provider->lock();
  width = sur_provider->get_width();
  height = sur_provider->get_height();

  buffer = static_cast<unsigned char*>(sur_provider->get_data());

  for (int x=0; x < width; x++, y_h += rand() % 3 - 1) {
    for (int y=y_h; y < height; y++) {
      buffer[(x+y*width)*4 + 0] = 255;
      buffer[(x+y*width)*4 + 1] = y*255 / height;
      buffer[(x+y*width)*4 + 2] = y*255 / height;
      buffer[(x+y*width)*4 + 3] = y*255 / height;
    }
  } 
  sur_provider->unlock();
  surface = CL_Surface::create(sur_provider);
  /*
  for (int y=0; y < 100; ++y) {
    for (int x=0; x < 100; ++x) {
      sub_surface[y][x] = 
	CL_SpriteSubsectionProvider::create(sur_provider, x * 16, y * 16, 16, 16);
    }
  }
  */
  //for(int i=0; i < sur_provider->get_pitch() * sur_provider->get_height(); ++i)
  //    buffer[i] = 155;

  
  //  sur_provider->perform_unlock();
  //  tiles_x = 20;
  //  tiles_y = 15;

  // Resize the vector
  /*  surface.resize(tiles_x);
  for(int i=0; i < tiles_x; ++i)
    surface[i].resize(tiles_y);
  */
  // Generate the sub_surfaces
  /*  for(int y=0; y < tiles_y; ++y)
        for(int x=0; x < tiles_x; ++x)
  surface[x][y] = CL_SpriteSubsectionProvider::create(sur_provider,
					    x * 32,
					    y * 32,
					    32,
					    32
					    );
  */
}

PinguRandomMap::~PinguRandomMap()
{
}

void
PinguRandomMap::draw_offset(int of_x, int of_y, float s)
{
  
  if (CL_Keyboard::get_keycode(CL_KEY_U))
    surface->reload();
    //    for(int y=0; y < tiles_y; ++y)
      //      for(int x=0; x < tiles_x; ++x) 
    //	surface[x][y]->reload();

  if (s == 1.0) {
    surface->put_screen(of_x, of_y);
    //    CL_SpriteSubsectionProvider::create(sur_provider, 0, 0, 100, 900)->put_screen(10, 10);
    /*
    for (int y=0; y < 100; ++y) {
      for (int x=0; x < 100; ++x) {
	sub_surface[y][x]->put_screen(x * 16, y * 16);
      }
    }
    */
  } else {
    surface->put_screen((int)(of_x * s), (int)(of_y * s), s, s);
  }
  /*
  for(int y=0; y < tiles_y; ++y)
    for(int x=0; x < tiles_x; ++x) {
      //    surface[x][y]->reload();
      surface[x][y]->put_screen(of_x, of_y);
    }
*/
}

int
PinguRandomMap::get_width()
{
  return sur_provider->get_width();
}

int
PinguRandomMap::get_height()
{
  return sur_provider->get_height();
}

ColMap*
PinguRandomMap::get_colmap()
{
  unsigned char* buffer;
  unsigned char* data;

  std::cout << "Locking surface" << std::endl;
  sur_provider->lock();

  data = static_cast<unsigned char*>(sur_provider->get_data());

  buffer = new unsigned char[sur_provider->get_width() 
			    * sur_provider->get_height()];

  std::cout << "Getting pointer: data: " << (int)data << " buffer: " << (int)buffer << std::endl;

  for(unsigned int i=0; 
      i < sur_provider->get_width() * sur_provider->get_height(); 
      ++i) 
    {
      buffer[i] = data[i*4];
    }
  sur_provider->unlock();
  colmap = new ColMap(buffer, 
		      sur_provider->get_width(),
		      sur_provider->get_height());
  return colmap;
}

void
PinguRandomMap::remove(int x, int y)
{
  /*
  unsigned char* buffer;
  buffer = static_cast<unsigned char*>(sur_provider->get_surface_data());
  buffer[x+y*sur_provider->get_pitch()] = 0;

  //  sur_provider->putpixel(x, y);
  
  need_reload = true;
  surface->reload();
  */
}

/* EOF */
