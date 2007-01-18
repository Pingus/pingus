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

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "scene_graph.hpp"
#include "sprite_node.hpp"

extern SDL_Surface* global_screen;

SpriteNode::SpriteNode(const std::string resourcename)
{
  surface = IMG_Load(resourcename.c_str());
}

SpriteNode::~SpriteNode() 
{
  SDL_FreeSurface(surface);
}

void
SpriteNode::set_pos(const Point& pos_)
{
  if (pos != pos_) has_changed_ = true;
  pos = pos_;
}

Point
SpriteNode::get_pos() const
{
  return pos;
}

void
SpriteNode::render(SceneGraph* graph)
{
  SDL_Rect dest;

  dest.x = pos.x;
  dest.y = pos.y;

  SDL_BlitSurface(surface, NULL, global_screen, &dest);

  has_changed_ = false;
  old_repeat = repeat;
  old_pos    = pos;
}

Rect
SpriteNode::get_old_screen_rect() const
{
  return Rect(old_pos.x - (old_repeat.left * surface->w),
              old_pos.y - (old_repeat.top  * surface->h), 
              surface->w * (old_repeat.left + old_repeat.right + 1),
              surface->h * (old_repeat.top  + old_repeat.bottom + 1));
}

Rect
SpriteNode::get_screen_rect() const
{
  return Rect(pos.x - (repeat.left * surface->w),
              pos.y - (repeat.top  * surface->h), 
              surface->w * (repeat.left + repeat.right + 1),
              surface->h * (repeat.top  + repeat.bottom + 1));
}

bool
SpriteNode::has_changed() const
{
  return has_changed_;
}

void
SpriteNode::mark(SceneGraph* graph)
{
  if (has_changed_)
    {
      graph->mark_screen_region(this, get_old_screen_rect());
      graph->mark_screen_region(this, get_screen_rect());
    }
}

void
SpriteNode::set_left_repeat(int left)
{
  if (left != repeat.left) has_changed_ = true;
  repeat.left = left;
}

void
SpriteNode::set_right_repeat(int right)
{
  if (right != repeat.right) has_changed_ = true;
  repeat.right = right;
}

void
SpriteNode::set_top_repeat(int top)
{
  if (top != repeat.top) has_changed_ = true;
  repeat.top = top;
}

void
SpriteNode::set_bottom_repeat(int bottom)
{
  if (bottom != repeat.bottom) has_changed_ = true;
  repeat.bottom = bottom;
}

int
SpriteNode::get_left_repeat(int left) const
{
  return repeat.left;
}

int
SpriteNode::get_right_repeat(int right) const
{
  return repeat.right;
}

int
SpriteNode::get_top_repeat(int top) const
{
  return repeat.top;
}

int
SpriteNode::get_bottom_repeat(int bottom) const
{
  return repeat.bottom;
}

/* EOF */
