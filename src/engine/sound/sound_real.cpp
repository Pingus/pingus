//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/sound/sound_real.hpp"

#include <SDL.h>
#include <stdexcept>

#include "engine/sound/sound_res_mgr.hpp"
#include "pingus/debug.hpp"
#include "pingus/globals.hpp"

namespace Sound {

PingusSoundReal::PingusSoundReal ()
  : music_sample(0)
{
  pout(PINGUS_DEBUG_SOUND) << "Initializing SDL audio" << std::endl;

  if (SDL_Init(SDL_INIT_AUDIO) == -1)
  {
    std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    throw std::runtime_error(SDL_GetError());
  }

  pout(PINGUS_DEBUG_SOUND) << "Initializing SDL_Mixer" << std::endl;

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
  {
    std::cout << "Unable to initialize SDL_Mixer: " << Mix_GetError() << std::endl;
    throw std::runtime_error(Mix_GetError());
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
  if (!globals::sound_enabled)
    return;

  SoundHandle chunk;

  chunk = SoundResMgr::load(name);
  if (!chunk)
  {
    perr(PINGUS_DEBUG_SOUND) << "Can't open sound '" << name << "' -- skipping\n"
                             << "  Mix_Error: " << Mix_GetError() << std::endl;
    return;
  }

  int channel = Mix_PlayChannel(-1, chunk, 0);
  if (channel != -1)
  {
    Mix_Volume(channel, static_cast<int>(volume * MIX_MAX_VOLUME));
    if (panning != 0.0f)
    {
      Uint8 left  = static_cast<Uint8>((panning < 0.0f) ? 255 : static_cast<Uint8>((panning - 1.0f) * -255));
      Uint8 right = static_cast<Uint8>((panning > 0.0f) ? 255 : static_cast<Uint8>((panning + 1.0f) * 255));
      Mix_SetPanning(channel, left, right);
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
PingusSoundReal::real_play_music (const std::string & arg_filename, float volume, bool loop)
{
  std::string filename;

  filename = arg_filename;

  if (!globals::music_enabled)
    return;

  pout(PINGUS_DEBUG_SOUND) << "PingusSoundReal: Playing music: " << filename << std::endl;

  real_stop_music();

  music_sample = Mix_LoadMUS(filename.c_str());
  if (!music_sample)
  {
    perr(PINGUS_DEBUG_SOUND) << "Can't load music: " << filename << "' -- skipping\n"
                             << "  Mix_Error: " << Mix_GetError() << std::endl;
    return;
  }

  Mix_VolumeMusic(static_cast<int>(volume * 0.5f * MIX_MAX_VOLUME)); // FIXME: music_volume
  Mix_PlayMusic(music_sample, loop ? -1 : 0);
}

} // namespace Sound

/* EOF */

