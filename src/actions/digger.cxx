//  $Id: digger.cxx,v 1.17 2002/10/13 16:40:01 grumbel Exp $
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
#include "../graphic_context.hxx"
#include "../pingu_map.hxx"
#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../sound.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "digger.hxx"

namespace Actions {

Digger::Digger (Pingu* p)
  : PinguAction(p),
    digger_c(0)
{
  digger_radius = PingusResource::load_surface ("Other/digger_radius", "pingus");
  digger_radius_gfx = PingusResource::load_surface ("Other/digger_radius", "pingus");
  sprite = Sprite (std::string("Pingus/digger") + to_string(pingu->get_owner ()),
		   "pingus");
#if 0
  // FIXME: Just an idea...
  sprite = Sprite (std::string("Pingus/digger") + to_string(pingu->get_owner ()),
		   "pingus", PropertyMgr::instance()->get_int ("actions/digger/sprite-fps", 20));
#endif
  sprite.set_align_center_bottom ();
}

void
Digger::update ()
{
  sprite.update ();
  
  if (++digger_c >= 5)
    {
      digger_c = 0;
      dig();
    }

  if (!have_something_to_dig())
    { 
      dig ();
      pingu->set_action(Actions::Walker);
    }
}

bool   
Digger::have_something_to_dig ()
{
  if (rel_getpixel(0, -1) !=  Groundtype::GP_NOTHING)
    {
      if (rel_getpixel(0, -1) ==  Groundtype::GP_SOLID)
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
Digger::dig ()
{
  WorldObj::get_world()->get_colmap()->remove(digger_radius, 
					      static_cast<int>(pingu->get_x () - 16),
					      static_cast<int>(pingu->get_y() - 14));
  WorldObj::get_world()->get_gfx_map()->remove(digger_radius_gfx,
					       static_cast<int>(pingu->get_x () - 16),
					       static_cast<int>(pingu->get_y() - 14));
      
  pingu->set_y(pingu->get_y() + 1);
}

void  
Digger::draw (GraphicContext& gc)
{
  gc.draw(sprite, pingu->get_pos ());
}

} // namespace Actions

/* EOF */
