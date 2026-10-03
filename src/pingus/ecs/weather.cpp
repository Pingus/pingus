// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Systems for the snow and rain generators

#include "pingus/ecs/system_parts.hpp"

#include <cmath>

#include <logmich/log.hpp>

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/globals.hpp"
#include "pingus/particles/rain_particle_holder.hpp"
#include "pingus/particles/snow_particle_holder.hpp"

namespace pingus::systems {

using namespace pingus::components;

namespace {

void add_snow_flake(World& world, Random& rng)
{
  bool const colliding = (rng.next_int(3) == 0);
  world.get_snow_particle_holder()->add_particle(rng.next_int(world.get_width()), -globals::tile_size, colliding);
}

void update_snow_generators(World& world, ecs::Registry& reg)
{
  reg.each<SnowGenerator>([&](ecs::Entity, SnowGenerator& snow) {
    Random& rng = world.get_fx_random();

    for (int i = 0; static_cast<float>(i) < std::floor(snow.intensity); ++i) {
      add_snow_flake(world, rng);
    }

    // the fractional part of the intensity is the chance of another flake
    if ((snow.intensity - static_cast<float>(static_cast<int>(snow.intensity))) > rng.next_float()) {
      add_snow_flake(world, rng);
    }
  });
}

void update_rain_generators(World& world, ecs::Registry& reg)
{
  reg.each<RainGenerator>([&](ecs::Entity, RainGenerator& rain) {
    Random& rng = world.get_fx_random();

    if (rain.waiter_count < 0.0f && rng.next_int(150) == 0)
    {
      log_info("Doing thunder");
      rain.do_thunder = true;
      rain.thunder_count = 1.0f;
      rain.waiter_count = 1.0f;
      sound::PingusSound::play_sound("thunder");
    }

    if (rain.do_thunder) {
      rain.thunder_count -= 10.0f * 0.025f;
    }

    rain.waiter_count -= 20.0f * 0.025f;

    for (int i = 0; i < 16; ++i) {
      world.get_rain_particle_holder()->add_particle(rng.next_int(world.get_width() * 2), -32);
    }
  });
}

} // namespace

void
update_weather(World& world)
{
  ecs::Registry& reg = world.get_registry();
  update_snow_generators(world, reg);
  update_rain_generators(world, reg);
}

void
draw_weather(World& world, SceneContext& gc, ecs::Entity entity)
{
  if (auto* rain = world.get_registry().try_get<RainGenerator>(entity))
  {
    if (rain->do_thunder)
    {
      if (rain->thunder_count < 0.0f)
      {
        rain->do_thunder = false;
        rain->thunder_count = 0.0f;
        rain->waiter_count = 1.0f;
      }

      gc.color().fill_screen(Color(255, 255, 255, static_cast<uint8_t>(rain->thunder_count * 255)));
    }
  }
}

} // namespace pingus::systems

/* EOF */
