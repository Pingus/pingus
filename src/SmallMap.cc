//  $Id: SmallMap.cc,v 1.1 2000/02/25 02:35:27 grumbel Exp $
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

#include "SmallMap.hh"

SmallMap::SmallMap()
{
  sur = 0;
}

SmallMap::~SmallMap()
{
  delete sur;
}
  
void
SmallMap::init()
{
  cout << "SmallMap: init()" << endl;

  CL_Canvas*  canvas;
  ColMap* colmap;
  unsigned char* buffer;
  unsigned char* cbuffer;
  int tx, ty;

  colmap = world->get_colmap(); 
  buffer = colmap->get_data();

  canvas = new CL_Canvas(200, 50);
 
  canvas->lock();
  
  cbuffer = static_cast<unsigned char*>(canvas->get_data());

  /*  for (unsigned int i = 0; i < (200 * 50); i++)
    {
      cbuffer[4*i + 0] = ;
      cbuffer[4*i + 1] = 125;
      cbuffer[4*i + 2] = 125;
      cbuffer[4*i + 3] = 125;
    }
  */
  for(int y = 0; y < 50; y++)
    {
      for (int x = 0; x < 200; x++)
	{
	  tx = x * colmap->get_width() / 200;
	  ty = y * colmap->get_height() / 50;

	  switch(buffer[tx + (ty * colmap->get_width())])
	    {
	    case ColMap::NOTHING:
	      cbuffer[4 * ((y * 200) + x) + 0] = 150;
	      cbuffer[4 * ((y * 200) + x) + 1] = 0;
	      cbuffer[4 * ((y * 200) + x) + 2] = 0;
	      cbuffer[4 * ((y * 200) + x) + 3] = 0;
	      break;
	    default:
	      cbuffer[4 * ((y * 200) + x) + 0] = 255;
	      cbuffer[4 * ((y * 200) + x) + 1] = 255;
	      cbuffer[4 * ((y * 200) + x) + 2] = 255;
	      cbuffer[4 * ((y * 200) + x) + 3] = 255;
	      break;
	    }
	}
    }

  canvas->unlock();
  
  sur = CL_Surface::create(canvas, true);

  cout << "SmallMap::setWorld" << endl;
}

void
SmallMap::set_world(World* w)
{
  world = w;
  init();
}

void
SmallMap::draw()
{
  cout << "Drawing smallmap" << endl;
  sur->put_screen(0, CL_Display::get_height() - sur->get_height()); 
}

void
SmallMap::let_move()
{
}

/* EOF */

