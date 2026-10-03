// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Effects defined in the animation sets: sounds, particles and overlays
// fired when an animation reaches a step

#include "pingus/ecs/system_parts.hpp"

#include "engine/sound/sound.hpp"
#include "pingus/particles/pingu_particle_holder.hpp"
#include "pingus/particles/smoke_particle_holder.hpp"

namespace pingus::systems {

using namespace pingus::components;

namespace {

/** Random offset between 0 and 'spread' */
float spread_offset(Random& rng, float spread)
{
  return spread > 0.0f ? static_cast<float>(rng.next_int(static_cast<int>(spread))) : 0.0f;
}

void fire_effect(World& world, Effect const& effect, Vector2f const& pos, std::vector<OverlayEffect>& overlays)
{
  if (auto* sound = std::get_if<SoundEffect>(&effect))
  {
    sound::PingusSound::play_sound(sound->name, sound->volume);
  }
  else if (auto* particles = std::get_if<ParticlesEffect>(&effect))
  {
    Random& rng = world.get_fx_random();
    for (int i = 0; i < particles->count; ++i)
    {
      float const x = pos.x() + particles->offset.x() + spread_offset(rng, particles->spread.x());
      float const y = pos.y() + particles->offset.y() + spread_offset(rng, particles->spread.y());

      if (particles->kind == "pingu")
      {
        world.get_pingu_particle_holder()->add_particle(static_cast<int>(x), static_cast<int>(y));
      }
      else // "smoke", checked when loading
      {
        float const vel_x = rng.next_float() - 0.5f;
        float const vel_y = rng.next_float() - 0.5f;
        world.get_smoke_particle_holder()->add_particle(x, y, vel_x, vel_y);
      }
    }
  }
  else if (auto* overlay = std::get_if<OverlayEffect>(&effect))
  {
    overlays.push_back(*overlay);
  }
}

/** Fire the effects of the animation that were passed since the last
    check. Restarting or looping animations fire them again. */
void update_effect_state(World& world, AnimationDef const& def, EffectState& state, int step,
                         Vector2f const& pos, std::vector<OverlayEffect>& overlays)
{
  if (state.animation != def.name || step < state.last_step)
  {
    state.animation = def.name;
    state.last_step = -1;
  }

  for (auto const& trigger : def.effects)
  {
    if (state.last_step < trigger.step && trigger.step <= step) {
      fire_effect(world, trigger.effect, pos, overlays);
    }
  }

  state.last_step = step;
}

void update_pingu_effects(World& world, Pingu& pingu)
{
  PinguView& view = world.get_registry().get<PinguView>(pingu.get_entity());

  if (!view.set) {
    view.set = AnimationSet::get("pingus/player" + pingu.get_owner_str());
  }

  std::shared_ptr<PinguAction> action = pingu.get_current_action();
  if (view.action != action)
  {
    view.action = std::move(action);
    view.effect_states.clear();
  }

  view.look.layers.clear();
  view.action->get_look(view.look);

  view.effect_states.resize(view.look.layers.size());
  for (size_t i = 0; i < view.look.layers.size(); ++i)
  {
    PinguLook::Layer const& layer = view.look.layers[i];
    update_effect_state(world, view.set->get_animation(layer.animation), view.effect_states[i],
                        layer.frame, pingu.get_pos(), view.overlays);
  }
}

} // namespace

void
update_effects(World& world)
{
  world.get_pingus()->for_each([&](Pingu& pingu) {
    update_pingu_effects(world, pingu);
  });

  world.get_registry().each<Transform, AnimatedSprite>([&](ecs::Entity, Transform& transform, AnimatedSprite& anim) {
    update_effect_state(world, anim.set->get_animation(anim.animation), anim.effect_state,
                        anim.frame, transform.pos, anim.overlays);
  });
}

} // namespace pingus::systems

/* EOF */
