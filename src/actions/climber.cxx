//  $Id: climber.cxx,v 1.1 2002/06/12 19:01:42 grumbel Exp $
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

#include "../pingu.hxx"
#include "../col_map.hxx"
#include "../pingus_resource.hxx"
#include "climber.hxx"

Climber::Climber()
{
}

void
Climber::init(void)
{
  sprite = Sprite ("Pingus/climber0", "pingus");

  // these alignments are necessary to prevent climber walking 
  // inside the wall.
  sprite_height = sprite.get_height();
  sprite_width = sprite.get_width();
  if (pingu->direction.is_left ()) {
      sprite.set_align (0, -sprite_height/2);
      sprite.set_direction (Sprite::LEFT); 
  } else {
      sprite.set_align (-sprite_width, -sprite_height/2);
      sprite.set_direction (Sprite::RIGHT);
  }
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

  sprite.update(delta);

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
	  //  std::cout << "Climber failed, no more wall" << std::endl;

          // If Pingu able to get to new position without head collision
          if (!head_collision_on_walk(pingu->direction, 1))
            {
            // Get ready to walk
  	  --pingu->pos.y;
	    pingu->pos.x += pingu->direction;
            }
          else
            {
            // Get ready to fall
            pingu->direction.change();
            }

          // Finish climbing.
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
Climber::draw_offset(int x, int y, float /*s*/)
{
  sprite.put_screen (pingu->get_pos () + CL_Vector (x, y));
}

/* EOF */
