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

#include "pingus/actions/splashed.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/pingu.hpp"
#include "pingus/world.hpp"

namespace pingus::actions {

Splashed::Splashed (Pingu* p) :
  PinguAction(p),
  // Game timing: splashing takes 16 steps of 33 ms
  clock(33, 16, false)
{
}

void
Splashed::update()
{
  clock.update();

  if (clock.is_finished())
  {
    pingu->set_status(Pingu::PS_DEAD);
  }
}

void
Splashed::get_look(PinguLook& look) const
{
  look.add("splashed", clock);
}

} // namespace pingus::actions

/* EOF */
