//  $Id: PingusWavProvider.cc,v 1.3 2000/06/23 18:39:56 grumbel Exp $
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
#include "audio.hh"
#include "PingusError.hh"
#include "PingusWavProvider.hh"

std::list<PingusWavProvider::wav_pair> PingusWavProvider::wave;

Mix_Chunk*
PingusWavProvider::load(std::string str)
{
#ifdef HAVE_LIBSDL_MIXER
  Mix_Chunk* wav_data;

  wav_data = get(str);

  if (wav_data)
    {
      return wav_data;
    }
  else
    {
      wav_pair chunk;
      chunk.filename = str;
      chunk.data = Mix_LoadWAV(str.c_str()); 

      if (!chunk.data) 
	{
	  throw PingusError("PingusWavProvider: Couldn't load " + str + ": " + SDL_GetError());
	} 
      else
	{
	  wave.push_back(chunk);
	  return chunk.data;
	}
    }
#endif /* HAVE_LIBSDL_MIXER */
  return 0;
}

Mix_Chunk*
PingusWavProvider::get(std::string str)
{
  for(std::list<wav_pair>::iterator i = wave.begin(); 
      i != wave.end(); 
      ++i)
    {
      if (i->filename == str)
	return i->data;
    }
  return 0;
}

/* EOF */
