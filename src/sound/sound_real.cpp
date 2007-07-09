//  $Id: sound_real.cxx,v 1.11 2003/10/21 11:01:52 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "SDL.h"
#include "SDL_mixer.h"
#include "../globals.hpp"
#include "../debug.hpp"
#include "sound_res_mgr.hpp"
#include "sound_real.hpp"

namespace Sound {

PingusSoundReal::PingusSoundReal ()
  : music_sample(0)
{
  pout(PINGUS_DEBUG_SOUND) << "Initializing SDL audio" << std::endl;

  if (SDL_Init(SDL_INIT_AUDIO) == -1)
    {
      std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
      throw SDL_GetError();
    }

  pout(PINGUS_DEBUG_SOUND) << "Initializing SDL_Mixer" << std::endl;

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
      std::cout << "Unable to initialize SDL_Mixer: " << Mix_GetError() << std::endl;
      throw Mix_GetError();
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
  if (!sound_enabled)
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
      Mix_Volume(channel, (int)(volume * MIX_MAX_VOLUME));
      if (panning != 0.0f)
        {
          Uint8 left = (panning < 0.0f) ? 255 : (Uint8)((panning - 1.0f) * -255);
          Uint8 right = (panning > 0.0f) ? 255 : (Uint8)((panning + 1.0f) * 255);
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
PingusSoundReal::real_play_music (const std::string & arg_filename, float volume)
{
  std::string filename;

  filename = arg_filename;

  if (!music_enabled)
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

  Mix_VolumeMusic((int)(volume * 0.5f * MIX_MAX_VOLUME)); // FIXME: music_volume
  Mix_PlayMusic(music_sample, -1);
}

} // namespace Sound

/* EOF */

