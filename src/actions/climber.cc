//  $Id: climber.cc,v 1.16 2001/08/07 19:55:22 grumbel Exp $
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

#include "../ColMap.hh"
#include "../PingusResource.hh"
#include "climber.hh"

Climber::Climber()
{
}

void
Climber::init(void)
{
  sprite = Sprite ("Pingus/climber0", "pingus");
}

void
Climber::update(float delta)
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

void
Climber::draw_offset(int x, int y, float s)
{
  sprite.put_screen (pingu->get_pos () + CL_Vector (x, y));
}

/* EOF */
