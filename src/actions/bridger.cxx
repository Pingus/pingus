//  $Id: bridger.cxx,v 1.11 2002/08/25 09:08:49 torangan Exp $
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

#include <cassert>
#include "../pingu_map.hxx"
#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../sound.hxx"
#include "../algo.hxx"
#include "../col_map.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "bridger.hxx"

using namespace std;

namespace Actions {

  bool Bridger::static_surfaces_loaded = false;
  CL_Surface Bridger::brick_l;
  CL_Surface Bridger::brick_r;
  CL_Surface Bridger::static_surface;

  Bridger::Bridger() : mode(B_BUILDING),
                       bricks(MAX_BRICKS),
		       block_build(false),
		       name("Bridger (" + to_string(bricks) + ")")

  {
  }

  void
  Bridger::init(void)
  {
    if (!static_surfaces_loaded)
      {
        static_surface = PingusResource::load_surface ("Pingus/bridger0", "pingus");
        brick_l = PingusResource::load_surface ("Other/brick_left", "pingus");
        brick_r = PingusResource::load_surface ("Other/brick_right", "pingus");
        static_surfaces_loaded = true;
      }
    build_sprite = Sprite ("Pingus/bridger_build" + to_string(pingu->get_owner ()),
			   "pingus", 15.0f, Sprite::NONE, Sprite::ONCE);
    walk_sprite = Sprite ("Pingus/bridger_walk" + to_string(pingu->get_owner ()),
			  "pingus", 15.0f, Sprite::NONE, Sprite::ONCE);
    build_sprite.set_align_center_bottom ();
    walk_sprite.set_align_center_bottom ();

    last_pos = pingu->pos;
  }

  void
  Bridger::draw_offset(int x, int y, float s)
  {

    int x_offset(6), y_offset(4);

    if (bricks == MAX_BRICKS) {
      x_offset = -4;
      y_offset = 0;
    
    } else if (bricks == MAX_BRICKS - 1) {
      x_offset = 0;
      y_offset = 1;
    } else if (bricks == MAX_BRICKS - 2) {
      x_offset = 3;
      y_offset = 2;
    }

    switch (mode)
      {
      case B_BUILDING:
        if (pingu->direction.is_left ())
	  build_sprite.set_direction (Sprite::LEFT);
        else
	  build_sprite.set_direction (Sprite::RIGHT);
      
        build_sprite.put_screen(pingu->get_x () + x - (x_offset * pingu->direction),
			        pingu->get_y () + y + y_offset);
        break;
      
      case B_WALKING:
        if (pingu->direction.is_left ())
	  walk_sprite.set_direction (Sprite::LEFT);
        else
	  walk_sprite.set_direction (Sprite::RIGHT);
      
        walk_sprite.put_screen(pingu->get_x () + x - (x_offset * pingu->direction),
			        pingu->get_y () + y + y_offset);
        break;
      }
      
    UNUSED_ARG(s);
  }

  void
  Bridger::update(float delta)
  {
    switch (mode)
      {
      case B_BUILDING:
        update_build (delta);
        break;

      case B_WALKING:
        update_walk (delta);
        break;
      }

    last_pos = pingu->pos;
  }

  void
  Bridger::update_walk (float delta)
  {
    if (walk_sprite.finished ())
      {
        if (way_is_free())
	   {
             mode = B_BUILDING;
             block_build = false;
             walk_sprite.reset ();
             walk_one_step_up();
	   }
        else // We reached a wall...
	   {
	     pingu->direction.change ();
	     pingu->set_action (Actions::Walker);
	     pingu->pos = last_pos;
	     return;
	   }
      }
    else
      {
        walk_sprite.update (delta);
      }
  }

  void
  Bridger::update_build (float delta)
  {
    build_sprite.update (delta);

    if (build_sprite.get_frame () >= 7 && !block_build)
      {
        block_build = true;

        if (bricks > 0)
	  {
            place_a_brick();
          }
        else // Out of bricks
	  {
	    pingu->set_action(Actions::Waiter);
	  }     
      }

    if (build_sprite.finished ())
      {
        mode = B_WALKING;
        build_sprite.reset ();
      }
  }

  bool
  Bridger::way_is_free()
  {
    return (rel_getpixel(4,2) ==  GroundpieceData::GP_NOTHING)
      && !head_collision_on_walk(4, 2)
      && !head_collision_on_walk(8, 4);
  }

  void
  Bridger::place_a_brick()
  {
    bricks--;
    name = "Bridger (" + to_string(bricks) + ")";

    if (bricks < 4)
      PingusSound::play_sound("sounds/ting.wav");
 
    if (pingu->direction.is_right())
      {
        pingu->get_world()->get_colmap()->put(brick_r, 
					      pingu->get_x () + 10 - brick_r.get_width(),
					      pingu->get_y (),
					      GroundpieceData::GP_BRIDGE);
        pingu->get_world()->get_gfx_map()->put(brick_r,
					       (int) pingu->get_x () + 10 - brick_r.get_width(),
					       pingu->get_y ());
      }
    else
      {
        pingu->get_world()->get_colmap()->put(brick_r, pingu->get_x () - 10,
					      pingu->get_y (),
					      GroundpieceData::GP_BRIDGE);
        pingu->get_world()->get_gfx_map()->put(brick_l,
					       pingu->get_x () - 10,
					       pingu->get_y ());
      }
  }

  void
  Bridger::walk_one_step_up()
  {
    pingu->pos.x += 4.0 * pingu->direction;
    pingu->pos.y -= 2.0;
    counter = 0;
  }

  std::string
  Bridger::get_name () const
  { 
    return name;
  }

}

/* EOF */
