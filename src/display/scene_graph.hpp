//  $Id$
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_SCENE_GRAPH_HPP
#define HEADER_SCENE_GRAPH_HPP

#define EMPTY_TILE 0x00000000
#define FULL_TILE  0x00002020

#define MICROTILE(x0, y0, x1, y1) (((x0) << 24) | ((y0) << 16) | ((x1) << 8) | (y1))

#include <vector>
#include "../field.hpp"

class Rect;
class SceneNode;

/** */
class SceneGraph
{
private:
  /** true if the region needs a refresh, false otherwise */
  Field<uint32_t> screen;

  typedef std::vector<SceneNode*> Nodes;
  Nodes nodes;
  
public:
  SceneGraph();
  ~SceneGraph();

  void add(SceneNode* );
  
  /** Draw the SceneGraph to the screen */
  void render();
  void mark_screen_region(SceneNode* node, const Rect& rect);
private:
  SceneGraph (const SceneGraph&);
  SceneGraph& operator= (const SceneGraph&);
};

#endif

/* EOF */
