//  $Id: PingusSoundReal.hh,v 1.2 2000/09/30 21:34:42 grumbel Exp $
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

#ifndef PINGUSSOUNDREAL_HH
#define PINGUSSOUND_HH

#include "audio.hh"
#include <string>

#include "PingusSound.hh"

/** A simple wrapper class around SDL_Mixer, it will init itself
    automatically if a sound is played. */
class PingusSoundReal : public PingusSound
{
private:
  ///
  bool is_init;
  /// 
  int audio_open;
  /// The current music file
  Mix_Music *music;

  /// Init SDL_mixer
  void init(int audio_rate, Uint16 audio_format,
		   int audio_channels, int audio_buffers);
public:
  PingusSoundReal () {}
  virtual ~PingusSoundReal () {}

  /** Load a mod and play it immediately.

      @param filename The complete filename, it will be passed to the
      PingusMusicProvider */
  virtual void real_play_mod(std::string filename);

  /** Load a wav and play it immediately.

      @param filename The complete filename, it will be passed to the
             PingusSoundProvider */
  virtual void real_play_wav(std::string filename);

  /** Shut down the sound and the music and quit SDL */
  virtual void real_clean_up();
};

#endif

/* EOF */
