// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Systems for the pingu entities

#include "pingus/ecs/system_parts.hpp"

#include <cstdio>

#include "engine/display/scene_context.hpp"
#include "pingus/fonts.hpp"
#include "pingus/pingu_action.hpp"

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

namespace {

void draw_pingu(World& world, SceneContext& gc, Pingu& pingu)
{
  PinguView& view = world.get_registry().get<PinguView>(pingu.get_entity());

  if (!view.set) {
    view.set = AnimationSet::get("pingus/player" + pingu.get_owner_str());
  }

  // overlays fired by effects are drawn once, below the pingu
  for (auto const& overlay : view.overlays) {
    draw_animation(gc, *view.set, view.sprites, overlay.animation, pingu.direction(),
                   0, 0, pingu.get_pos(), overlay.offset);
  }
  view.overlays.clear();

  view.look.layers.clear();
  pingu.get_current_action()->get_look(view.look);

  for (auto const& layer : view.look.layers) {
    draw_animation(gc, *view.set, view.sprites, layer.animation, pingu.direction(),
                   layer.frame, layer.frame_count, pingu.get_pos(), layer.offset);
  }

  if (pingu.get_action_time() != -1)
  {
    // FIXME: some people preffer a 5-0 or a 9-0 countdown, not sure
    // FIXME: about that got used to the 50-0 countdown [counting is
    // FIXME: in ticks, should probally be in seconds]
    char str[16];
    snprintf(str, 16, "%d", pingu.get_action_time() / 3);
    gc.color().print_center(pingus::fonts::chalk_normal, Vector2i(pingu.get_xi(), pingu.get_yi() - 48), str);
  }
}

} // namespace

void
draw_pingus(World& world, SceneContext& gc)
{
  PinguHolder& holder = *world.get_pingus();

  // Draw all walkers first and then the others, so that pingus doing
  // something are easier to spot
  holder.for_each([&](Pingu& pingu) {
    if (pingu.get_action() == ActionName::WALKER) {
      draw_pingu(world, gc, pingu);
    }
  });

  holder.for_each([&](Pingu& pingu) {
    if (pingu.get_action() != ActionName::WALKER) {
      draw_pingu(world, gc, pingu);
    }
  });
}

} // namespace pingus::systems

/* EOF */
