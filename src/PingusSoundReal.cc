//  $Id: PingusSoundReal.cc,v 1.12 2001/11/18 00:03:28 grumbel Exp $
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

#include <string>
#include <iostream>

#ifdef HAVE_LIBCLANVORBIS
#  include <ClanLib/vorbis.h>
#endif

#ifdef HAVE_LIBCLANMIKMOD
#  include <ClanLib/mikmod.h>
#endif

#include "globals.hh"
#include "PingusError.hh"
#include "PingusSoundReal.hh"

PingusSoundReal::PingusSoundReal ()
{
  init();
}

PingusSoundReal::~PingusSoundReal()
{
  if (music) {
    if (music -> is_playing())
      music -> stop();
    delete music;
  }
}


void
PingusSoundReal::init()
{

  is_init = true;

  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    cout << "Initializing ClanLib-Sound" << endl;
    
  CL_SetupSound::init();
  
  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    cout << "Initializing ClanLib-MikMod" << endl;
  
#ifdef HAVE_LIBCLANMIKMOD
  CL_SetupMikMod::init();
#endif
}


void
PingusSoundReal::real_play(std::string filename, float volume, float panning)
{

  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "PingusSoundReal: Playing file: " << filename << std::endl;
  
  if (music) {
    if (music -> is_playing())
      music -> stop();
    delete music;
    music = NULL;
  }

  sample = 0;
  if (filename.substr(filename.size()-4, 4) == ".ogg") {
#ifdef HAVE_LIBCLANVORBIS
    sample = new CL_SoundBuffer (new CL_VorbisSoundProvider(filename.c_str()), true);
#endif
  } else if (filename.substr(filename.size()-4, 4) == ".wav") {
    std::cout << "Überspringe .WAV" << std::endl;
    //sample = new CL_SoundBuffer (new CL_Sample(filename.c_str(), NULL), true);
        
  } else {  // MikMod wird den Rest schon schaffen ;-)
#ifdef HAVE_LIBCLANMIKMOD
    sample = new CL_SoundBuffer (new CL_Streamed_MikModSample(filename.c_str()), true);
#endif
  }
  
  if (sample)
    {
      music = new CL_SoundBuffer_Session(sample -> prepare());
  
      std::cout << "vor set_volume\n";
      music -> set_volume(volume);
      std::cout << "nach set_volume\n";
      music -> set_looping(true);
      music -> play();
    }
}


/* EOF */

