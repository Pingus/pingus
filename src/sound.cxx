//  $Id: sound.cxx,v 1.5 2002/08/22 00:36:30 grumbel Exp $
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

#include <assert.h>
#include "path_manager.hxx"
#include "sound_dummy.hxx"

PingusSound* PingusSound::sound;

void
PingusSound::init (PingusSound* s)
{
  assert (sound == 0);
  sound = s;
}


/** Load a sound file and play it immediately.
    
    @param filename The complete filename */
   
void 
PingusSound::play_sound(const std::string & filename, float volume, float panning)
{
  assert (sound);
  sound->real_play_sound (path_manager.complete (filename), volume, panning);
}

void
PingusSound::play_sound(Sound::Name name, float volume, float panning)
{
  // FIXME: We need to return a handle to the sound

  // This should be configurable via a .xml file
  switch (name)
    {
    case Sound::DIGGER:
      play_sound (path_manager.complete ("sounds/digger.wav"));
      break;
    case Sound::PLOP:
      play_sound (path_manager.complete ("sounds/plop.wav"));
      break;
    case Sound::GOODIDEA:
      play_sound (path_manager.complete ("sounds/goodidea.wav"));
      break;
    case Sound::OHNO:
      play_sound (path_manager.complete ("sounds/ohno.wav"));
      break;
    case Sound::TICK:
      play_sound (path_manager.complete ("sounds/tick.wav"));
      break;
    }
}

/** Load a sound file and play it immediately.
    
    @param filename The complete filename 
    @param volume   volume */
    
void 
PingusSound::play_music(const std::string & filename, float volume)
{
  assert (sound);
  sound->real_play_music(path_manager.complete (filename), volume);
}


/* EOF */

