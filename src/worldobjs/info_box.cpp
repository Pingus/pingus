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

#include "../display/scene_context.hpp"
#include "../pingu.hpp"
#include "../pingu_holder.hpp"
#include "../fonts.hpp"
#include "../world.hpp"
#include "../resource.hpp"
#include "info_box.hpp"

namespace WorldObjs {

InfoBox::InfoBox(const FileReader& reader)
  : sprite(Resource::load_sprite("worldobjs/infobox")),
    is_open (false)
{
  reader.read_vector("position", pos);
  reader.read_string("info-text", info_text);
}

void
InfoBox::draw (SceneContext& gc)
{
#if 0 // FIXME:
  int x = static_cast<int>(gc.get_x_offset() + (gc.get_width ()/2));
  int y = static_cast<int>(gc.get_y_offset() + (gc.get_height()/2));

  int x_pos = static_cast<int>(pos.x) + x;
  int y_pos = static_cast<int>(pos.y) + y - 100;

  if (is_open)
    {
      int width = Fonts::pingus_small.bounding_rect(0, 0, info_text).get_width();
      int border = 6;
      gc.color().draw_line(pos, pos + Vector3f(0, 0 - 100), 0.0f, 1.0f, 0.0f, 1.0f);
      gc.color().draw(sprite, pos);
      CL_Display::fill_rect(CL_Rect(x_pos - width/2 - border,
                                    y_pos - border,
                                    x_pos + width/2 + border,
                                    y_pos + Fonts::pingus_small.get_height() + border),
			    Color(0, 0, 0, 255));
      gc.print_center(Fonts::pingus_small, x_pos, y_pos, info_text);
    }
  else
    {
      gc.color().draw(sprite, pos);
    }
#endif
}

void
InfoBox::update ()
{
  sprite.update();

  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if ((*pingu)->is_inside (static_cast<int>(pos.x - 16),
                               static_cast<int>(pos.y - 32),
			       static_cast<int>(pos.x + 16),
			       static_cast<int>(pos.y)))
	{
	  is_open = true;
	}
    }
}

float
InfoBox::get_z_pos() const 
{
  return pos.z;
}

} // namespace WorldObjs

/* EOF */
