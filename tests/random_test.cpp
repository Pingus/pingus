// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "math/random.hpp"

using namespace pingus;

TEST(RandomTest, same_seed_same_sequence)
{
  Random a(1234);
  Random b(1234);
  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(a.next_int(1000), b.next_int(1000));
  }
}

TEST(RandomTest, portable_sequence)
{
  // mt19937 is fully specified, the first output for seed 5489 is fixed
  Random rng(5489);
  EXPECT_EQ(rng.next_int(1000000000), 3499211612 % 1000000000);
}

TEST(RandomTest, ranges)
{
  Random rng(42);
  for (int i = 0; i < 1000; ++i)
  {
    int const n = rng.next_int(7);
    EXPECT_GE(n, 0);
    EXPECT_LT(n, 7);

    float const f = rng.next_float();
    EXPECT_GE(f, 0.0f);
    EXPECT_LT(f, 1.0f);
  }
  EXPECT_EQ(rng.next_int(0), 0);
}

TEST(RandomTest, seed_from_string)
{
  EXPECT_EQ(Random::seed_from_string(""), 2166136261u);
  EXPECT_NE(Random::seed_from_string("level1"), Random::seed_from_string("level2"));
}

/* EOF */
