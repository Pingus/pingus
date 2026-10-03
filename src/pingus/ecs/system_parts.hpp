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
  world.get_pingus()->for_each(std::forward<Func>(func));
}

// systems.cpp
/** Draw a frame of an animation of the set at pos plus the animation's
    and the given offset, loading its sprite into 'sprites' if needed.
    With frame_count set, frame is a game timing step mapped onto the
    animation's frames. */
void draw_animation(SceneContext& gc, AnimationSet const& set, std::map<std::string, Sprite>& sprites,
                    std::string_view animation, Direction const& direction,
                    int frame, int frame_count, Vector2f const& pos, Vector2f const& offset,
                    float z_index = 0.0f);

/** The sprite of the animation for the direction, loaded into 'sprites'
    if needed */
Sprite& animation_sprite(AnimationSet const& set, std::map<std::string, Sprite>& sprites,
                         std::string_view animation, Direction const& direction);

// traps.cpp
void update_traps(World& world);
void startup_trap(World& world, ecs::Entity entity);

// weather.cpp
void update_weather(World& world);
void draw_weather(World& world, SceneContext& gc, ecs::Entity entity);

// objects.cpp
void update_entrances(World& world);
void update_level_objects(World& world);
void startup_level_object(World& world, ecs::Entity entity);
void draw_level_object(World& world, SceneContext& gc, ecs::Entity entity);

} // namespace pingus::systems

#endif

/* EOF */
