//  $Id: music_provider.hxx,v 1.4 2002/09/27 11:26:43 torangan Exp $
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

#ifndef HEADER_PINGUS_MUSIC_PROVIDER_HXX
#define HEADER_PINGUS_MUSIC_PROVIDER_HXX

#include <map>
#include <string>
#include "audio.hxx"

class PingusMusicProvider
{
  static std::map<std::string, Mix_Music*> music;

public:
  static Mix_Music* load (const std::string& str);
  static Mix_Music* get (const std::string& str);
  
private:
  PingusMusicProvider (const PingusMusicProvider&);
  PingusMusicProvider& operator= (const PingusMusicProvider&);
};

#endif

/* EOF */
