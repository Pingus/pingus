//  $Id: info_box.cxx,v 1.12 2002/09/14 13:35:38 torangan Exp $
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

#include <fstream>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Display/display.h>
#include "../graphic_context.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../pingu.hxx"
#include "../worldobjsdata/info_box_data.hxx"
#include "info_box.hxx"

namespace WorldObjs {

InfoBox::InfoBox (WorldObjsData::InfoBoxData* data_)
  : is_open (false),
    data(new WorldObjsData::InfoBoxData(*data_))
{
}

void
InfoBox::draw (GraphicContext& gc)
{
  int x = static_cast<int>(gc.get_x_offset() + (gc.get_width ()/2));
  int y = static_cast<int>(gc.get_y_offset() + (gc.get_height()/2));

  int x_pos = static_cast<int>(data->pos.x) + x;
  int y_pos = static_cast<int>(data->pos.y) + y - 100;

  if (is_open)
    {
      int width = data->font->get_text_width(data->info_text.c_str());
      int border = 6;
      gc.draw_line(data->pos, data->pos + CL_Vector(0, 0 - 100), 0.0f, 1.0f, 0.0f, 1.0f);
      gc.draw(data->sprite, data->pos);
      CL_Display::fill_rect(x_pos - width/2 - border,
                            y_pos - border,
			    x_pos + width/2 + border,
			    y_pos + data->font->get_height() + border,
			    0.0, 0.0, 0.0, 1.0);
      data->font->print_center(x_pos, y_pos, data->info_text.c_str()); 
    }
  else
    {
      gc.draw(data->sprite, data->pos);
    }
}

void
InfoBox::update (float delta)
{
  data->sprite.update(delta);

  PinguHolder* holder = world->get_pingu_p();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if ((*pingu)->is_inside (static_cast<int>(data->pos.x - 16),
                               static_cast<int>(data->pos.y - 32),
			       static_cast<int>(data->pos.x + 16),
			       static_cast<int>(data->pos.y)))
	{
	  is_open = true;
	}
    }
}

float
InfoBox::get_z_pos () const {
  return data->pos.z;
}

} // namespace WorldObjs

/* EOF */
