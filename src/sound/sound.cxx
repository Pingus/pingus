//  $Id: sound.cxx,v 1.3 2003/03/04 13:59:44 grumbel Exp $
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

#include <iostream>
#include <assert.h>
#include "../path_manager.hxx"
#include "../globals.hxx"
#include "sound_dummy.hxx"
#include "sound_real.hxx"

PingusSound* PingusSound::sound;

using namespace Sound;

void
PingusSound::init (PingusSound* s)
{
  if (s == 0)
    {
      if (sound_enabled || music_enabled) 
        {
          if (verbose)
            std::cout << "Init Sound" << std::endl;

          PingusSound::init (new PingusSoundReal ());
        }
      else
        {
          if (verbose)
            std::cout << "Sound disabled" << std::endl;
          PingusSound::init (new PingusSoundDummy ());
        }
    }
  else
    {
      sound = s;
    }
}

void
PingusSound::deinit ()
{
  delete sound;
  sound = 0;
}

/** Load a sound file and play it immediately.
    
    @param filename The complete filename */
   
void 
PingusSound::play_sound(const std::string& name, float volume, float panning)
{
  assert (sound);
  sound->real_play_sound(name, volume, panning);
}

void
PingusSound::play_sound(Sound::Name name, float volume, float panning)
{
  // FIXME: We need to return a handle to the sound

  // This should be configurable via a .xml file
  switch (name)
    {
    case Sound::DIGGER:
      play_sound (path_manager.complete ("sounds/digger.wav"), volume, panning);
      break;
    case Sound::PLOP:
      play_sound (path_manager.complete ("sounds/plop.wav"), volume, panning);
      break;
    case Sound::GOODIDEA:
      play_sound (path_manager.complete ("sounds/goodidea.wav"), volume, panning);
      break;
    case Sound::OHNO:
      play_sound (path_manager.complete ("sounds/ohno.wav"), volume, panning);
      break;
    case Sound::TICK:
      play_sound (path_manager.complete ("sounds/tick.wav"), volume, panning);
      break;
    }
}

/** Load a sound file and play it immediately.
    
    @param name
    @param volume   volume */
void 
PingusSound::play_music(const std::string & name, float volume)
{
  assert (sound);
  sound->real_play_music(path_manager.complete ("music/" + name), volume);
}

void
PingusSound::stop_music()
{
  assert(sound);
  sound->real_stop_music();
}

/* EOF */

