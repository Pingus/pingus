// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Systems for the interactive level objects: entrances, exits,
// teleporters, ice blocks, conveyor belts and switch doors

#include "pingus/ecs/system_parts.hpp"

#include <logmich/log.hpp>

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"

namespace pingus::systems {

using namespace pingus::components;

namespace {

/** Find the entity with the given id, returns null_entity if there is
    none or it lacks component T */
template<typename T>
ecs::Entity find_by_id(ecs::Registry& reg, std::string const& id)
{
  ecs::Entity result = ecs::null_entity;
  bool found = false;
  reg.each<ObjectId>([&](ecs::Entity entity, ObjectId& object_id) {
    if (!found && object_id.id == id)
    {
      found = true;
      if (reg.has<T>(entity)) {
        result = entity;
      }
    }
  });
  return result;
}

// Exit

void startup_exit(World& world, Transform const& transform, Exit const& exit)
{
  // the exit's surface is anchored at its bottom center
  CollisionMask mask(exit.desc);
  world.get_colmap()->remove(mask,
                             static_cast<int>(transform.pos.x()) - mask.get_width()/2,
                             static_cast<int>(transform.pos.y()) - mask.get_height());
}

void update_exits(World& world, ecs::Registry& reg)
{
  reg.each<Transform, TriggerZone, Owner, Exit>([&](ecs::Entity, Transform& transform, TriggerZone& zone, Owner& owner, Exit& exit) {
    exit.sprite.update();

    for_each_pingu(world, [&](Pingu& pingu) {
      if (pingu.get_owner() == owner.owner_id &&
          in_zone(pingu, transform, zone) &&
          pingu.get_status() != Pingu::PS_EXITED &&
          pingu.get_status() != Pingu::PS_DEAD &&
          pingu.get_action() != ActionName::EXITER)
      {
        pingu.set_action(ActionName::EXITER);
      }
    });
  });
}

// Entrance

void update_entrances(World& world, ecs::Registry& reg)
{
  reg.each<Transform, Owner, Entrance>([&](ecs::Entity, Transform& transform, Owner& owner, Entrance& entrance) {
    if (entrance.last_release + entrance.release_rate >= world.get_time()) {
      return;
    }
    entrance.last_release = world.get_time();

    if (world.check_armageddon()) {
      return;
    }

    Pingu* pingu = world.get_pingus()->create_pingu(transform.pos, owner.owner_id);
    if (!pingu) {
      // all pingus released
      return;
    }

    Direction d;
    switch (entrance.direction)
    {
      case Entrance::Direction::LEFT:
        d.left();
        break;

      case Entrance::Direction::MISC:
        if (entrance.last_was_right) {
          d.left();
        } else {
          d.right();
        }
        entrance.last_was_right = !entrance.last_was_right;
        break;

      case Entrance::Direction::RIGHT:
        d.right();
        break;
    }
    pingu->set_direction(d);
  });
}

// Teleporter

void startup_teleporter(World& world, Teleporter& teleporter)
{
  if (teleporter.target_id.empty())
  {
    log_error("target-id is empty");
    return;
  }

  teleporter.target = find_by_id<TeleporterTarget>(world.get_registry(), teleporter.target_id);
  if (teleporter.target == ecs::null_entity) {
    log_error("Teleporter: Couldn't find matching target-id or object isn't a TeleporterTarget");
  }
}

void update_teleporters(World& world, ecs::Registry& reg)
{
  reg.each<TeleporterTarget>([](ecs::Entity, TeleporterTarget& target) {
    target.clock.update();
  });

  reg.each<Transform, TriggerZone, Teleporter>([&](ecs::Entity, Transform& transform, TriggerZone& zone, Teleporter& teleporter) {
    teleporter.clock.update();

    if (teleporter.target == ecs::null_entity) {
      return;
    }

    Vector2f const target_pos = reg.get<Transform>(teleporter.target).pos;
    TeleporterTarget& target = reg.get<TeleporterTarget>(teleporter.target);

    for_each_pingu(world, [&](Pingu& pingu) {
      if (in_zone(pingu, transform, zone))
      {
        pingu.set_pos(target_pos.x(), target_pos.y());
        target.clock.restart();
        teleporter.clock.restart();
      }
    });
  });
}

// Ice block

void update_ice_blocks(World& world, ecs::Registry& reg)
{
  reg.each<Transform, TriggerZone, IceBlock>([&](ecs::Entity, Transform& transform, TriggerZone& zone, IceBlock& ice) {
    if (ice.finished) {
      return;
    }

    for_each_pingu(world, [&](Pingu& pingu) {
      if (in_zone(pingu, transform, zone)) {
        ice.last_contact = world.get_time();
      }
    });

    if (ice.last_contact && ice.last_contact + 1000 > world.get_time())
    {
      ice.thickness -= 0.01f;
      if (ice.thickness < 0)
      {
        ice.finished = true;
        ice.thickness = 0;
        world.remove(*ice.cmap, static_cast<int>(transform.pos.x()), static_cast<int>(transform.pos.y()));
      }
    }
  });
}

// Conveyor belt

void startup_conveyor_belt(World& world, Transform const& transform, ConveyorBelt const& belt)
{
  CollisionMask mask("worldobjs/conveyorbelt_cmap");
  for (int i = 0; i < (belt.width + 2); ++i)
  {
    world.put(mask,
              static_cast<int>(transform.pos.x()) + (15 * i),
              static_cast<int>(transform.pos.y()),
              Groundtype::GP_SOLID);
  }
}

void update_conveyor_belts(World& world, ecs::Registry& reg)
{
  reg.each<Transform, TriggerZone, ConveyorBelt>([&](ecs::Entity, Transform& transform, TriggerZone& zone, ConveyorBelt& belt) {
    belt.left.update();
    belt.middle.update();
    belt.right.update();

    for_each_pingu(world, [&](Pingu& pingu) {
      if (in_zone(pingu, transform, zone)) {
        pingu.set_pos(Vector2f(pingu.get_pos().x() - belt.speed * 0.025f, pingu.get_pos().y()));
      }
    });
  });
}

void draw_conveyor_belt(SceneContext& gc, Transform const& transform, ConveyorBelt const& belt)
{
  Vector2f const& pos = transform.pos;
  float const left_width = static_cast<float>(belt.left.get_width());
  float const middle_width = static_cast<float>(belt.middle.get_width());

  gc.color().draw(belt.left, pos);
  for (int i = 0; i < belt.width; ++i)
  {
    gc.color().draw(belt.middle,
                    Vector2f(pos.x() + left_width + static_cast<float>(i) * middle_width, pos.y()),
                    transform.z_index);
  }
  gc.color().draw(belt.right,
                  Vector2f(pos.x() + left_width + static_cast<float>(belt.width) * middle_width, pos.y()),
                  transform.z_index);
}

// Switch door

/** Set the door's area in the collision map to the given ground type */
void put_door(World& world, Transform const& transform, SwitchDoor const& door, Groundtype::GPType type)
{
  int const x = static_cast<int>(transform.pos.x());
  int const y = static_cast<int>(transform.pos.y());

  world.get_colmap()->put(*door.box_cmap, x, y, type);
  for (int i = 0; i < door.height; ++i)
  {
    world.get_colmap()->put(*door.tile_cmap, x,
                            y + i * door.tile_cmap->get_height() + door.box_cmap->get_height(),
                            type);
  }
}

void startup_switch(World& world, SwitchDoorSwitch& sw)
{
  if (sw.target_id.empty())
  {
    log_error("target-id is empty");
    return;
  }

  sw.door = find_by_id<SwitchDoor>(world.get_registry(), sw.target_id);
  if (sw.door == ecs::null_entity) {
    log_error("given target-id is not a SwitchDoorDoor");
  }
}

void update_switch_doors(World& world, ecs::Registry& reg)
{
  reg.each<Transform, TriggerZone, SwitchDoorSwitch>([&](ecs::Entity, Transform& transform, TriggerZone& zone, SwitchDoorSwitch& sw) {
    if (sw.triggered || sw.door == ecs::null_entity) {
      return;
    }

    for_each_pingu(world, [&](Pingu& pingu) {
      if (!sw.triggered && in_zone(pingu, transform, zone))
      {
        sw.triggered = true;
        reg.get<SwitchDoor>(sw.door).opening = true;
      }
    });
  });

  reg.each<Transform, SwitchDoor>([&](ecs::Entity, Transform& transform, SwitchDoor& door) {
    if (door.current_height > 0 && door.opening)
    {
      --door.current_height;

      // If the door is open enough, so that a pingu fits under it, it
      // is removed from the collision map
      if (door.current_height + 10 < door.height) {
        put_door(world, transform, door, Groundtype::GP_NOTHING);
      }
    }
  });
}

void draw_switch_door(SceneContext& gc, Transform const& transform, SwitchDoor const& door)
{
  gc.color().draw(door.box, transform.pos);
  for (int i = 0; i < door.current_height; ++i)
  {
    gc.color().draw(door.tile,
                    Vector2f(transform.pos.x(),
                             transform.pos.y() + static_cast<float>(i * door.tile.get_height() + door.box.get_height())));
  }
}

} // namespace

void
update_entrances(World& world)
{
  update_entrances(world, world.get_registry());
}

void
update_level_objects(World& world)
{
  ecs::Registry& reg = world.get_registry();
  update_exits(world, reg);
  update_teleporters(world, reg);
  update_conveyor_belts(world, reg);
  update_switch_doors(world, reg);
  update_ice_blocks(world, reg);

  reg.each<Teleporter, AnimatedSprite>([](ecs::Entity, Teleporter& teleporter, AnimatedSprite& anim) {
    anim.frame = teleporter.clock.frame();
  });
  reg.each<TeleporterTarget, AnimatedSprite>([](ecs::Entity, TeleporterTarget& target, AnimatedSprite& anim) {
    anim.frame = target.clock.frame();
  });
}

void
startup_level_object(World& world, ecs::Entity entity)
{
  ecs::Registry& reg = world.get_registry();
  Transform const& transform = reg.get<Transform>(entity);

  if (auto* exit = reg.try_get<Exit>(entity)) {
    startup_exit(world, transform, *exit);
  }

  if (auto* teleporter = reg.try_get<Teleporter>(entity)) {
    startup_teleporter(world, *teleporter);
  }

  if (auto* ice = reg.try_get<IceBlock>(entity)) {
    world.put(*ice->cmap,
              static_cast<int>(transform.pos.x()),
              static_cast<int>(transform.pos.y()),
              Groundtype::GP_GROUND);
  }

  if (auto* belt = reg.try_get<ConveyorBelt>(entity)) {
    startup_conveyor_belt(world, transform, *belt);
  }

  if (auto* door = reg.try_get<SwitchDoor>(entity)) {
    put_door(world, transform, *door, Groundtype::GP_SOLID);
  }

  if (auto* sw = reg.try_get<SwitchDoorSwitch>(entity)) {
    startup_switch(world, *sw);
  }
}

void
draw_level_object(World& world, SceneContext& gc, ecs::Entity entity)
{
  ecs::Registry& reg = world.get_registry();
  Transform const& transform = reg.get<Transform>(entity);

  if (auto* exit = reg.try_get<Exit>(entity))
  {
    gc.color().draw(exit->sprite, transform.pos);
    gc.color().draw(exit->flag, transform.pos + geom::foffset(40, 0));
  }

  if (auto* ice = reg.try_get<IceBlock>(entity))
  {
    if (!ice->finished) {
      gc.color().draw(ice->sprite, transform.pos);
    }
  }

  if (auto* belt = reg.try_get<ConveyorBelt>(entity)) {
    draw_conveyor_belt(gc, transform, *belt);
  }

  if (auto* door = reg.try_get<SwitchDoor>(entity)) {
    draw_switch_door(gc, transform, *door);
  }

  if (auto* sw = reg.try_get<SwitchDoorSwitch>(entity)) {
    gc.color().draw(sw->sprite, transform.pos);
  }
}

} // namespace pingus::systems

/* EOF */
