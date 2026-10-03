// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ANIMATION_SET_HPP
#define HEADER_PINGUS_PINGUS_ANIMATION_SET_HPP

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "math/vector2f.hpp"
#include "pingus/animation_clock.hpp"
#include "pingus/direction.hpp"
#include "util/reader.hpp"

namespace pingus {

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

  std::string const& sprite_name(Direction const& dir) const { return dir.is_left() ? left : right; }

  /** A clock with the animation's timing, from the sprite metadata */
  AnimationClock make_clock() const;
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
            (loop #f))))                          ; optional
*/
class AnimationSet
{
public:
  /** The animation set with the given name, loaded once and cached.
      Throws if it doesn't exist. */
  static std::shared_ptr<AnimationSet const> get(std::string const& name);

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
