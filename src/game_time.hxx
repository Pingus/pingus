//  $Id: game_time.hxx,v 1.6 2002/08/23 15:49:48 torangan Exp $
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

#ifndef HEADER_PINGUS_GAME_TIME_HXX
#define HEADER_PINGUS_GAME_TIME_HXX

#include "pingus.hxx"

/** The GameTime represents the time which passes in the Pingus World.
    Its behaviour is analogue to CL_System::get_time (), but with the
    difference that it only increases if the game runs, if the game is
    in pause mode, the time will not continue. 
    
    FIXME: This should not be a static singletone class */
class GameTime
{
private:
  /** Tick counter */
  int count;
  
  /** How long does a tick take in msec */
  int tick_time;

public:
  GameTime (int arg_tick_time);
  
  /** Number of ticks since the time starts, a tick is one basically
      update call to the world */
  int  get_ticks(void);

  /** Return the passed time in miliseconds (1000msec = 1sec) */
  int get_time ();

  /** Return in realtime (milisecondons ) how long a tick normally takes */
  int get_tick_time ();

  /** Increase the tick count */
  void update(void);
  
  /** Start from zero */
  void reset(void);
  
private:
  GameTime (const GameTime&);
  GameTime operator= (const GameTime&);
};

#endif

/* EOF */
