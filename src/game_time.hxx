//  $Id: game_time.hxx,v 1.3 2002/06/28 08:32:20 grumbel Exp $
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

/** The GameTime represents the time which passes in the Pingus World.
    Its behaviour is analogue to CL_System::get_time (), but with the
    difference that it only increases if the game runs, if the game is
    in pause mode, the time will not continue. 
    
    FIXME: This should not be a static singletone class */
class GameTime
{
private:
  /** Tick counter */
  static int count;

public:
  /** Number of ticks since the time starts, a tick is one basically
      update call to the world */
  static int  get_ticks(void);

  /** Return the passed time in miliseconds (1000msec = 1sec) */
  static int get_time ();

  /** Return in realtime (milisecondons ) how long a tick normally takes */
  static int get_tick_time ();

  /** Increase the tick count */
  static void increase(void);
  
  /** Start from zero */
  static void reset(void);
};

#endif

/* EOF */
