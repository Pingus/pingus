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

#ifndef HEADER_PINGUS_ENGINE_SOUND_SOUND_DUMMY_HPP
#define HEADER_PINGUS_ENGINE_SOUND_SOUND_DUMMY_HPP

#include "engine/sound/sound_impl.hpp"

namespace Sound {

class PingusSoundDummy : public PingusSoundImpl
{
private:
  float m_master_volume;
  float m_sound_volume;
  float m_music_volume;

public:
  PingusSoundDummy () :
    m_master_volume(1.0f),
    m_sound_volume(1.0f),
    m_music_volume(1.0f)
  {}

  virtual ~PingusSoundDummy () { }

  void real_play_sound (const std::string & filename, float volume, float panning);
  void real_play_music (const std::string & filename, float volume, bool loop);
  void real_stop_music();

  void set_sound_volume(float volume) { m_sound_volume = volume; }
  void set_music_volume(float volume) { m_music_volume = volume;  }
  void set_master_volume(float volume) { m_master_volume = volume; }

  float get_sound_volume() const { return m_sound_volume; }
  float get_music_volume() const { return m_music_volume; }
  float get_master_volume() const { return m_master_volume; }

private:
  PingusSoundDummy (const PingusSoundDummy&);
  PingusSoundDummy& operator= (const PingusSoundDummy&);
};

} // namespace Sound

#endif

/* EOF */
