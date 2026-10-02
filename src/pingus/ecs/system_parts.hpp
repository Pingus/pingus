// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ECS_SYSTEM_PARTS_HPP
#define HEADER_PINGUS_PINGUS_ECS_SYSTEM_PARTS_HPP

// Internal interface between the system implementation files

#include "ecs/registry.hpp"
#include "pingus/ecs/components.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

namespace pingus::systems {

/** Level objects at or below the pingus' z-index update before the
    pingus move, the others afterwards. This keeps the order of the old
    z-sorted WorldObj update loop, which the level timing depends on
    (e.g. entrances at z 0 release before the pingus move, at z 100
    after). */
enum class Phase { BEFORE_PINGUS, AFTER_PINGUS };

inline bool in_phase(World& world, components::Transform const& transform, Phase phase)
{
  bool const before = transform.z_index <= world.get_pingus()->z_index();
  return before == (phase == Phase::BEFORE_PINGUS);
}

/** True if the pingu's position lies strictly within the zone */
inline bool in_zone(Pingu const& pingu, components::Transform const& transform, components::TriggerZone const& zone)
{
  Vector2f const pos = pingu.get_pos();
  return (pos.x() > transform.pos.x() + zone.x1 && pos.x() < transform.pos.x() + zone.x2 &&
          pos.y() > transform.pos.y() + zone.y1 && pos.y() < transform.pos.y() + zone.y2);
}

/** Call func(Pingu&) for each active pingu */
template<typename Func>
void for_each_pingu(World& world, Func&& func)
{
  PinguHolder* holder = world.get_pingus();
  for (PinguIter it = holder->begin(); it != holder->end(); ++it) {
    func(**it);
  }
}

// traps.cpp
void update_traps(World& world, Phase phase);
void startup_trap(World& world, ecs::Entity entity);
void draw_trap(World& world, SceneContext& gc, ecs::Entity entity);

// weather.cpp
void update_weather(World& world, Phase phase);
void draw_weather(World& world, SceneContext& gc, ecs::Entity entity);

// objects.cpp
void update_level_objects(World& world, Phase phase);
void startup_level_object(World& world, ecs::Entity entity);
void draw_level_object(World& world, SceneContext& gc, ecs::Entity entity);

} // namespace pingus::systems

#endif

/* EOF */
