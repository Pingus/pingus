//  $Id: sound_real.hxx,v 1.4 2003/10/21 11:01:52 grumbel Exp $
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

#ifndef HEADER_PINGUS_SOUND_REAL_HXX
#define HEADER_PINGUS_SOUND_REAL_HXX

#include <config.h>
#include <vector>
#include "sound_impl.hxx"
#include "sound.hxx"

class CL_SoundBuffer;
class CL_SoundBuffer_Session;

namespace Sound {

/** A simple wrapper class around SDL_Mixer, it will init itself
    automatically if a sound is played. */
class PingusSoundReal : public PingusSoundImpl
{
private:
  /** The current music file */
  CL_SoundBuffer * music_sample;

  /** Music Controller Session */
  CL_SoundBuffer_Session* music_session;

  /** Sound Output object */
  CL_SoundOutput* sound_output;

public:
  PingusSoundReal ();
  virtual ~PingusSoundReal ();

  /** Load a music file and play it immediately.
      @param filename The complete filename
      @param volume   The volume to play the music with  */
  virtual void real_play_music(const std::string & filename, float volume);

  virtual void real_stop_music();

  /** Load a sound file and play it immediately
      @param filename The complete filename
      @param volume   The volume to play the sound at
      @param panning  The panning to play the sound with */
  virtual void real_play_sound(const std::string & filename, float volume, float panning);

private:
  PingusSoundReal (const PingusSoundReal&);
  PingusSoundReal& operator= (const PingusSoundReal&);
};

} // namespace Sound

#endif

/* EOF */
