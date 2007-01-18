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

#ifndef HEADER_SCENE_NODE_HPP
#define HEADER_SCENE_NODE_HPP

#include "../math/rect.hpp"

class SceneGraph;

/** */
class SceneNode
{
private:
public:
  SceneNode();
  virtual ~SceneNode();

  /** Return the area that the SceneNode covers on the screen */
  virtual Rect get_screen_rect() const =0;

  /** Mark regions for update */
  virtual void mark(SceneGraph* graph) =0;

  virtual void render(SceneGraph* graph) =0;
  
private:
  SceneNode (const SceneNode&);
  SceneNode& operator= (const SceneNode&);
};

#endif

/* EOF */
