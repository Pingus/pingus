//  $Id: PingusSound.hh,v 1.14 2001/04/03 10:45:49 grumbel Exp $
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

#include <string>

class PingusSound
{
protected: 
  static PingusSound* sound;

  virtual void real_play_mod (std::string filename, float volume) =0;
  virtual void real_play_wav(std::string filename, float volume, float panning) =0;
  virtual void real_clean_up() =0;

public:
  static void init (PingusSound* s);

  /** Load a mod and play it immediately.

      @param filename The complete filename, it will be passed to the
      PingusMusicProvider */
  static void play_mod(std::string filename, float volume = 0.5f);

  /** Load a wav and play it immediately.

      @param filename The complete filename, it will be passed to the
             PingusSoundProvider */
  static void play_wav(std::string filename, float volume = 0.5f, float panning = 0.0f);

  /** Shut down the sound and the music and quit SDL */
  static void clean_up();
};

#endif

/* EOF */
