//  $Id: hammer.cxx,v 1.9 2002/10/04 13:46:56 grumbel Exp $
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

#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../pingu_action.hxx"
#include "../worldobjsdata/hammer_data.hxx"
#include "hammer.hxx"

namespace WorldObjs {

Hammer::Hammer (const WorldObjsData::HammerData& data_) 
  : data(new WorldObjsData::HammerData(data_))
{
  sprite = Sprite("Traps/hammer", "traps");
}

Hammer::~Hammer ()
{
  delete data;
}

float
Hammer::get_z_pos () const
{
  return data->pos.z;
}

void 
Hammer::draw (GraphicContext& gc)
{
  gc.draw (sprite, data->pos);
}

void
Hammer::update ()
{
  sprite.update();

  if (sprite.finished())
    {
      PinguHolder* holder = world->get_pingus();

      for (PinguIter pingu_it = holder->begin (); pingu_it != holder->end (); ++pingu_it) 
	{
	  Pingu* pingu = *pingu_it;
	  if (pingu->get_action()->get_type() != Actions::Splashed)
	    {
	      if (pingu->get_x() > data->pos.x + 55  && pingu->get_x() < data->pos.x + 77
		    && pingu->get_y() > data->pos.y + 146 && pingu->get_y() < data->pos.y + 185)
		  pingu->set_action(Actions::Splashed);
	    }
	}
      sprite.reset();
    }
}

} // namespace WorldObjs

/* EOF */
