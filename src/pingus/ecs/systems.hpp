// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ECS_SYSTEMS_HPP
#define HEADER_PINGUS_PINGUS_ECS_SYSTEMS_HPP

#include "ecs/registry.hpp"

namespace pingus {

class ObjectData;
class SceneContext;
class SmallMap;
class World;

} // namespace pingus

/** Systems operating on the level object entities of a World.

    Per tick World::update() runs update_spawners(), then lets the pingus
    move, then runs update_objects(). Startup and drawing are
    done per entity, since they have to be interleaved in z-order with
    the remaining non-entity world objects (ground, pingus, particles). */
namespace pingus::systems {

/** The z-index the object is sorted and drawn with. Usually the one from
    the level file, but some types always use a fixed one. */
float object_z_index(ObjectData const& data);

/** Create the entity and its components for a level object */
ecs::Entity create_object(World& world, ObjectData const& data);

/** True if the object covers the whole screen */
bool is_solid_background(ObjectData const& data);

/** One time setup once all objects exist: draw into the collision map,
    resolve links between objects, ... */
void startup(World& world, ecs::Entity entity);

/** Release new pingus, runs before the pingus move */
void update_spawners(World& world);

/** Traps, exits and other objects reacting to the pingus, weather and
    decorative animation; runs after the pingus moved */
void update_objects(World& world);

void draw(World& world, SceneContext& gc, ecs::Entity entity);
void draw_smallmap(World& world, SmallMap& smallmap);

} // namespace pingus::systems

#endif

/* EOF */
