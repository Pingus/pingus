//  $Id: PingusSound.cc,v 1.18 2001/04/03 10:45:49 grumbel Exp $
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

#include "globals.hh"
#include "PingusSoundDummy.hh"
#include "PingusSound.hh"

PingusSound* PingusSound::sound;

void
PingusSound::init (PingusSound* s)
{
  assert (sound == 0);
  sound = s;
}

void
PingusSound::play_mod(std::string filename, float volume)
{
  assert (sound);
  if (music_enabled) sound->real_play_mod (filename, volume);
}

/** Load a wav and play it immediately.
    
    @param filename The complete filename, it will be passed to the
    PingusSoundProvider */
void 
PingusSound::play_wav(std::string filename, float volume, float panning)
{
  assert (sound);
  if (sound_enabled) sound->real_play_wav (filename, volume, panning);
}

/** Shut down the sound and the music and quit SDL */
void 
PingusSound::clean_up()
{
  assert (sound);
  sound->real_clean_up ();
}

/* EOF */
