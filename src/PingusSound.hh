//  $Id: PingusSound.hh,v 1.1 2000/02/04 23:45:18 mbn Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef SOUNDSPECS_HH
#define SOUNDSPECS_HH

#include "SpecsReader.hh"

// This class manage all sounds in pinugs, the sound can be played by
// giving play() an integer of the enum Sound.
// BUG: I don't like the design of this class, so if somebody is
// willing to redesign it, he is free to go.
class PingusSound : public SpecsReader
{
private:
  static string directory;
  static PingusSound* t;

  PingusSound();
public:
  enum Sounds {
    UNDEF, OHNO, DOOR, EXPLODE, YIPEE, LETSGO, OING, SPLAT  
  };

  static void init(string filename);
  static void play(Sounds s);
};

#endif

/* EOF */
