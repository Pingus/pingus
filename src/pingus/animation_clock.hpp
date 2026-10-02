// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ANIMATION_CLOCK_HPP
#define HEADER_PINGUS_PINGUS_ANIMATION_CLOCK_HPP

#include <string>

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

  /** Set the sprite to show the clock's current frame */
  void apply_to(Sprite& sprite) const;

private:
  int m_frame_delay;
  int m_frame_count;
  bool m_loop;
  bool m_finished;
  int m_frame;
  int m_tick_count;
};

} // namespace pingus

#endif

/* EOF */
