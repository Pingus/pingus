//  $Id: climber.cc,v 1.11 2001/03/31 11:21:51 grumbel Exp $
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

#include "../PingusResource.hh"
#include "climber.hh"

bool Climber::static_surfaces_loaded = false;
CL_Surface Climber::static_surface;

Climber::Climber()
{
}

void
Climber::init(void)
{
  action_name = "climber";
  environment = PinguEnvironment(sky | land);

  if (!static_surfaces_loaded)
    {
      static_surface = PingusResource::load_surface ("Pingus/climber", "pingus");
      static_surfaces_loaded = true;
    }

  surface = static_surface;
  counter.set_size(surface.get_num_frames()/2);
  counter.set_type(Counter::loop);
  counter.set_count(0);
  counter.set_speed(10);
  is_multi_direct = true;
}

int 
Climber::x_offset(void)
{
  if (pingu->direction.is_left())
    return 0;
  else 
    return -32;
}

int 
Climber::y_offset(void)
{
  return -16;
}

void
Climber::update(void)
{
  /*
    std::cout << "Climer update()" << std::endl;
    std::cout << "Direction: " << pingu->direction << std::endl;
    printf("%3d %3d %3d\n", rel_getpixel(1,1), rel_getpixel(0,1), rel_getpixel(-1,1));
    printf("%3d %3d %3d\n", rel_getpixel(1,0), rel_getpixel(0,0), rel_getpixel(-1,0));
    printf("%3d %3d %3d\n", rel_getpixel(1,-1), rel_getpixel(0,-1),rel_getpixel(-1, -1));
  */  

  // If above is free
  if (rel_getpixel(0, 1) == ColMap::NOTHING
      || rel_getpixel (0, 1) & ColMap::BRIDGE)
    {
      // and there is still ground to walk on
      if (rel_getpixel(1, 1) & ColMap::WALL) 
	{
	  --pingu->pos.y;
	  return;
	}
      else if (rel_getpixel(1, 1) == ColMap::NOTHING) 
	{
	  //  std::cout << "Climber failed, falling down" << std::endl;
	  --pingu->pos.y;
	  pingu->pos.x += pingu->direction;
	  is_finished = true;
	}
    }
  else 
    {
      //    std::cout << "Climber failed, falling down" << std::endl;
      pingu->direction.change();
      is_finished = true;
    }
}
/*
void
Climber::draw_offset(int x, int y, double s=1.0)
{
  if (pingu->status == (dead || exited))
    return;

  assert(surface);
  
  if (pingu->direction.is_left()) {
    surface->put_screen(int((pingu->x_pos + x) * s), int((pingu->y_pos + y -16) * s), 
                        int(surface->get_width() * s), int(surface->get_height() * s),
			++counter + ((pingu->direction.is_left()) ? 0 : counter.size()));
  } else {
    surface->put_screen(int((pingu->x_pos + x - 32) * s), int((pingu->y_pos + y -16) * s),
                        int(surface->get_width() * s), int(surface->get_height() * s),
                        ++counter + ((pingu->direction.is_left()) ? 0 : counter.size()));
  }
}
*/
/* EOF */
