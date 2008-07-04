//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include "../display/display.hpp"
#include "../gettext.h"
#include "../resource.hpp"
#include "../world.hpp"
#include "../pingu_holder.hpp"
#include "../server.hpp"
#include "../globals.hpp"
#include "../fonts.hpp"
#include "pingus_counter.hpp"


PingusCounter::PingusCounter(Server* s)
  : server(s),
    background(Sprite("core/buttons/info"))
{
  font = Fonts::pingus_small_fixnum;
}

void
PingusCounter::draw(DrawingContext& gc)
{
  char str[128];

  if (!fast_mode)
    gc.draw(background, Vector2i(gc.get_width()/2, 0));

  World* world = server->get_world();

  snprintf(str, 128, _("Released:%3d/%d   Out:%3d   Saved:%3d/%d").c_str(),
	   world->get_pingus()->get_number_of_released(),
	   world->get_pingus()->get_number_of_allowed(),
	   world->get_pingus()->get_number_of_alive(),
	   world->get_pingus()->get_number_of_exited(),
	   server->get_plf().get_number_to_save());

  gc.print_center(font, gc.get_width()/2, -1, str);
}


/* EOF */
