//  $Id: smasher.cxx,v 1.23 2003/10/21 21:37:06 grumbel Exp $
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

#include <iostream>
#include "../math.hxx"
#include "../col_map.hxx"
#include "../gui/graphic_context.hxx"
#include "../particles/smoke_particle_holder.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../pingus_resource.hxx"
#include "../sound/sound.hxx"
#include "../world.hxx"
#include "../worldobjsdata/smasher_data.hxx"
#include "smasher.hxx"

namespace Pingus {
namespace WorldObjs {

Smasher::Smasher (const WorldObjsData::SmasherData& data_)
  : data (new WorldObjsData::SmasherData(data_)),
    smashing(false),
    downwards(false),
    count(0)
{
}

Smasher::~Smasher ()
{
  delete data;
}

float
Smasher::get_z_pos () const
{
  return data->pos.z;
}

void
Smasher::update ()
{

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    catch_pingu(*pingu);

  if (smashing)
    {
      if (downwards)
	      {
	        if (count >= 5)
	          {
	            // SMASH!!! The thing hitten earth and kills the pingus
	            downwards = false;
	            --count;
	            Sound::PingusSound::play_sound("sounds/tenton.wav", 0.7f);

	            for(int i=0; i < 20; ++i)
		            {
		              world->get_smoke_particle_holder()->
                                add_particle(static_cast<int>(data->pos.x + 20 + rand() % 260),
                                             static_cast<int>(data->pos.y + 180),
                                             Math::frand()-0.5f, Math::frand()-0.5f);
		            }

	            for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
		            {
		              if ((*pingu)->is_inside(static_cast<int>(data->pos.x + 30),
					              static_cast<int>(data->pos.y + 90),
					              static_cast<int>(data->pos.x + 250),
					              static_cast<int>(data->pos.y + 190)))
		                {
		                  if ((*pingu)->get_action() != Actions::Splashed)
			            (*pingu)->set_action(Actions::Splashed);
		                }
		            }
	          }
	        else
	          {
	            ++count;
	          }
	      }
      else
	      {
	        if (count <= 0)
		        {
	            count = 0;
	            smashing = false;
	          } else {
	            --count;
	          }
	      }
    }
}

void
Smasher::on_startup ()
{
  std::cout << "Drawing colmap entry" << std::endl;
  CL_PixelBuffer buf = PingusResource::load_pixelbuffer("Traps/smasher_cmap", "traps");
  world->get_colmap()->put(buf, 
			   static_cast<int>(data->pos.x),
			   static_cast<int>(data->pos.y),
			   Groundtype::GP_SOLID);
}

void
Smasher::draw (GraphicContext& gc)
{
  gc.draw (data->surface, data->pos, count);
}

void
Smasher::catch_pingu (Pingu* pingu)
{
  // Activate the smasher if a Pingu is under it
  if ((   pingu->direction.is_left()
	  && pingu->get_x() > data->pos.x + 65
	  && pingu->get_x() < data->pos.x + 85)
      ||
      (   pingu->direction.is_right()
	  && pingu->get_x() > data->pos.x + 190
	  && pingu->get_x() < data->pos.x + 210))
    {
      if (pingu->get_action() != Actions::Splashed)
	{
	  if (!smashing)
	    {
	      count = 0;
	      downwards = true;
	      smashing = true;
	    }
	}
    }
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
