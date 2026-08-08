/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2005 Matthias Braun <matze@braunis.de>
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

#ifndef HEADER_WINDSTILLE_SOUND_STREAM_SOUND_SOURCE_HPP
#define HEADER_WINDSTILLE_SOUND_STREAM_SOUND_SOURCE_HPP

#include <stdio.h>

#include <array>
#include <memory>
#include <optional>

#include "openal_sound_source.hpp"

namespace wstsound {

class SoundFile;
class SoundChannel;

class StreamSoundSource : public OpenALSoundSource
{
public:
  StreamSoundSource(SoundChannel& channel, std::unique_ptr<SoundFile> sound_file);
  ~StreamSoundSource() override;

  void play() override;
  void pause() override;
  void finish() override;

  SourceState get_state() const override { return m_state; }

  void update(float delta) override;

  void seek_to(float sec) override;
  void seek_to_sample(int sample) override;

  void set_looping(bool looping) override;
  void set_loop(int sample_beg, int sample_end) override;

  float get_pos() const override;
  float get_duration() const override;

  int get_sample_pos() const override;
  int get_sample_duration() const override;

  int sec_to_sample(float sec) const override;
  float sample_to_sec(int sample) const override;

private:
  void fill_buffer_and_queue(ALuint buffer);
  void update_queue();
  void clear_queue();

private:
  struct Loop
  {
    int sample_beg;
    int sample_end;
  };

private:
  static const size_t STREAMFRAGMENTS = 4;
  static const size_t STREAMFRAGMENTSIZE = 65536;

  std::unique_ptr<SoundFile> m_sound_file;
  std::array<ALuint, STREAMFRAGMENTS> m_buffers;
  bool m_buffers_queued;
  ALenum m_format;
  int m_total_samples_processed;
  SourceState m_state;

  std::optional<Loop> m_loop;

public:
  StreamSoundSource(const StreamSoundSource&) = delete;
  StreamSoundSource& operator=(const StreamSoundSource&) = delete;
};

} // namespace wstsound

#endif

/* EOF */
