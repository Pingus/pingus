//  $Id: sound.hxx,v 1.9 2002/11/02 22:10:52 grumbel Exp $
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

#ifndef HEADER_PINGUS_SOUND_HXX
#define HEADER_PINGUS_SOUND_HXX

#include <string>
#include "sounds.hxx"

class PingusSound
{
protected: 
  static PingusSound* sound;
  
protected:
  PingusSound () { }

  virtual void real_play_sound(const std::string & filename, float volume, float panning) =0;
  virtual void real_play_music(const std::string & filename, float volume) =0;

public:
  static void init (PingusSound* s = 0);
  static void deinit ();

  /** Load a sound file and play it immediately.

      @param filename The complete filename 
      @param volume   volume
      @param panning  panning */
  static void play_sound(const std::string & filename, float volume = 1.0f, float panning = 0.0f);
  
  static void play_music(const std::string & filename, float volume = 1.0f);

  static void play_sound(Sound::Name name, float volume = 1.0f, float panning = 0.0f);
  
private:
  PingusSound (const PingusSound&);
  PingusSound& operator= (const PingusSound&);
};

#endif

/* EOF */
