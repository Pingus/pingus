//  $Id: PingusSoundDummy.cc,v 1.3 2001/05/13 18:45:08 grumbel Exp $
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

#include <iostream>
#include "globals.hh"
#include "PingusSoundDummy.hh"

void
PingusSoundDummy::real_play_mod (std::string filename, float volume)
{
  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "PingusSoundDummy::real_play_mod:" << filename << ":" << volume << std::endl;
}

void 
PingusSoundDummy::real_play_wav(std::string filename, float volume, float panning)
{
  if (pingus_debug_flags & PINGUS_DEBUG_SOUND)
    std::cout << "PingusSoundDummy::real_play_wav: " << filename << std::endl;
}

void 
PingusSoundDummy::real_clean_up()
{
}


/* EOF */
