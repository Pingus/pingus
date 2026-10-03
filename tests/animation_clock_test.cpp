// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "engine/display/sprite_description.hpp"
#include "pingus/animation_clock.hpp"

using namespace pingus;

TEST(AnimationClockTest, static_clock_never_advances)
{
  AnimationClock clock;
  for (int i = 0; i < 100; ++i) {
    clock.update();
  }
  EXPECT_EQ(clock.frame(), 0);
  EXPECT_FALSE(clock.is_finished());
}

TEST(AnimationClockTest, frames_advance_by_tick)
{
  // 66ms per frame, two ticks of 33ms per frame
  AnimationClock clock(66, 4, true);
  EXPECT_EQ(clock.frame(), 0);
  clock.update();
  EXPECT_EQ(clock.frame(), 0);
  clock.update();
  EXPECT_EQ(clock.frame(), 1);
  clock.update();
  clock.update();
  EXPECT_EQ(clock.frame(), 2);
  EXPECT_FLOAT_EQ(clock.progress(), 0.5f);
}

TEST(AnimationClockTest, looping_wraps_around)
{
  AnimationClock clock(33, 3, true);
  clock.update();
  clock.update();
  EXPECT_EQ(clock.frame(), 2);
  clock.update();
  EXPECT_EQ(clock.frame(), 0);
  EXPECT_FALSE(clock.is_finished());
}

TEST(AnimationClockTest, non_looping_finishes_on_last_frame)
{
  AnimationClock clock(33, 3, false);
  clock.update();
  clock.update();
  EXPECT_EQ(clock.frame(), 2);
  EXPECT_FALSE(clock.is_finished());
  clock.update();
  EXPECT_TRUE(clock.is_finished());
  EXPECT_EQ(clock.frame(), 2);

  clock.restart();
  EXPECT_FALSE(clock.is_finished());
  EXPECT_EQ(clock.frame(), 0);
}

TEST(AnimationClockTest, from_description)
{
  SpriteDescription desc;
  desc.speed = 50;
  desc.array = geom::isize(4, 2);
  desc.loop = false;

  AnimationClock clock = AnimationClock::from_description(desc);
  EXPECT_EQ(clock.frame_count(), 8);
  EXPECT_FALSE(clock.is_looping());
}

TEST(AnimationClockTest, map_frame)
{
  // same number of frames: unchanged
  for (int i = 0; i < 15; ++i) {
    EXPECT_EQ(AnimationClock::map_frame(i, 15, 15), i);
  }
  // art with fewer or more frames than the game timing
  EXPECT_EQ(AnimationClock::map_frame(7, 15, 5), 2);
  EXPECT_EQ(AnimationClock::map_frame(14, 15, 5), 4);
  EXPECT_EQ(AnimationClock::map_frame(3, 4, 8), 6);
  // no game timing: the frame is an art frame
  EXPECT_EQ(AnimationClock::map_frame(3, 0, 8), 3);
}

/* EOF */
