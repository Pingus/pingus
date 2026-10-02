// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ECS_SYSTEMS_HPP
#define HEADER_PINGUS_PINGUS_ECS_SYSTEMS_HPP

#include "ecs/registry.hpp"

namespace pingus {

class ObjectData;
class ObjectTypeDef;
class SceneContext;
class SmallMap;
class World;

} // namespace pingus

/** Systems operating on the level object entities of a World.

    Per tick World::update() runs update_objects() before the pingus
    move and update_after_pingus() afterwards. Startup and drawing are
    done per entity, since they have to be interleaved in z-order with
    the remaining non-entity world objects (ground, pingus, particles). */
namespace pingus::systems {

/** True if objects of this type are created as entities by create_object() */
bool is_entity_type(ObjectTypeDef const& type);

/** Create the entity and its components for a level object */
ecs::Entity create_object(World& world, ObjectData const& data);

/** True if the entity covers the whole screen */
bool is_solid_background(World& world, ecs::Entity entity);

/** One time setup once all objects exist: draw into the collision map,
    resolve links between objects, ... */
void startup(World& world, ecs::Entity entity);

/** Per tick logic that runs before the pingus are updated */
void update_objects(World& world);

/** Per tick logic that runs after the pingus are updated */
void update_after_pingus(World& world);

void draw(World& world, SceneContext& gc, ecs::Entity entity);
void draw_smallmap(World& world, SmallMap& smallmap);

} // namespace pingus::systems

#endif

/* EOF */
