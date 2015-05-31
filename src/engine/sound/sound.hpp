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

#ifndef HEADER_PINGUS_ENGINE_SOUND_SOUND_HPP
#define HEADER_PINGUS_ENGINE_SOUND_SOUND_HPP

#include <memory>
#include <string>

#include "engine/sound/sound_impl.hpp"

namespace Sound {

class PingusSoundImpl;

class PingusSound
{
private:
  static std::unique_ptr<PingusSoundImpl> sound;

public:
  PingusSound  () { }
  ~PingusSound () { }

  static void init(std::unique_ptr<PingusSoundImpl> s = {});
  static void deinit();

  /** Load a sound file and play it immediately.

      @param name     Name of the sound, aka 'ohno'
      @param volume   volume
      @param panning  panning */
  static void play_sound(const std::string & name, float volume = 1.0f, float panning = 0.0f);

  static void play_music(const std::string & name, float volume = 1.0f, bool loop = true);
  static void stop_music();

  static void set_sound_volume(float volume);
  static void set_music_volume(float volume);
  static void set_master_volume(float volume);

  static float get_sound_volume();
  static float get_music_volume();
  static float get_master_volume();

private:
  PingusSound (const PingusSound&);
  PingusSound& operator= (const PingusSound&);
};

} // namespace Sound

#endif

/* EOF */
