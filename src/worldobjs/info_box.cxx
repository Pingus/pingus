//  $Id: info_box.cxx,v 1.24 2003/10/21 11:01:52 grumbel Exp $
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

#include <ClanLib/Display/font.h>
#include <ClanLib/Display/display.h>
#include "../gui/graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../fonts.hxx"
#include "../world.hxx"
#include "../worldobjsdata/info_box_data.hxx"
#include "info_box.hxx"

namespace Pingus {
namespace WorldObjs {

InfoBox::InfoBox (const WorldObjsData::InfoBoxData& data_)
  : data(new WorldObjsData::InfoBoxData(data_)),
    sprite("infobox", "worldobjs"),
    is_open (false)
{
  sprite.set_align_center_bottom();
}

InfoBox::~InfoBox ()
{
  delete data;
}

void
InfoBox::draw (GraphicContext& gc)
{
#ifdef CLANLIB_0_6
  int x = static_cast<int>(gc.get_x_offset() + (gc.get_width ()/2));
  int y = static_cast<int>(gc.get_y_offset() + (gc.get_height()/2));

  int x_pos = static_cast<int>(data->pos.x) + x;
  int y_pos = static_cast<int>(data->pos.y) + y - 100;

  if (is_open)
    {
      int width = Fonts::pingus_small.get_text_width(data->info_text.c_str());
      int border = 6;
      gc.draw_line(data->pos, data->pos + Vector(0, 0 - 100), 0.0f, 1.0f, 0.0f, 1.0f);
      gc.draw(data->sprite, data->pos);
      CL_Display::fill_rect(x_pos - width/2 - border,
                            y_pos - border,
			    x_pos + width/2 + border,
			    y_pos + Fonts::pingus_small->get_height() + border,
			    0.0, 0.0, 0.0, 1.0);
      gc.print_center(Fonts::pingus_small, x_pos, y_pos, data->info_text.c_str());
    }
  else
    {
      gc.draw(data->sprite, data->pos);
    }
#endif
}

void
InfoBox::update ()
{
  data->sprite.update();

  PinguHolder* holder = world->get_pingus();
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
} // namespace Pingus

/* EOF */
