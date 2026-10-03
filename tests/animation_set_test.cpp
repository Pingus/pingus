// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "pingus/animation_set.hpp"
#include "pingus/path_manager.hpp"

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

TEST(AnimationSetTest, effects)
{
  ReaderDocument doc = ReaderDocument::from_string(
    "(pingus-animset"
    "  (animations"
    "    (bomber (sprite \"a\")"
    "      (effects"
    "        (sound (at-step 10) (name \"plop\") (volume 0.5))"
    "        (particles (at-step 13) (kind \"smoke\") (count 20) (offset 20 180) (spread 260 0))"
    "        (overlay (at-step 13) (animation \"explosion\") (offset -32 -48))))))");
  AnimationSet set = AnimationSet::from_reader(doc.get_root());
  auto const& effects = set.get_animation("bomber").effects;
  ASSERT_EQ(effects.size(), 3u);

  EXPECT_EQ(effects[0].step, 10);
  auto const& sound = std::get<SoundEffect>(effects[0].effect);
  EXPECT_EQ(sound.name, "plop");
  EXPECT_FLOAT_EQ(sound.volume, 0.5f);

  EXPECT_EQ(effects[1].step, 13);
  auto const& particles = std::get<ParticlesEffect>(effects[1].effect);
  EXPECT_EQ(particles.kind, "smoke");
  EXPECT_EQ(particles.count, 20);
  EXPECT_EQ(particles.offset, Vector2f(20, 180));
  EXPECT_EQ(particles.spread, Vector2f(260, 0));

  auto const& overlay = std::get<OverlayEffect>(effects[2].effect);
  EXPECT_EQ(overlay.animation, "explosion");
  EXPECT_EQ(overlay.offset, Vector2f(-32, -48));
}

TEST(AnimationSetTest, invalid_effects)
{
  auto load = [](std::string const& effect) {
    ReaderDocument doc = ReaderDocument::from_string(
      "(pingus-animset (animations (a (sprite \"a\") (effects " + effect + "))))");
    return AnimationSet::from_reader(doc.get_root());
  };
  EXPECT_THROW(load("(sound (name \"x\"))"), std::runtime_error);                   // no at-step
  EXPECT_THROW(load("(explode (at-step 1))"), std::runtime_error);                  // unknown effect
  EXPECT_THROW(load("(particles (at-step 1) (kind \"fire\"))"), std::runtime_error); // unknown kind
}

TEST(AnimationSetTest, game_data_animation_sets_load)
{
  // all animation sets shipped with the game must parse, needs the data
  // directory (ctest runs from the source directory)
  g_path_manager.set_path("data/");
  for (auto const& name : {"pingus/player0", "pingus/player1", "pingus/player2", "pingus/player3",
                           "traps/spike", "traps/fake_exit", "traps/guillotine", "traps/laser_exit",
                           "traps/hammer", "traps/smasher", "worldobjs/teleporter", "worldobjs/teleporter-target",
                           "worldobjs/iceblock", "worldobjs/conveyorbelt", "worldobjs/switchdoor-door",
                           "worldobjs/switchdoor-switch"})
  {
    EXPECT_NO_THROW(AnimationSet::get(name)) << name;
  }
}

/* EOF */
