// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Systems for the pingu entities

#include "pingus/ecs/system_parts.hpp"

namespace pingus::systems {

using namespace pingus::components;

void
update_pingus(World& world)
{
  PinguHolder& holder = *world.get_pingus();

  world.get_registry().each<Pingu, ActivePingu>([&](ecs::Entity, Pingu& pingu, ActivePingu&) {
    pingu.update();

    if (pingu.get_status() != Pingu::PS_ALIVE) {
      holder.deactivate(pingu);
    }
  });
}

void
draw_pingus(World& world, SceneContext& gc)
{
  PinguHolder& holder = *world.get_pingus();

  // Draw all walkers first and then the others, so that pingus doing
  // something are easier to spot
  holder.for_each([&](Pingu& pingu) {
    if (pingu.get_action() == ActionName::WALKER) {
      pingu.draw(gc);
    }
  });

  holder.for_each([&](Pingu& pingu) {
    if (pingu.get_action() != ActionName::WALKER) {
      pingu.draw(gc);
    }
  });
}

} // namespace pingus::systems

/* EOF */
