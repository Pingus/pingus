//  $Id: PingusWavProvider.cc,v 1.10 2002/06/08 23:11:08 torangan Exp $
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
#include <iostream>
#include "globals.hh"
#include "PingusError.hh"
#include "PingusWavProvider.hh"

std::map<std::string, Mix_Chunk*> PingusWavProvider::wave;

Mix_Chunk*
PingusWavProvider::load(std::string str)
{
#ifdef HAVE_LIBSDL_MIXER
  Mix_Chunk* wav_data;

  wav_data = wave[str];

  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "Loading: " << str << " data: " << wav_data << std::endl;

  if (wav_data)
    {
      return wav_data;
    }
  else
    {
      wav_data = Mix_LoadWAV(str.c_str()); 
      
      if (!wav_data)
	{
	  throw PingusError ("PingusWaveProvider: Couldn't open '" + str + "'");
	}
      else
	{

	  wav_data->volume = 80;

	  if (!wav_data) 
	    {
	      throw PingusError("PingusWavProvider: Couldn't load " + str + ": " + SDL_GetError());
	    } 
	  else
	    {
	      wave[str] = wav_data;
	      return wav_data;
	    }
	}
    }
#endif /* HAVE_LIBSDL_MIXER */
  if (str.size()); // suppress warning about unused argument
  return 0;
}

/* EOF */
