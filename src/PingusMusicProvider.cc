//  $Id: PingusMusicProvider.cc,v 1.5 2000/10/10 13:22:39 grumbel Exp $
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
#include "PingusError.hh"
#include "PingusMusicProvider.hh"

std::list<PingusMusicProvider::music_pair> PingusMusicProvider::music;

Mix_Music*
PingusMusicProvider::load(std::string str)
{
#ifdef HAVE_LIBSDL_MIXER
  Mix_Music* music_data;

  music_data = get(str);

  if (music_data)
    {
      return music_data;
    }
  else
    {
      music_pair song;
      song.filename = str;
      song.data = Mix_LoadMUS(str.c_str()); 

      if (!song.data) 
	{
	  throw PingusError("PingusMusicProvider: Couldn't load " + str + ": " + SDL_GetError());
	} 
      else
	{
	  music.push_back(song);
	  return song.data;
	}
    }
#endif /* HAVE_LIBSDL_MIXER */
  return 0;
}

Mix_Music*
PingusMusicProvider::get(std::string str)
{
  for(std::list<music_pair>::iterator i = music.begin(); i != music.end(); i++)
    {
      if (i->filename == str)
	return i->data;
    }
  return 0;
}

/* EOF */
