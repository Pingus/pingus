// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "pingus/animation_set.hpp"

using namespace pingus;

TEST(AnimationSetTest, from_reader)
{
  ReaderDocument doc = ReaderDocument::from_string(
    "(pingus-animset"
    "  (animations"
    "    (idle (sprite \"traps/guillotineidle\"))"
    "    (kill (left \"a/left\") (right \"a/right\") (offset 3 -2) (loop #f))))");
  AnimationSet set = AnimationSet::from_reader(doc.get_root());

  ASSERT_EQ(set.get_animations().size(), 2u);

  AnimationDef const& idle = set.get_animation("idle");
  EXPECT_EQ(idle.left, "traps/guillotineidle");
  EXPECT_EQ(idle.right, "traps/guillotineidle");
  EXPECT_FALSE(idle.loop.has_value());

  AnimationDef const& kill = set.get_animation("kill");
  Direction left;
  left.left();
  Direction right;
  right.right();
  EXPECT_EQ(kill.sprite_name(left), "a/left");
  EXPECT_EQ(kill.sprite_name(right), "a/right");
  EXPECT_EQ(kill.offset, Vector2f(3, -2));
  EXPECT_EQ(kill.loop, std::optional<bool>(false));

  EXPECT_EQ(set.find("missing"), nullptr);
  EXPECT_THROW(set.get_animation("missing"), std::runtime_error);
}

TEST(AnimationSetTest, invalid_animation)
{
  ReaderDocument doc = ReaderDocument::from_string(
    "(pingus-animset (animations (broken (left \"only/left\"))))");
  EXPECT_THROW(AnimationSet::from_reader(doc.get_root()), std::runtime_error);
}

/* EOF */
