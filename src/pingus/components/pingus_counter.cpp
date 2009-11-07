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

#include "pingus/components/pingus_counter.hpp"

#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/pingu_holder.hpp"
#include "pingus/server.hpp"
#include "pingus/world.hpp"

PingusCounter::PingusCounter(Server* s) :
  server(s),
  font(),
  background("core/buttons/info")
{
  font = Fonts::pingus_small_fixnum;
}

void
PingusCounter::draw(DrawingContext& gc)
{
  char str[128];

  gc.draw(background, Vector2i(gc.get_width()/2, 0));

  World* world = server->get_world();

  snprintf(str, 128, _("Released:%3d/%d   Out:%3d   Saved:%3d/%d").c_str(),
           world->get_pingus()->get_number_of_released(),
           world->get_pingus()->get_number_of_allowed(),
           world->get_pingus()->get_number_of_alive(),
           world->get_pingus()->get_number_of_exited(),
           server->get_plf().get_number_to_save());

  gc.print_center(font, Vector2i(gc.get_width()/2, -2), str);
}

/* EOF */
