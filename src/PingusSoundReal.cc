//  $Id: PingusSoundReal.cc,v 1.13 2001/11/18 12:43:19 grumbel Exp $
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

#include <config.h>

#include <string>
#include <iostream>

#ifdef HAVE_LIBCLANVORBIS
#  include <ClanLib/vorbis.h>
#endif

#ifdef HAVE_LIBCLANMIKMOD
#  include <ClanLib/mikmod.h>
#endif

#include "globals.hh"
#include "PingusError.hh"
#include "PingusSoundReal.hh"

PingusSoundReal::PingusSoundReal ()
  : sample (0), music (0)
{
  if (music_enabled || sound_enabled)
    init();
}

PingusSoundReal::~PingusSoundReal()
{
  if (music) {
    if (music -> is_playing())
      music -> stop();
    delete music;
  }
  
  if (is_init) {

#ifdef HAVE_LIBCLANMIKMOD
    CL_SetupMikMod::deinit();
#endif

    CL_SetupSound::deinit();
  
  }
}


void
PingusSoundReal::init()
{

  is_init = true;

  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    cout << "Initializing ClanLib-Sound" << endl;
    
  CL_SetupSound::init();
  
  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    cout << "Initializing ClanLib-MikMod" << endl;
  
#ifdef HAVE_LIBCLANMIKMOD
  CL_SetupMikMod::init();
#endif
}

void
PingusSoundReal::real_play_sound(const std::string & filename, 
				 float volume, 
				 float panning)
{
  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "PingusSoundReal: Playing sound: " << filename << std::endl;

  if (!sound_enabled)
    return;

  // create a new CL_SoundBuffer_Session if there is none for the requested effect yet
  if (!sounds.count(filename)) {
    CL_SoundBuffer * sound = new CL_SoundBuffer (new CL_Sample(filename.c_str(), NULL), true);
    sounds[filename] = CL_SoundBuffer_Session(sound -> prepare());
  }
  
  sounds[filename].set_volume(volume);
  sounds[filename].set_pan(panning);
  sounds[filename].set_looping(false);
  sounds[filename].play();  
}

void
PingusSoundReal::real_play_music(const std::string & filename, float volume)
{
  if (!music_enabled)
    return;

  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "PingusSoundReal: Playing music: " << filename << std::endl;
  
  if (music) {
    if (music -> is_playing())
      music -> stop();
    delete music;
    music = NULL;
  }

  if (filename.substr(filename.size()-4, 4) == ".ogg") 
    {
#ifdef HAVE_LIBCLANVORBIS
      sample = new CL_SoundBuffer (new CL_VorbisSoundProvider(filename.c_str()), true);
#else
      sample = 0;
#endif
    } 
  else if (filename.substr(filename.size()-4, 4) == ".wav") 
    {
      std::cout << "Überspringe .WAV" << std::endl;
      sample = new CL_SoundBuffer (new CL_Sample(filename.c_str(), NULL), true);
      return;
    } 
  else // MikMod unterst<FC>tzt verschiedene Formate
    { 
#ifdef HAVE_LIBCLANMIKMOD
      sample = new CL_SoundBuffer (new CL_Streamed_MikModSample(filename.c_str()), true);
#else
      sample = 0;
#endif    
    }
  
  if (sample)
    {
      music = new CL_SoundBuffer_Session(sample -> prepare());
  
      music -> set_volume(volume);
      music -> set_looping(true);
      music -> play();
    }
}


/* EOF */

