//  $Id: climber.cxx,v 1.16 2002/10/04 11:38:29 torangan Exp $
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

#include "../col_map.hxx"
#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "climber.hxx"

namespace Actions {

Climber::Climber()
{
}

void
Climber::init(void)
{
  sprite = Sprite ("Pingus/climber0", "pingus");

  // these alignments are necessary to prevent climber walking 
  // inside the wall.
  int sprite_height = sprite.get_height();
  int sprite_width  = sprite.get_width();

  if (pingu->direction.is_left ()) 
    {
      sprite.set_align (0, -sprite_height/2);
      sprite.set_direction (Sprite::LEFT); 
    } 
  else 
    {
      sprite.set_align (-sprite_width, -sprite_height/2);
      sprite.set_direction (Sprite::RIGHT);
    }
}

void
Climber::update()
{
  /*
    std::cout << "Climer update()" << std::endl;
    std::cout << "Direction: " << pingu->direction << std::endl;
    printf("%3d %3d %3d\n", rel_getpixel(1,1), rel_getpixel(0,1), rel_getpixel(-1,1));
    printf("%3d %3d %3d\n", rel_getpixel(1,0), rel_getpixel(0,0), rel_getpixel(-1,0));
    printf("%3d %3d %3d\n", rel_getpixel(1,-1), rel_getpixel(0,-1),rel_getpixel(-1, -1));
  */  

  sprite.update();

  // If above is free
  if (rel_getpixel(0, 1) ==  Groundtype::GP_NOTHING
      || rel_getpixel (0, 1) ==  Groundtype::GP_BRIDGE)
    {
      // and there is still ground to walk on
      if (rel_getpixel(1, 1) !=  Groundtype::GP_NOTHING) 
	{
	  pingu->set_pos(pingu->get_x(), 
			 pingu->get_y() - 1);
	  return;
	}
      else if (rel_getpixel(1, 1) ==  Groundtype::GP_NOTHING) 
	{
	  //  std::cout << "Climber failed, no more wall" << std::endl;

	  // If Pingu able to get to new position without head collision
	  if (!head_collision_on_walk(pingu->direction, 1))
	    {
              // Get ready to walk
	      pingu->set_pos(pingu->get_x() + pingu->direction, 
			     pingu->get_y() - 1);
	    }
	  else
	    {
              // Get ready to fall
              pingu->direction.change();
	    }

	  // Finish climbing.
	  pingu->set_action(Actions::Walker);
	}
    }
  else 
    {
      //    std::cout << "Climber failed, falling down" << std::endl;
      pingu->direction.change();
      pingu->set_action(Actions::Walker);
    }
}

void
Climber::draw (GraphicContext& gc)
{
  gc.draw (sprite, pingu->get_pos());
}

bool
Climber::change_allowed(ActionName new_action)
{
  return (new_action == Actions::Floater);
}

} // namespace Actions

/* EOF */
