// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/components/pingus_counter.hpp"

#include <format>

#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/pingu_holder.hpp"
#include "pingus/server.hpp"
#include "pingus/world.hpp"

namespace pingus {

PingusCounter::PingusCounter(Server* s) :
  server(s),
  font(),
  background("core/buttons/info")
{
  font = pingus::fonts::pingus_small_fixnum;
}

void
PingusCounter::draw(DrawingContext& gc)
{
  gc.draw(background, Vector2i(gc.get_width()/2, 0));

  World* world = server->get_world();

  // Runtime translated format string → std::vformat (not std::format,
  // which requires a compile-time format string). make_format_args needs
  // lvalues, so bind the counts first.
  int released = world->get_pingus()->get_number_of_released();
  int allowed  = world->get_pingus()->get_number_of_allowed();
  int alive    = world->get_pingus()->get_number_of_alive();
  int exited   = world->get_pingus()->get_number_of_exited();
  int to_save  = server->get_plf().get_number_to_save();

  std::string text = std::vformat(
    std::string(_("Released:{:3d}/{:d}   Out:{:3d}   Saved:{:3d}/{:d}")),
    std::make_format_args(released, allowed, alive, exited, to_save));

  gc.print_center(font, Vector2i(gc.get_width()/2, -2), text);
}

} // namespace pingus

/* EOF */
