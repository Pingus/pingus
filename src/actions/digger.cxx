//  $Id: digger.cxx,v 1.26 2003/10/21 11:01:52 grumbel Exp $
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
#include "../gui/graphic_context.hxx"
#include "../pingu_map.hxx"
#include "../world.hxx"
#include "../resource.hxx"
#include "../sound/sound.hxx"
#include "../pingu.hxx"
#include "digger.hxx"

namespace Pingus {
namespace Actions {

Digger::Digger (Pingu* p)
  : PinguAction(p),
    digger_radius(Resource::load_pixelbuffer("other/digger_radius")),
    digger_radius_gfx(Resource::load_pixelbuffer("other/digger_radius")),
    digger_c(0)
{
  digger_radius_width  = digger_radius.get_width();
  digger_radius_height = digger_radius.get_height();

  digger_radius_gfx_width  = digger_radius_gfx.get_width();
  digger_radius_gfx_height = digger_radius_gfx.get_height();

  sprite = Resource::load_sprite("pingus/digger/left");
}

void
Digger::update ()
{
  sprite.update ();

  if (rel_getpixel(0, -1) ==  Groundtype::GP_WATER
      || rel_getpixel(0, -1) ==  Groundtype::GP_LAVA)
    {
      pingu->set_action(Actions::Drown);
      return;
    }

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
	  Sound::PingusSound::play_sound("chink");
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
					      static_cast<int>(pingu->get_x() - (digger_radius_width / 2)),
					      static_cast<int>(pingu->get_y() - digger_radius_height + 2));
  WorldObj::get_world()->get_gfx_map()->remove(digger_radius_gfx,
					       static_cast<int>(pingu->get_x () - (digger_radius_gfx_width / 2)),
					       static_cast<int>(pingu->get_y() - digger_radius_gfx_height + 2));
  pingu->set_y(pingu->get_y() + 1);
}

void
Digger::draw (GraphicContext& gc)
{
  gc.draw(sprite, pingu->get_pos ());
}

} // namespace Actions
} // namespace Pingus

/* EOF */
