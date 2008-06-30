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

#ifndef HEADER_PINGUS_GAME_SESSION_RESULT_HPP
#define HEADER_PINGUS_GAME_SESSION_RESULT_HPP

#include "pingus.hpp"


/** With this class you can obtain information about the success or
    failure of a level, in addition you can get details like how many
    penguins where saved or killed, etc. */
class GameSessionResult
{
public:
  GameSessionResult ();

  GameSessionResult (const GameSessionResult&) { }
  GameSessionResult& operator= (const GameSessionResult&) { return *this; }

  /** Gives information if the level was succeeded or not
      @return true if the level was succeeded, otherwise false */
  bool finished ();

  /** @return The percentage of pingus which where saved in this level */
  float percentage_saved ();

  /** @return The percentage of time, which was used to finish this level */
  float percentage_time ();
};


#endif

/* EOF */
