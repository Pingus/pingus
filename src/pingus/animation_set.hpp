// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ANIMATION_SET_HPP
#define HEADER_PINGUS_PINGUS_ANIMATION_SET_HPP

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "math/vector2f.hpp"
#include "pingus/animation_clock.hpp"
#include "pingus/direction.hpp"
#include "util/reader.hpp"

namespace pingus {

/** Play a sound */
struct SoundEffect
{
  std::string name = {};
  float volume = 1.0f;
};

/** Emit particles at the object's position plus offset. 'kind' is one of
    the particle systems ("pingu", "smoke"), 'spread' adds a random
    offset between 0 and spread to each particle. */
struct ParticlesEffect
{
  std::string kind = {};
  int count = 1;
  Vector2f offset = {};
  Vector2f spread = {};
};

/** Draw an animation of the same set once, at the object's position
    plus offset */
struct OverlayEffect
{
  std::string animation = {};
  Vector2f offset = {};
};

using Effect = std::variant<SoundEffect, ParticlesEffect, OverlayEffect>;

/** An effect fired when an animation reaches a step: the step of its game
    timing, or the animation frame for purely visual animations. Step 0
    fires when the animation starts. Looping or restarted animations fire
    their effects again. */
struct EffectTrigger
{
  int step;
  Effect effect;
};

/** One named animation of an AnimationSet */
struct AnimationDef
{
  std::string name = {};

  /** Sprite resources for the two directions, the same for animations
      without a direction */
  std::string left = {};
  std::string right = {};

  /** Added to the position the animation is drawn at */
  Vector2f offset = {};

  /** Overrides the loop flag of the sprite resources */
  std::optional<bool> loop = {};

  std::vector<EffectTrigger> effects = {};

  std::string const& sprite_name(Direction const& dir) const { return dir.is_left() ? left : right; }

  /** A clock with the animation's timing, from the sprite metadata */
  AnimationClock make_clock() const;

  /** Separate clocks for the left and right sprites */
  DirectionalAnimationClock make_directional_clock() const;
};

/** Named animations an object can show, so that the game requests
    "kill" facing left instead of picking sprite resources itself.

    Loaded from data/animsets/NAME.animset:

      (pingus-animset
        (animations
          (idle
            (sprite "traps/guillotineidle"))      ; both directions
          (kill
            (left "traps/guillotinekill/left")
            (right "traps/guillotinekill/right")
            (offset 0 -2)                         ; optional
            (loop #f)                             ; optional
            (effects                              ; optional
              (sound (at-step 7) (name "splash") (volume 0.5))
              (particles (at-step 7) (kind "pingu") (count 3)
                         (offset 40 90) (spread 4 0))
              (overlay (at-step 0) (animation "flash")
                       (offset -32 -48))))))

    The effect types are a fixed vocabulary implemented in C++, the data
    only picks them and their parameters. */
class AnimationSet
{
public:
  /** The animation set with the given name, loaded once and cached.
      Throws if it doesn't exist. */
  static std::shared_ptr<AnimationSet const> get(std::string const& name);

  /** Load an animation set from an arbitrary .animset file path (not cached).
      Used by the asset viewer for reload; throws on error. */
  static AnimationSet from_file(Pathname const& path);

  static AnimationSet from_reader(ReaderObject const& reader);

public:
  AnimationSet();

  /** nullptr if there is no animation with that name */
  AnimationDef const* find(std::string_view name) const;

  /** Throws if there is no animation with that name */
  AnimationDef const& get_animation(std::string_view name) const;

  std::vector<AnimationDef> const& get_animations() const { return m_animations; }

private:
  std::vector<AnimationDef> m_animations;
};

} // namespace pingus

#endif

/* EOF */
