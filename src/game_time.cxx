//  $Id: game_time.cxx,v 1.4 2002/06/28 09:51:46 grumbel Exp $
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

#include "globals.hxx"
#include "game_time.hxx"

GameTime::GameTime (int arg_tick_time)
  : count (0), tick_time (arg_tick_time)
{
}

int
GameTime::get_time ()
{
  return count * game_speed;
}

int 
GameTime::get_ticks(void)
{
  return count;
}

int
GameTime::get_tick_time ()
{
  return game_speed;
}

void
GameTime::update(void)
{
  count += 1;
}

void
GameTime::reset(void)
{
  count = 0;
}

/* EOF */
