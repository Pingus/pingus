// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Integration tests running small levels through the World simulation.
// Needs the game data, ctest runs it from the source directory.

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "engine/display/display.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/ecs/components.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/resource.hpp"
#include "pingus/world.hpp"
#include "util/pathname.hpp"

using namespace pingus;

namespace {

class WorldTest : public ::testing::Test
{
protected:
  static void SetUpTestSuite()
  {
    static bool initialized = false;
    if (!initialized)
    {
      g_path_manager.set_path("data/");
      Resource::init();
      Display::create_window(FramebufferType::NULL_FRAMEBUFFER, geom::isize(640, 480), false, false);
      sound::PingusSound::init();
      initialized = true;
    }
  }

  /** Create a World for a level with the given objects. The level is 640x300
      with a solid floor at y=150 and an entrance at (100, 140) releasing
      'pingus' pingus to the right. */
  static std::unique_ptr<World> make_world(std::string const& objects, int pingus = 2)
  {
    std::string floor;
    for (int x = 0; x < 640; x += 64) {
      floor += "(groundpiece (type \"solid\") (position " + std::to_string(x) + " 150 0)"
        " (surface (image \"groundpieces/solid/misc/metalplate_horiz\") (modifier \"ROT0\")))\n";
    }

    std::string const level =
      "(pingus-level (version 3)\n"
      " (head (levelname \"test\") (description \"\") (author \"\")"
      "  (number-of-pingus " + std::to_string(pingus) + ") (number-to-save 0) (time -1)"
      "  (music \"none\") (actions) (levelsize 640 300))\n"
      " (objects\n"
      "  (solidcolor-background (colori 0 0 0 255))\n"
      "  (entrance (position 100 140 0) (direction \"right\") (release-rate 50))\n" +
      floor + objects + "))\n";

    std::filesystem::path const path = std::filesystem::temp_directory_path() / "pingus-world-test.pingus";
    std::ofstream(path) << level;
    PingusLevel plf(Pathname(path.string(), Pathname::SYSTEM_PATH));
    return std::make_unique<World>(plf);
  }

  static void run(World& world, int ticks)
  {
    for (int i = 0; i < ticks; ++i) {
      world.update();
    }
  }
};

} // namespace

TEST_F(WorldTest, pingus_walk_on_floor)
{
  auto world = make_world("", 1);
  run(*world, 300);
  PinguHolder* pingus = world->get_pingus();
  ASSERT_EQ(pingus->get_number_of_released(), 1);
  Pingu* pingu = pingus->get_pingu(0);
  ASSERT_NE(pingu, nullptr);
  EXPECT_EQ(pingu->get_action(), ActionName::WALKER);
  EXPECT_EQ(pingu->get_pos().y(), 149.0f);
  EXPECT_GT(pingu->get_pos().x(), 100.0f);
}

TEST_F(WorldTest, fake_exit_smashes_once_per_trigger)
{
  auto world = make_world("(fake_exit (position 300 151 0))");

  components::FakeExit* fake_exit = nullptr;
  world->get_registry().each<components::FakeExit>([&](ecs::Entity, components::FakeExit& f) {
    fake_exit = &f;
  });
  ASSERT_NE(fake_exit, nullptr);

  run(*world, 1500);

  // both pingus walked into the trap
  EXPECT_EQ(world->get_pingus()->get_number_of_killed(), 2);
  // and it went back to idle afterwards instead of smashing forever
  EXPECT_FALSE(fake_exit->smashing);
  EXPECT_EQ(fake_exit->clock.frame(), 0);
}

/* EOF */
