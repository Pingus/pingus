// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "ecs/registry.hpp"

using namespace pingus::ecs;

namespace {

struct Position { float x; float y; };
struct Name { std::string name; };
struct Tag {};

} // namespace

TEST(RegistryTest, create_and_destroy)
{
  Registry reg;
  Entity a = reg.create();
  Entity b = reg.create();
  EXPECT_NE(a, b);
  EXPECT_TRUE(reg.valid(a));

  reg.emplace<Name>(a, "a");
  reg.destroy(a);
  EXPECT_FALSE(reg.valid(a));
  EXPECT_FALSE(reg.has<Name>(a));
  EXPECT_TRUE(reg.valid(b));

  // ids are never reused
  Entity c = reg.create();
  EXPECT_NE(c, a);
  EXPECT_FALSE(reg.valid(null_entity));
}

TEST(RegistryTest, components)
{
  Registry reg;
  Entity e = reg.create();
  reg.emplace<Position>(e, 1.0f, 2.0f);
  ASSERT_TRUE(reg.has<Position>(e));
  EXPECT_FALSE(reg.has<Name>(e));
  EXPECT_EQ(reg.get<Position>(e).y, 2.0f);
  EXPECT_EQ(reg.try_get<Name>(e), nullptr);

  reg.get<Position>(e).x = 5.0f;
  EXPECT_EQ(reg.get<Position>(e).x, 5.0f);

  reg.remove<Position>(e);
  EXPECT_FALSE(reg.has<Position>(e));
}

TEST(RegistryTest, each_in_creation_order)
{
  Registry reg;
  std::vector<Entity> entities;
  for (int i = 0; i < 5; ++i)
  {
    Entity e = reg.create();
    entities.push_back(e);
    reg.emplace<Position>(e, static_cast<float>(i), 0.0f);
    if (i % 2 == 0) {
      reg.emplace<Tag>(e);
    }
  }

  std::vector<float> visited;
  reg.each<Position, Tag>([&](Entity, Position& pos, Tag&) {
    visited.push_back(pos.x);
  });
  EXPECT_EQ(visited, (std::vector<float>{0.0f, 2.0f, 4.0f}));
}

TEST(RegistryTest, references_stay_valid_while_adding)
{
  Registry reg;
  Entity first = reg.create();
  Position& pos = reg.emplace<Position>(first, 1.0f, 1.0f);
  for (int i = 0; i < 1000; ++i) {
    reg.emplace<Position>(reg.create(), 0.0f, 0.0f);
  }
  pos.x = 42.0f;
  EXPECT_EQ(reg.get<Position>(first).x, 42.0f);
}

TEST(RegistryTest, destroy_during_each)
{
  Registry reg;
  for (int i = 0; i < 4; ++i) {
    reg.emplace<Position>(reg.create(), static_cast<float>(i), 0.0f);
  }

  int count = 0;
  reg.each<Position>([&](Entity e, Position&) {
    ++count;
    // destroy the following entity, it must be skipped
    Entity next{to_index(e) + 1};
    if (reg.valid(next)) {
      reg.destroy(next);
    }
  });
  EXPECT_EQ(count, 2);
}

/* EOF */
