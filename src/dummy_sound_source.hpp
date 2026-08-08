/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2010 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_WINDSTILLE_SOUND_DUMMY_SOUND_SOURCE_HPP
#define HEADER_WINDSTILLE_SOUND_DUMMY_SOUND_SOURCE_HPP

#include "sound_source.hpp"

namespace wstsound {

class DummySoundSource : public SoundSource
{
public:
  DummySoundSource() :
    m_state(SourceState::Paused)
  {}
  ~DummySoundSource() override {}

  void play() override { m_state = SourceState::Playing; }
  void pause() override { m_state = SourceState::Paused; }
  void finish() override { m_state = SourceState::Finished; }

  SourceState get_state() const override { return m_state; }

  float get_duration() const override { return 0.0f; }
  int get_sample_duration() const override { return 0; }

  void set_looping(bool looping) override {}
  void set_loop(int sample_beg, int sample_end) override {}

  /// Set volume (0.0 is silent, 1.0 is normal)
  void  set_gain(float gain) override {}
  float get_gain() const override { return 1.0f; }
  void set_pitch(float pitch) override {}

  void  seek_to(float sec) override {}
  void  seek_to_sample(int sample) override {}

  /** Return the current position in seconds */
  float get_pos() const override { return 0.0f; }

  int get_sample_pos() const override { return 0; }

  void set_relative(bool relative) override {}
  void set_position(float x, float y, float z) override {}
  void set_velocity(float x, float y, float z) override {}

  /** Distances closer then reference distance will increase the gain
      (or keep it at 1.0f), while distances further away will lower it */
  void set_reference_distance(float distance) override {}

  /** Higher factor will mean the sound gain lowers quicker with
      distance, while lower factor will reduce the lowering of gain,
      0.0f disables distance based reduction completly */
  void set_rolloff_factor(float factor) override {}

  void set_direct_filter(FilterPtr const& filter) override {}
  void set_effect_slot(EffectSlotPtr const& slot, FilterPtr const& filter = {}) override {}

  /** Needs to be called whenever the SoundChannels gain changes */
  void update_gain() const override {}

  void update(float delta) override {
    m_state = SourceState::Finished;
  }

  float sample_to_sec(int sample) const override {
    return 0.0f;
  }

  int sec_to_sample(float sec) const override {
    return 0;
  }

private:
  SourceState m_state;

private:
  DummySoundSource(const DummySoundSource&);
  DummySoundSource& operator=(const DummySoundSource&);
};

} // namespace wstsound

#endif

/* EOF */
