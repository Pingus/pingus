//  $Id: PingusSound.cc,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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

#include <ClanLib/core.h>


#include "globals.hh"
#include "algo.hh"
#include "PingusSound.hh"

PingusSound* PingusSound::t;
string PingusSound::directory;

PingusSound::PingusSound(void)
{
}

void
PingusSound::init(string filename)
{
  t = new PingusSound();

  directory = get_directory(filename);

  if (verbose)
    cout << "PingusSound: " << directory << endl;

  t->register_token(OHNO, "OhNo");
  t->register_token(DOOR, "Door");
  t->register_token(EXPLODE, "Explode");
  t->register_token(YIPEE, "Yipee");
  t->register_token(LETSGO, "LetsGo");
  t->register_token(OING, "Oing");
  t->register_token(SPLAT, "Splat");
  
  cout << "Reading File" << endl;

  t->open(filename.c_str());

  if (verbose > 2) {
    cout << "Testing Tokens" << endl;
    cout << "Test OhNo: " << (*t)[OHNO] << endl;
    cout << "Test Door: " << (*t)[DOOR] << endl;
    cout << "Test Explode: " << (*t)[EXPLODE] << endl;
    cout << "Test Yipee: " << (*t)[YIPEE] << endl;
    cout << "Test LetsGo: " << (*t)[LETSGO] << endl;
  }
  t->close();
}

void
PingusSound::play(Sounds s)
{
  if (sound_enabled) {
    if (verbose > 2)
      cout << "PingusSound: playing \"" << directory + (*t)[s] << "\""
	   << " " << static_cast<int>(s) << endl;

    CL_SoundBuffer *soundbuffer = CL_Sample::create((directory + (*t)[s]).c_str(), NULL);
    soundbuffer->play();
  }
}

/* EOF */
