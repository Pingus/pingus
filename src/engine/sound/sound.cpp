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

#include "engine/sound/sound.hpp"

#include <assert.h>

#include "engine/sound/sound_dummy.hpp"
#include "engine/sound/sound_real.hpp"
#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "util/log.hpp"
#include "util/mem.hpp"

namespace Sound {

std::unique_ptr<PingusSoundImpl> PingusSound::sound;

void
PingusSound::init(std::unique_ptr<PingusSoundImpl> s)
{
  if (!s)
  {
    if (globals::sound_enabled || globals::music_enabled)
    {
      try {
        PingusSound::init(util::make_unique<PingusSoundReal>());
      } catch (const std::exception& err) {
        log_error("Sound Error: %1%", err.what());
        log_error("Sound will be disabled");
        PingusSound::init(util::make_unique<PingusSoundDummy>());
      }
    }
    else
    {
      PingusSound::init(util::make_unique<PingusSoundDummy>());
    }
  }
  else
  {
    sound = std::move(s);
  }
}

void
PingusSound::deinit ()
{
  sound = {};
}

/** Load a sound file and play it immediately.
    @param filename The complete filename
    @param volume The desired volume level
    @param panning The desired panning level (stereo only) */
void
PingusSound::play_sound(const std::string& name, float volume, float panning)
{
  assert(sound);
  sound->real_play_sound(name, volume, panning);
}

/** Load a sound file and play it immediately.

    @param name
    @param volume   volume */
void
PingusSound::play_music(const std::string & name, float volume, bool loop)
{
  assert (sound);
  sound->real_play_music(g_path_manager.complete ("music/" + name), volume, loop);
}

void
PingusSound::stop_music()
{
  assert(sound);
  sound->real_stop_music();
}

void
PingusSound::set_sound_volume(float volume)
{
  assert(sound);
  sound->set_sound_volume(volume);
}

void
PingusSound::set_music_volume(float volume)
{
  assert(sound);
  sound->set_music_volume(volume);
}

void
PingusSound::set_master_volume(float volume)
{
  assert(sound);
  sound->set_master_volume(volume);
}

float
PingusSound::get_sound_volume()
{
  assert(sound);
  return sound->get_sound_volume();
}

float
PingusSound::get_music_volume()
{
  assert(sound);
  return sound->get_music_volume();
}

float
PingusSound::get_master_volume()
{
  assert(sound);
  return sound->get_master_volume();
}

} // namespace Sound

/* EOF */

