//  $Id: digger.cxx,v 1.5 2002/06/28 15:12:22 torangan Exp $
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

#include <cstdio>

#include "../col_map.hxx"
#include "../pingu_map.hxx"
#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../sound.hxx"
#include "../particles/ground_particle.hxx"
#include "../algo.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "digger.hxx"

Digger::Digger() : digger_c(0)
{
}

void
Digger::init(void)
{
  digger_radius = PingusResource::load_surface ("Other/digger_radius", "pingus");
  digger_radius_gfx = PingusResource::load_surface ("Other/digger_radius_gfx", "pingus");
  sprite = Sprite (std::string("Pingus/digger") + to_string(pingu->get_owner ()),
		   "pingus");
  sprite.set_align_center_bottom ();
}

void
Digger::update(float delta)
{
  sprite.update (delta);
  
  if (++digger_c >= 3)
    {
      digger_c = 0;
      dig();
    }

  if (!have_something_to_dig())
    { 
      dig ();
      pingu->set_action(Walker);
    }
}

bool   
Digger::have_something_to_dig()
{
  if (rel_getpixel(0, -1) !=  GroundpieceData::GP_NOTHING)
    {
      if (rel_getpixel(0, -1) ==  GroundpieceData::GP_SOLID)
	{
	  PingusSound::play_sound("sounds/chink.wav");
	  return false;  
	}
      else
	return true;
    }
  else
    {
      return false;
    }
}

void
Digger::dig()
{
  pingu->get_world()->get_colmap()->remove(digger_radius, pingu->get_x () - 16, pingu->get_y() - 14);
  pingu->get_world()->get_gfx_map()->remove(digger_radius_gfx, pingu->get_x () - 16, pingu->get_y() - 14);
      
  ++pingu->pos.y;
}

void  
Digger::draw_offset(int x, int y, float /*s*/)
{
  //std::cout << "DRawing digger" << std::endl;
  sprite.put_screen (int(pingu->pos.x + x), int(pingu->pos.y + y));
}

/* EOF */
