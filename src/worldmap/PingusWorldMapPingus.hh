//  $Id: PingusWorldMapPingus.hh,v 1.1 2000/09/21 15:23:57 grumbel Exp $
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

#ifndef PINGUSWORLDMAPPINGUS_HH
#define PINGUSWORLDMAPPINGUS_HH

#include <stack>
#include "../Position.hh"
#include "../AnimCounter.hh"
#include "PingusWorldMapGraph.hh"

/** This is the representation of the horde of Pingus which will walk
    on the worldmap */
class PingusWorldMapPingus
{
private:
  CL_Surface* sur;
  Position pos;
  AnimCounter counter;
  stack<Position> targets;

public:
  PingusWorldMapPingus ();
  ~PingusWorldMapPingus ();

  void draw ();
  void let_move ();
  void walk_to (PingusWorldMapNode node);
  void set_position (PingusWorldMapNode node);
};

#endif

/* EOF */
