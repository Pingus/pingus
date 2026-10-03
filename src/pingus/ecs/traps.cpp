// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Systems for the traps: spike, fake exit, guillotine, hammer, laser exit
// and smasher

#include "pingus/ecs/system_parts.hpp"

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/particles/smoke_particle_holder.hpp"

namespace pingus::systems {

using namespace pingus::components;

namespace {

/** Area in which pingus get killed once the spikes are out */
TriggerZone const spike_kill_zone{16.0f - 12.0f, 0.0f, 16.0f + 12.0f, 32.0f};

void update_spikes(World& world, ecs::Registry& reg)
{
  reg.each<Transform, TriggerZone, Spike>([&](ecs::Entity, Transform& transform, TriggerZone& zone, Spike& spike) {
    if (spike.killing) {
      spike.clock.update();
    }

    for_each_pingu(world, [&](Pingu& pingu) {
      if (!spike.killing)
      {
        if (in_zone(pingu, transform, zone))
        {
          spike.clock.restart();
          spike.killing = true;
        }
      }
      else
      {
        if (spike.clock.frame() == 3 && in_zone(pingu, transform, spike_kill_zone)) {
          pingu.set_status(Pingu::PS_DEAD);
        }
      }
    });

    if (spike.clock.frame() == spike.clock.frame_count() - 1) {
      spike.killing = false;
    }
  });
}

void update_fake_exits(World& world, ecs::Registry& reg)
{
  reg.each<Transform, TriggerZone, FakeExit>([&](ecs::Entity, Transform& transform, TriggerZone& zone, FakeExit& fake_exit) {
    for_each_pingu(world, [&](Pingu& pingu) {
      if (in_zone(pingu, transform, zone) &&
          pingu.get_action() != ActionName::SPLASHED)
      {
        if (!fake_exit.smashing)
        {
          fake_exit.clock.restart();
          fake_exit.smashing = true;
        }

        if (fake_exit.clock.frame() == 4) {
          pingu.set_action(ActionName::SPLASHED);
        }
      }
    });

    if (fake_exit.smashing)
    {
      fake_exit.clock.update();

      // One smash per trigger, then back to the idle frame
      if (fake_exit.clock.is_finished())
      {
        fake_exit.smashing = false;
        fake_exit.clock.restart();
      }
    }
  });
}

void update_guillotines(World& world, ecs::Registry& reg)
{
  reg.each<Transform, TriggerZone, Guillotine>([&](ecs::Entity, Transform& transform, TriggerZone& zone, Guillotine& guillotine) {
    if (guillotine.kill_clock.is_finished()) {
      guillotine.killing = false;
    }

    for_each_pingu(world, [&](Pingu& pingu) {
      if (!guillotine.killing && in_zone(pingu, transform, zone))
      {
        guillotine.killing = true;
        pingu.set_status(Pingu::PS_DEAD);
        guillotine.direction = pingu.direction();
        guillotine.kill_clock.restart();
      }
    });

    if (guillotine.killing)
    {
      guillotine.kill_clock.update();
      // FIXME: Should be a different sound
      if (guillotine.kill_clock.frame() == 7) {
        world.play_sound("splash", transform.pos);
      }
    }
    else
    {
      guillotine.idle_clock.update();
    }
  });
}

void update_hammers(World& world, ecs::Registry& reg)
{
  reg.each<Transform, Hammer>([&](ecs::Entity, Transform& transform, Hammer& hammer) {
    Vector2f const& pos = transform.pos;

    if (hammer.down)
    {
      hammer.count += 1;
      if (hammer.count == hammer.frame_count - 1)
      {
        for_each_pingu(world, [&](Pingu& pingu) {
          if (pingu.get_action() != ActionName::SPLASHED &&
              pingu.get_x() > pos.x() + 55  && pingu.get_x() < pos.x() + 77 &&
              pingu.get_y() > pos.y() + 146 && pingu.get_y() < pos.y() + 185)
          {
            pingu.set_action(ActionName::SPLASHED);
          }
        });
        hammer.down = false;
      }
    }
    else
    {
      hammer.count -= 1;
      if (hammer.count == 0) {
        hammer.down = true;
      }
    }
  });
}

void update_laser_exits(World& world, ecs::Registry& reg)
{
  reg.each<Transform, TriggerZone, LaserExit>([&](ecs::Entity, Transform& transform, TriggerZone& zone, LaserExit& laser) {
    for_each_pingu(world, [&](Pingu& pingu) {
      if (!laser.killing &&
          in_zone(pingu, transform, zone) &&
          pingu.get_action() != ActionName::LASERKILL)
      {
        laser.killing = true;
        pingu.set_action(ActionName::LASERKILL);
      }
    });

    if (laser.killing)
    {
      if (laser.clock.is_finished())
      {
        laser.clock.restart();
        laser.killing = false;
      }
      else
      {
        laser.clock.update();
      }
    }
  });
}

void update_smashers(World& world, ecs::Registry& reg)
{
  reg.each<Transform, Smasher>([&](ecs::Entity, Transform& transform, Smasher& smasher) {
    Vector2f const& pos = transform.pos;

    // Activate the smasher if a Pingu is under it
    for_each_pingu(world, [&](Pingu& pingu) {
      if (((pingu.direction().is_left() &&
            pingu.get_pos().x() > pos.x() + 65 &&
            pingu.get_pos().x() < pos.x() + 85) ||
           (pingu.direction().is_right() &&
            pingu.get_pos().x() > pos.x() + 190 &&
            pingu.get_pos().x() < pos.x() + 210)) &&
          pingu.get_action() != ActionName::SPLASHED &&
          !smasher.smashing)
      {
        smasher.count = 0;
        smasher.downwards = true;
        smasher.smashing = true;
      }
    });

    if (!smasher.smashing) {
      return;
    }

    if (smasher.downwards)
    {
      if (smasher.count >= 5)
      {
        // SMASH!!! The thing hitten earth and kills the pingus
        smasher.downwards = false;
        --smasher.count;
        sound::PingusSound::play_sound("tenton");

        Random& rng = world.get_fx_random();
        for (int i = 0; i < 20; ++i)
        {
          float const x = pos.x() + 20 + float(rng.next_int(260));
          float const vel_x = rng.next_float() - 0.5f;
          float const vel_y = rng.next_float() - 0.5f;
          world.get_smoke_particle_holder()->add_particle(x, pos.y() + 180, vel_x, vel_y);
        }

        for_each_pingu(world, [&](Pingu& pingu) {
          if (pingu.is_inside(pos.x() + 30, pos.y() + 90, pos.x() + 250, pos.y() + 190) &&
              pingu.get_action() != ActionName::SPLASHED)
          {
            pingu.set_action(ActionName::SPLASHED);
          }
        });
      }
      else
      {
        ++smasher.count;
      }
    }
    else
    {
      if (smasher.count <= 0)
      {
        smasher.count = 0;
        smasher.smashing = false;
      }
      else
      {
        --smasher.count;
      }
    }
  });
}

} // namespace

void
update_traps(World& world)
{
  ecs::Registry& reg = world.get_registry();
  update_spikes(world, reg);
  update_fake_exits(world, reg);
  update_guillotines(world, reg);
  update_hammers(world, reg);
  update_laser_exits(world, reg);
  update_smashers(world, reg);
}

void
startup_trap(World& world, ecs::Entity entity)
{
  ecs::Registry& reg = world.get_registry();
  if (reg.has<Smasher>(entity))
  {
    Transform const& transform = reg.get<Transform>(entity);
    CollisionMask buf("traps/smasher_cmap");
    world.put(buf,
              static_cast<int>(transform.pos.x()),
              static_cast<int>(transform.pos.y()),
              Groundtype::GP_SOLID);
  }
}

void
draw_trap(World& world, SceneContext& gc, ecs::Entity entity)
{
  ecs::Registry& reg = world.get_registry();
  Transform const& transform = reg.get<Transform>(entity);

  if (auto* spike = reg.try_get<Spike>(entity))
  {
    if (spike->killing)
    {
      spike->clock.apply_to(spike->sprite);
      gc.color().draw(spike->sprite, transform.pos);
    }
  }

  if (auto* fake_exit = reg.try_get<FakeExit>(entity))
  {
    fake_exit->clock.apply_to(fake_exit->sprite);
    gc.color().draw(fake_exit->sprite, transform.pos);
  }

  if (auto* guillotine = reg.try_get<Guillotine>(entity))
  {
    if (guillotine->killing)
    {
      Sprite& sprite = guillotine->direction.is_left() ? guillotine->sprite_kill_left : guillotine->sprite_kill_right;
      guillotine->kill_clock.apply_to(sprite);
      gc.color().draw(sprite, transform.pos);
    }
    else
    {
      guillotine->idle_clock.apply_to(guillotine->sprite_idle);
      gc.color().draw(guillotine->sprite_idle, transform.pos);
    }
  }

  if (auto* hammer = reg.try_get<Hammer>(entity))
  {
    hammer->sprite.set_frame(hammer->count);
    gc.color().draw(hammer->sprite, transform.pos);
  }

  if (auto* laser = reg.try_get<LaserExit>(entity))
  {
    laser->clock.apply_to(laser->sprite);
    gc.color().draw(laser->sprite, transform.pos);
  }

  if (auto* smasher = reg.try_get<Smasher>(entity))
  {
    smasher->sprite.set_frame(smasher->count);
    gc.color().draw(smasher->sprite, transform.pos);
  }
}

} // namespace pingus::systems

/* EOF */
