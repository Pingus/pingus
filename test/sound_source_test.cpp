/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>

#include <wstsound/sound_file.hpp>
#include <wstsound/sound_manager.hpp>

#include "dummy_sound_source.hpp"
#include "sound_source.hpp"
#include "static_sound_source.hpp"
#include "stream_sound_source.hpp"

using namespace wstsound;

class SoundSourceTest : public ::testing::TestWithParam<std::string> {};

TEST(SoundSourceTest, dummy_creation)
{
  SoundManager mgr;
  auto static_source = mgr.sound().prepare("data/sound.wav", SoundSourceType::STATIC);
  auto stream_source = mgr.sound().prepare("data/sound.wav", SoundSourceType::STREAM);
  auto nonexistant_source = mgr.sound().prepare("data/does_not_exist.wav");

  EXPECT_TRUE(static_source);
  EXPECT_TRUE(stream_source);
  EXPECT_TRUE(nonexistant_source);

  if (mgr.is_dummy()) {
    EXPECT_TRUE(dynamic_cast<DummySoundSource*>(static_source.get()) != nullptr);
    EXPECT_TRUE(dynamic_cast<DummySoundSource*>(stream_source.get()) != nullptr);
    EXPECT_TRUE(dynamic_cast<DummySoundSource*>(nonexistant_source.get()) != nullptr);
  } else {
    EXPECT_TRUE(dynamic_cast<StaticSoundSource*>(static_source.get()) != nullptr);
    EXPECT_TRUE(dynamic_cast<StreamSoundSource*>(stream_source.get()) != nullptr);
    EXPECT_TRUE(dynamic_cast<DummySoundSource*>(nonexistant_source.get()) != nullptr);
  }
}

TEST_P(SoundSourceTest, duration)
{
  SoundManager mgr;
  if (mgr.is_dummy()) { return; }

  std::string const& filename = GetParam();

  auto sound_file = SoundFile::from_file(filename);
  auto static_source = mgr.sound().prepare(filename, SoundSourceType::STATIC);
  auto stream_source = mgr.sound().prepare(filename, SoundSourceType::STREAM);

  EXPECT_EQ(sound_file->get_duration(), static_source->get_duration());
  EXPECT_EQ(sound_file->get_duration(), stream_source->get_duration());
}

INSTANTIATE_TEST_CASE_P(
  SoundSourceTests,
  SoundSourceTest,
  ::testing::Values(
    "data/sound.wav",
    "data/sound.ogg",
    "data/sound.opus"
    ));

/* EOF */
