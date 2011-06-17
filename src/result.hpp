//  $Id$
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

#ifndef HEADER_PINGUS_RESULT_HXX
#define HEADER_PINGUS_RESULT_HXX

#include "pingus_level.hpp"
#include "pingus.hpp"


/** Result of a Pingus game */
struct Result
{
  /** Leveldata */
  PingusLevel plf;

  /** Total number of that got saved */
  int saved;

  /** Total number of that got killed */
  int killed;

  /** Total number of Pingus */
  int total;

  /** Time used to finish this level */
  int used_time;

  /** Maximum time available for this level */
  int max_time;

  /** Number of Pingus needed to save */
  int needed;

  /** Set when the global time limit is reached */
  bool aborted;

  bool success() const {
#ifdef PINGUS_MODE_EVIL
    return (killed >= needed);
#else
    return (saved >= needed);
#endif
  }
};


#endif

/* EOF */
