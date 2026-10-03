// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Systems for the pingu entities

#include "pingus/ecs/system_parts.hpp"

#include <algorithm>
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

  std::shared_ptr<PinguAction> action = pingu.get_current_action();
  if (view.action != action)
  {
    view.action = std::move(action);
    view.shown_once.clear();
  }

  view.look.layers.clear();
  view.action->get_look(view.look);

  for (auto const& layer : view.look.layers)
  {
    if (layer.once)
    {
      std::string const name(layer.animation);
      if (std::find(view.shown_once.begin(), view.shown_once.end(), name) != view.shown_once.end()) {
        continue;
      }
      view.shown_once.push_back(name);
    }

    AnimationDef const& def = view.set->get_animation(layer.animation);
    std::string const& sprite_name = def.sprite_name(pingu.direction());

    auto it = view.sprites.find(sprite_name);
    if (it == view.sprites.end()) {
      it = view.sprites.emplace(sprite_name, Sprite(sprite_name)).first;
    }

    it->second.set_frame(layer.frame);
    geom::foffset const offset(def.offset.x() + layer.offset.x(), def.offset.y() + layer.offset.y());
    gc.color().draw(it->second, pingu.get_pos() + offset);
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
