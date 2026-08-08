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

#include <fstream>

#include "mp3_sound_file.hpp"
#include "ogg_sound_file.hpp"
#include "opus_sound_file.hpp"
#include "wav_sound_file.hpp"

using namespace wstsound;

namespace {

size_t get_sample_duration(SoundFile& soundfile, size_t bytes) {
  return bytes / soundfile.get_format().get_channels() / (soundfile.get_format().get_bits_per_sample() / 8);
}

size_t get_real_size(SoundFile& soundfile) {
  size_t total_bytesread = 0;
  while (true) {
    std::array<char, 1024 * 64> buffer;
    size_t const bytesread = soundfile.read(buffer.data(), buffer.size());
    if (bytesread == 0) {
      break;
    }
    total_bytesread += bytesread;
  }
  return total_bytesread;
}

} // namespace

TEST(SoundFileTest, wav)
{
  auto fin = std::make_unique<std::ifstream>("data/sound.wav", std::ios::binary);
  WavSoundFile sound_file(std::move(fin));

  EXPECT_EQ(sound_file.get_size(), 22788);
  EXPECT_EQ(sound_file.get_format().get_rate(), 44100);
  EXPECT_EQ(sound_file.get_format().get_channels(), 1);
  EXPECT_EQ(sound_file.get_format().get_bits_per_sample(), 16);
  EXPECT_FLOAT_EQ(sound_file.get_duration(), 0.25836736f);

  size_t const real_byte_size = get_real_size(sound_file);
  size_t const real_sample_duration = get_sample_duration(sound_file, real_byte_size);
  EXPECT_EQ(sound_file.tell(), real_byte_size);
  EXPECT_EQ(sound_file.get_sample_duration(), real_sample_duration);
}

TEST(SoundFileTest, ogg)
{
  auto fin = std::make_unique<std::ifstream>("data/sound.ogg", std::ios::binary);
  OggSoundFile sound_file(std::move(fin));

  EXPECT_EQ(sound_file.get_size(), 22788);
  EXPECT_EQ(sound_file.get_format().get_rate(), 44100);
  EXPECT_EQ(sound_file.get_format().get_channels(), 1);
  EXPECT_EQ(sound_file.get_format().get_bits_per_sample(), 16);
  EXPECT_FLOAT_EQ(sound_file.get_duration(), 0.25836736f);

  size_t const real_byte_size = get_real_size(sound_file);
  size_t const real_sample_duration = get_sample_duration(sound_file, real_byte_size);
  EXPECT_EQ(sound_file.tell(), real_byte_size);
  EXPECT_EQ(sound_file.get_sample_duration(), real_sample_duration);
}

TEST(SoundFileTest, opus)
{
  auto fin = std::make_unique<std::ifstream>("data/sound.opus", std::ios::binary);
  OpusSoundFile sound_file(std::move(fin));

  EXPECT_EQ(sound_file.get_size(), 24804);
  EXPECT_EQ(sound_file.get_format().get_rate(), 48000);
  EXPECT_EQ(sound_file.get_format().get_channels(), 1);
  EXPECT_EQ(sound_file.get_format().get_bits_per_sample(), 16);
  EXPECT_FLOAT_EQ(sound_file.get_duration(), 0.25837499f);

  size_t const real_byte_size = get_real_size(sound_file);
  size_t const real_sample_duration = get_sample_duration(sound_file, real_byte_size);
  EXPECT_EQ(sound_file.tell(), real_byte_size);
  EXPECT_EQ(sound_file.get_sample_duration(), real_sample_duration);
}

TEST(SoundFileTest, mp3)
{
  auto fin = std::make_unique<std::ifstream>("data/sound.mp3", std::ios::binary);
  MP3SoundFile sound_file(std::move(fin));

  EXPECT_EQ(sound_file.get_size(), 22788);
  EXPECT_EQ(sound_file.get_format().get_rate(), 44100);
  EXPECT_EQ(sound_file.get_format().get_channels(), 1);
  EXPECT_EQ(sound_file.get_format().get_bits_per_sample(), 16);
  EXPECT_FLOAT_EQ(sound_file.get_duration(), 0.25836736f);

  size_t const real_byte_size = get_real_size(sound_file);
  size_t const real_sample_duration = get_sample_duration(sound_file, real_byte_size);
  EXPECT_EQ(sound_file.tell(), real_byte_size);
  EXPECT_EQ(sound_file.get_sample_duration(), real_sample_duration);
}

/* EOF */
