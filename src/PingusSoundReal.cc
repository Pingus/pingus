//  $Id: PingusSoundReal.cc,v 1.16 2001/12/04 12:18:50 grumbel Exp $
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

// Needed cause of missing include in some sound file...
#include <ClanLib/core.h>

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
  
  // Alle allozierten Sound Buffer wieder löschen
  for (unsigned int i = 0; i < sound_holder.size(); i++)
    delete sound_holder[i];
  
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
    std::cout << "Initializing ClanLib-Sound" << std::endl;
    
  CL_SetupSound::init();
  
  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "Initializing ClanLib-MikMod" << std::endl;
  
#ifdef HAVE_LIBCLANMIKMOD
  CL_SetupMikMod::init();
#endif
}

void
PingusSoundReal::real_play_sound(const std::string & filename, float volume, float panning)
{

  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "PingusSoundReal: Playing sound: " << filename << "Buffer-Size: " << sound_holder.size() << std::endl;

  if (!sound_enabled)
    return;

  

  std::vector<CL_SoundBuffer_Session *>::iterator it;

  // search for unused SoundBuffer_Sessions - clean them up
  for (it = sound_holder.begin(); it != sound_holder.end(); it++) {
    if (!(*it) -> is_playing()) {
      delete *it;
      sound_holder.erase(it);
    }
  }

  CL_SoundBuffer         * buffer;
  CL_SoundBuffer_Session * sess;
  
  try {
    buffer = new CL_SoundBuffer (new CL_Sample(filename.c_str(), NULL), true);
    sess   = new CL_SoundBuffer_Session(buffer -> prepare());
  } catch (const CL_Error & e) {
    if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
      std::cout << "Can't open file " << filename << " -- skipping\n";
    return;
  }
  
  sess -> set_volume(volume);
  sess -> set_pan(panning);
  sess -> set_looping(false);
  sess -> play();  

  sound_holder.push_back(sess);
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

  sample = 0;

  if (filename.substr(filename.size()-4, 4) == ".ogg") 
    {
#ifdef HAVE_LIBCLANVORBIS
      sample = new CL_SoundBuffer (new CL_VorbisSoundProvider(filename.c_str()), true);
#else
      sample = 0;
#endif

  } else if (filename.substr(filename.size()-4, 4) == ".wav") {

    sample = new CL_SoundBuffer (new CL_Sample(filename.c_str(), NULL), true);
        
  } else {  // MikMod unterstützt verschiedene Formate
  
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
