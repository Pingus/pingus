//  $Id: PingusWorldMapPingus.hh,v 1.8 2001/04/06 15:04:46 grumbel Exp $
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

#include <queue>
#include "PingusWorldMapGraph.hh"
#include "../Sprite.hh"

/** This is the representation of the horde of Pingus which will walk
    on the worldmap */
class PingusWorldMapPingus
{
private:
  Sprite sprite;
  CL_Vector pos;
  queue<PingusWorldMapNode*> targets;
  bool is_left;
  PingusWorldMapNode* current_node;

public:
  PingusWorldMapPingus ();
  ~PingusWorldMapPingus ();

  void draw (const CL_Vector& offset);
  void update (float delta);
  void walk_to (PingusWorldMapNode* node);
  bool is_walking ();
  void set_position (PingusWorldMapNode* node);
  CL_Vector get_pos () { return pos; }
  PingusWorldMapNode* get_node ();
};

#endif

/* EOF */
