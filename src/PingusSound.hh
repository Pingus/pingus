//  $Id: PingusSound.hh,v 1.11 2000/06/19 07:26:08 grumbel Exp $
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

#ifndef PINGUSSOUND_HH
#define PINGUSSOUND_HH

#include "audio.hh"
#include <string>

/** A simple wrapper class around SDL_Mixer, it will init itself
    automatically if a sound is played. */
class PingusSound
{
private:
  ///
  static bool is_init;
  /// 
  static int audio_open;
  /// The current music file
  static Mix_Music *music;

  /// Init SDL_mixer
  static void init(int audio_rate, Uint16 audio_format,
		   int audio_channels, int audio_buffers);
public:
  /** Load a mod and play it immediately.

      @param filename The complete filename, it will be passed to the
      PingusMusicProvider */
  static void play_mod(std::string filename);

  /** Load a wav and play it immediately.

      @param filename The complete filename, it will be passed to the
             PingusSoundProvider */
  static void play_wav(std::string filename);

  /** Shut down the sound and the music and quit SDL */
  static void clean_up();
};

#endif

/* EOF */
