//  $Id: PinguEnums.hh,v 1.5 2000/06/27 16:05:16 grumbel Exp $
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

#ifndef PINGUENUMS_HH
#define PINGUENUMS_HH

/** Haven't yet exerimented much which pingu status, but maybe it is a
    good idea. Maybe that should be changed to a bitmask. */
enum PinguStatus { alive, exited, dead, sleeping, ko, not_catchable };

/// The enviroment in which a PinguAction get be activated.
enum PinguEnvironment {
  land   = 1<<0,
  water  = 1<<1,
  fire   = 1<<2,
  sky    = 1<<3,
  always = 1<<4
};

///
enum PingusGameMode { 
  INTERACTIVE_MODE,
  DEMO_MODE 
};

#endif

/* EOF */
