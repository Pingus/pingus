//  $Id: PingusSound.cc,v 1.16 2000/10/12 19:33:51 grumbel Exp $
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
#include "PingusSound.hh"

PingusSound* PingusSound::sound;

void
PingusSound::init (PingusSound* s)
{
  assert (sound == 0);
  sound = s;
}

void
PingusSound::play_mod(std::string filename)
{
  assert (sound);
  if (music_enabled) sound->real_play_mod (filename);
}

/** Load a wav and play it immediately.
    
    @param filename The complete filename, it will be passed to the
    PingusSoundProvider */
void 
PingusSound::play_wav(std::string filename)
{
  assert (sound);
  if (sound_enabled) sound->real_play_wav (filename);
}

/** Shut down the sound and the music and quit SDL */
void 
PingusSound::clean_up()
{
  assert (sound);
  sound->real_clean_up ();
}

/* PingusSoundDummy */

void
PingusSoundDummy::real_play_mod (std::string filename)
{
  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "PingusSoundDummy::real_play_mod: " << filename << std::endl;
}

void 
PingusSoundDummy::real_play_wav(std::string filename)
{
  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "PingusSoundDummy::real_play_wav: " << filename << std::endl;
}

void 
PingusSoundDummy::real_clean_up()
{
}

/* EOF */
