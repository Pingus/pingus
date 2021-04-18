// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_ENGINE_SOUND_SOUND_REAL_HPP
#define HEADER_PINGUS_ENGINE_SOUND_SOUND_REAL_HPP

#include <memory>
#include <wstsound/fwd.hpp>

#include "engine/sound/sound_impl.hpp"

namespace Sound {

/** A simple wrapper class around SDL_Mixer, it will init itself
    automatically if a sound is played. */
class PingusSoundReal : public PingusSoundImpl
{
public:
  PingusSoundReal();
  ~PingusSoundReal() override;

  void update(float delta) override;

  /** Load a music file and play it immediately.
      @param filename The complete filename
      @param volume   The volume to play the music with
      @param loop     The music file should loop continuously  */
  void real_play_music(const std::string & filename, float volume, bool loop) override;

  void real_stop_music() override;

  /** Load a sound file and play it immediately
      @param filename The complete filename
      @param volume   The volume to play the sound at
      @param panning  The panning to play the sound with */
  void real_play_sound(const std::string & filename, float volume, float panning) override;

  void set_sound_volume(float volume) override;
  void set_music_volume(float volume) override;
  void set_master_volume(float volume) override;

  float get_sound_volume() const override;
  float get_music_volume() const override;
  float get_master_volume() const override;

private:
  void apply_volume_changes();

private:
  std::unique_ptr<wstsound::SoundManager> m_sound_manager;

  /** The current music file */
  // Mix_Music* music_sample;
  wstsound::SoundSourcePtr m_music_source;

  float m_music_volume;
  float m_sound_volume;
  float m_master_volume;

private:
  PingusSoundReal (const PingusSoundReal&);
  PingusSoundReal& operator= (const PingusSoundReal&);
};

} // namespace Sound

#endif

/* EOF */
