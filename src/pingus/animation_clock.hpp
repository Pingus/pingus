// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ANIMATION_CLOCK_HPP
#define HEADER_PINGUS_PINGUS_ANIMATION_CLOCK_HPP

#include <string>

#include "pingus/direction.hpp"

namespace pingus {

class Sprite;
class SpriteDescription;

/** Game-logic side of a sprite animation: tracks the current frame and
    whether a non-looping animation has finished, without touching any
    image data. Game logic queries the clock instead of the Sprite and the
    Sprite only mirrors the clock's frame when drawing, see apply_to().

    Timing is identical to Sprite::update(), i.e. every update() advances
    the animation by one game tick of kTickMillis. */
class AnimationClock
{
public:
  /** Milliseconds an animation advances per game tick, matches the
      default delta of Sprite::update() */
  static constexpr int kTickMillis = 33;

  /** Create a clock from the timing data of the given sprite resource
      (speed, array, loop), the image itself is not loaded */
  static AnimationClock from_sprite(std::string const& res_name);
  static AnimationClock from_description(SpriteDescription const& desc);

public:
  /** Static clock with a single frame that never finishes */
  AnimationClock();
  AnimationClock(int frame_delay, int frame_count, bool loop);

  /** Advance the animation by one game tick */
  void update();

  void restart();
  void finish();

  int frame() const { return m_frame; }
  int frame_count() const { return m_frame_count; }

  /** Current frame as a fraction of the frame count, in [0, 1) */
  float progress() const;

  /** True when a non-looping animation reached its end */
  bool is_finished() const { return m_finished; }
  bool is_looping() const { return m_loop; }
  void set_loop(bool loop) { m_loop = loop; }

  /** Set the sprite to show the clock's current frame */
  void apply_to(Sprite& sprite) const;

  /** Map a frame of a clock with 'frame_count' frames proportionally to an
      animation with 'art_frame_count' frames */
  static int map_frame(int frame, int frame_count, int art_frame_count)
  {
    if (frame_count <= 0 || art_frame_count <= 0) {
      return frame;
    }
    return frame * art_frame_count / frame_count;
  }

private:
  int m_frame_delay;
  int m_frame_count;
  bool m_loop;
  bool m_finished;
  int m_frame;
  int m_tick_count;
};

/** Separate clocks for the left and right variant of a sprite, for
    animations that only advance in the direction the pingu is facing */
class DirectionalAnimationClock
{
public:
  static DirectionalAnimationClock from_sprites(std::string const& left, std::string const& right)
  {
    return DirectionalAnimationClock(AnimationClock::from_sprite(left), AnimationClock::from_sprite(right));
  }

public:
  DirectionalAnimationClock(AnimationClock left, AnimationClock right) :
    m_left(left), m_right(right)
  {}

  AnimationClock& operator[](Direction const& dir) { return dir.is_left() ? m_left : m_right; }
  AnimationClock const& operator[](Direction const& dir) const { return dir.is_left() ? m_left : m_right; }

private:
  AnimationClock m_left;
  AnimationClock m_right;
};

} // namespace pingus

#endif

/* EOF */
