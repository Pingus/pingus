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

#include "engine/sound/sound_real.hpp"

#include <SDL.h>
#include <stdexcept>

#include "engine/sound/sound_res_mgr.hpp"
#include "pingus/globals.hpp"
#include "util/log.hpp"
#include "util/raise_exception.hpp"

namespace Sound {

PingusSoundReal::PingusSoundReal() :
  music_sample(0),
  m_music_volume(1.0f),
  m_sound_volume(1.0f),
  m_master_volume(1.0f)
{
  log_info("Initializing SDL audio");

  if (SDL_Init(SDL_INIT_AUDIO) == -1)
  {
    raise_exception(std::runtime_error, "Unable to initialize SDL: " << SDL_GetError());
  }

  log_info("Initializing SDL_Mixer");

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
  {
    raise_exception(std::runtime_error, "Unable to initialize SDL_Mixer: " << Mix_GetError());
  }
}

PingusSoundReal::~PingusSoundReal()
{
  real_stop_music();
  SoundResMgr::free_sound_map();

  Mix_CloseAudio();
}

void
PingusSoundReal::real_play_sound(const std::string& name, float volume, float panning)
{
  if (globals::sound_enabled &&
      m_master_volume > 0 &&
      m_sound_volume > 0)
  {
    SoundHandle chunk;

    chunk = SoundResMgr::load(name);
    if (!chunk)
    {
      log_error("Can't open sound '%1%' -- skipping\n  Mix_Error: %2%", name, Mix_GetError());
      return;
    }

    int channel = Mix_PlayChannel(-1, chunk, 0);
    if (channel != -1)
    {
      Mix_Volume(channel, static_cast<int>(MIX_MAX_VOLUME * volume * m_sound_volume * m_master_volume));
      if (panning != 0.0f)
      {
        Uint8 left  = static_cast<Uint8>((panning < 0.0f) ? 255 : static_cast<Uint8>((panning - 1.0f) * -255));
        Uint8 right = static_cast<Uint8>((panning > 0.0f) ? 255 : static_cast<Uint8>((panning + 1.0f) * 255));
        Mix_SetPanning(channel, left, right);
      }
    }
  }
}

void
PingusSoundReal::real_stop_music ()
{
  if (music_sample)
  {
    Mix_HaltMusic();
    Mix_FreeMusic(music_sample);
    music_sample = 0;
  }
}

void
PingusSoundReal::real_play_music(const std::string& filename, float volume, bool loop)
{
  if (globals::music_enabled &&
      m_master_volume > 0 &&
      m_music_volume > 0)
  {
    log_info("PingusSoundReal: Playing music: %1%", filename);

    real_stop_music();

    music_sample = Mix_LoadMUS(filename.c_str());
    if (!music_sample)
    {
      log_error("Can't load music: %1%' -- skipping\n  Mix_Error: %2%", filename, Mix_GetError());
      return;
    }

    Mix_PlayMusic(music_sample, loop ? -1 : 0);
    Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * volume * m_music_volume * m_master_volume));
  }
}

void
PingusSoundReal::set_sound_volume(float volume)
{
  m_sound_volume = volume;
  apply_volume_changes();
}

void
PingusSoundReal::set_music_volume(float volume)
{
  m_music_volume = volume;
  apply_volume_changes();
}

void
PingusSoundReal::set_master_volume(float volume)
{
  m_master_volume = volume;
  apply_volume_changes();
}

void
PingusSoundReal::apply_volume_changes()
{
  int sound_volume = static_cast<int>(MIX_MAX_VOLUME * m_sound_volume * m_master_volume);
  int music_volume = static_cast<int>(MIX_MAX_VOLUME * m_music_volume * m_master_volume);

  Mix_Volume(-1, sound_volume);
  Mix_VolumeMusic(music_volume);
}

float
PingusSoundReal::get_sound_volume() const
{
  return m_sound_volume;
}

float
PingusSoundReal::get_music_volume() const
{
  return m_music_volume;
}

float
PingusSoundReal::get_master_volume() const
{
  return m_master_volume;
}

} // namespace Sound

/* EOF */

