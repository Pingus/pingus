//  $Id$
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

#ifndef HEADER_PINGUS_DELTA_MANAGER_HXX
#define HEADER_PINGUS_DELTA_MANAGER_HXX

#include "SDL.h"
#include <assert.h>
#include <iostream>
#include "pingus.hpp"


class DeltaManager
{
private:
  unsigned int absolute_time;
  unsigned int last_time;
public:
  DeltaManager ()
    : absolute_time(0),
      last_time (SDL_GetTicks())
  {}

  float getset ()
  {
    float ret = get ();
    set ();
    return ret;
  }

  void set ()
  {
    last_time = SDL_GetTicks();
    absolute_time += last_time;
  }

  float get () const
  {
    unsigned int t = SDL_GetTicks();
    if (t < last_time)
      { // catching time travel
        std::cout << "Time Travel: first: " << last_time << " second: " << t << std::endl;
        return 0.0f;
      }
    else
      {
        return (SDL_GetTicks() - last_time) / 1000.0f;
      }
  }

  /** @return time in miliseconds passed since the start of the DeltaManager */
  unsigned int get_absolute() const
  {
    return absolute_time;
  }

private:
  DeltaManager (const DeltaManager&);
  DeltaManager& operator= (const DeltaManager&);
};


#endif

/* EOF */
