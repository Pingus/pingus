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

#include "pingus/actions/floater.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/pingu.hpp"

namespace pingus::actions {

Floater::Floater(Pingu* p) :
  PinguAction(p),
  falling_depth(0),
  step(0),
  clock(look_animation("floater").make_clock())
{
}

void
Floater::update()
{
  clock.update();

  pingu->set_velocity(glm::vec2(0.0f, 1.0f));

  if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING)
  {
    ++step;
    if (step > 0)
    {
      pingu->set_y(pingu->get_y() + 1);
      step = 0;
    }
  }
  else
  {
    pingu->set_action (ActionName::WALKER);
  }
}

bool
Floater::change_allowed(ActionName::Enum action)
{
  return action == ActionName::BOMBER;
}

void
Floater::get_look(PinguLook& look) const
{
  look.add("floater", clock.frame());
}

} // namespace pingus::actions

/* EOF */
