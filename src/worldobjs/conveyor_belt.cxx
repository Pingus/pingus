//  $Id: conveyor_belt.cxx,v 1.25 2003/10/20 13:11:09 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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
#include "../display/scene_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../resource.hxx"
#include "../world.hxx"
#include "../worldobjsdata/conveyor_belt_data.hxx"
#include "conveyor_belt.hxx"

namespace Pingus {
namespace WorldObjs {

ConveyorBelt::ConveyorBelt (const WorldObjsData::ConveyorBeltData& data_)
  : data(new WorldObjsData::ConveyorBeltData(data_)),
    left_sur  (Resource::load_sprite ("worldobjs/conveyorbelt_left")),
    right_sur (Resource::load_sprite ("worldobjs/conveyorbelt_right")),
    middle_sur(Resource::load_sprite ("worldobjs/conveyorbelt_middle"))
{
}

void
ConveyorBelt::draw (SceneContext& gc)
{
  gc.color().draw(left_sur, data->pos, static_cast<int>(data->counter));
  for (int i=0; i < data->width; ++i)
    gc.color().draw(middle_sur,
	    Vector(static_cast<int>(data->pos.x + left_sur.get_width() + i * middle_sur.get_width()),
                  static_cast<int>(data->pos.y)),
	    static_cast<int>(data->counter));

  gc.color().draw(right_sur,
	  Vector(static_cast<int>(data->pos.x + left_sur.get_width() + data->width * middle_sur.get_width()),
                 static_cast<int>(data->pos.y)),
	  static_cast<int>(data->counter));
}

void
ConveyorBelt::on_startup ()
{
  CL_PixelBuffer sur(Resource::load_pixelbuffer("conveyorbelt_cmap", "worldobjs"));

  for (int i=0; i < (data->width + 2); ++i)
    world->get_colmap()->put(sur,
                             static_cast<int>(data->pos.x) + (15 * i),
			     static_cast<int>(data->pos.y),
			     Groundtype::GP_SOLID);
}

void
ConveyorBelt::update ()
{
  data->counter += data->speed * 0.025f;

  if (data->counter >= 14.0f)
    data->counter = 0.0f;
  else if (data->counter < 0.0f)
    data->counter = middle_sur.get_frame_count() - 1;

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
    {
      if (   (*pingu)->get_x() > data->pos.x
	  && (*pingu)->get_x() < data->pos.x + 15 * (data->width + 2)
	  && (*pingu)->get_y() > data->pos.y - 2
	  && (*pingu)->get_y() < data->pos.y + 10)
	{
	  Vector pos = (*pingu)->get_pos();
	  pos.x -= data->speed * 0.025f;
	  (*pingu)->set_pos(pos);
	}
    }
}

float
ConveyorBelt::get_z_pos () const
{
  return data->pos.z;
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
