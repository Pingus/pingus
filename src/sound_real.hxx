//  $Id: sound_real.hxx,v 1.9 2002/12/28 16:57:38 torangan Exp $
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

#ifndef HEADER_PINGUS_SOUND_REAL_HXX
#define HEADER_PINGUS_SOUND_REAL_HXX

#include <vector>
#include "sound.hxx"

class CL_SoundBuffer;
class CL_SoundBuffer_Session;

/** A simple wrapper class around SDL_Mixer, it will init itself
    automatically if a sound is played. */
class PingusSoundReal : public PingusSound
{
private:
  /** The current music file */
  CL_SoundBuffer * sample;

  /** Music Controller Session */
  CL_SoundBuffer_Session * music;

  /** Stores all Sound Effects */
  std::vector<CL_SoundBuffer_Session *> sound_holder;

public:
  PingusSoundReal ();
  virtual ~PingusSoundReal ();

  /** Load a music file and play it immediately.

      @param filename The complete filename
      @param volume   The volume to play the music with  */
  virtual void real_play_music(const std::string & filename, float volume);

  /** Load a sound file and play it immediately
  
      @param filename The complete filename
      @param volume   The volume to play the sound at 
      @param panning  The panning to play the sound with */

  virtual void real_play_sound(const std::string & filename, float volume, float panning);

private:
  PingusSoundReal (const PingusSoundReal&);
  PingusSoundReal& operator= (const PingusSoundReal&);  
};

#endif

/* EOF */
