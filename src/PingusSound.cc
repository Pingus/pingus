//  $Id: PingusSound.cc,v 1.5 2000/04/14 17:38:13 grumbel Exp $
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

#include <string>

#include "PingusSound.hh"

int PingusSound::audio_open;
Mix_Music* PingusSound::music;

void
PingusSound::init(int audio_rate, Uint16 audio_format,
		  int audio_channels, int audio_buffers)
{
  music = 0;

  printf("Initint music\n");

  if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) 
    {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(255);
    }
  printf("SDL init...\n");

  /* Open the audio device */
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
    fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
    exit(2);
  } else {
    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
    printf("Opened audio at %d Hz %d bit %s, %d bytes audio buffer\n", audio_rate,
	   (audio_format&0xFF),
	   (audio_channels > 1) ? "stereo" : "mono", 
	   audio_buffers );
    atexit(clean_up);
  }
  audio_open = 1;

  Mix_SetMusicCMD(getenv("MUSIC_CMD"));

  printf("SDL init... done\n");
}

void
PingusSound::clean_up()
{
  if( Mix_PlayingMusic() ) 
    {
      Mix_FadeOutMusic(1500);
      SDL_Delay(1500);
    }
  
  if ( music ) 
    {
      Mix_FreeMusic(music);
      music = NULL;
    }
  
  if ( audio_open ) 
    {
      Mix_CloseAudio();
      audio_open = 0;
    }
  SDL_Quit();
}

void
PingusSound::play(string filename)
{
  if (music)
    {
      Mix_FadeOutMusic(1000);
      Mix_FreeMusic(music);
    }
  
  printf("Playing...\n");
  music = Mix_LoadMUS(filename.c_str());

  if ( music == NULL ) 
    {
      fprintf(stderr, "Couldn't load %s: %s\n",
	      filename.c_str(), SDL_GetError());
      exit(2);
    } 
  Mix_FadeInMusic(music,-1,2000);
  printf("Playing...now\n");
}

void
PingusSound::keep_alive()
{
  
}

/* EOF */
