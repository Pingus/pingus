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

#include "vector2.hxx"
#include "scene_node.hxx"

class SpriteNode : public SceneNode
{
private:
  Vector2 pos;
  Rect    repeat;

  Vector2 old_pos;
  Rect    old_repeat;

  SDL_Surface* surface;
  bool has_changed_;

public:
  SpriteNode(const std::string resourcename);
  ~SpriteNode();

  bool has_changed() const;
  Rect get_screen_rect() const;
  Rect get_old_screen_rect() const;
    
  void    set_pos(const Vector2& pos);
  Vector2 get_pos() const;
  
  void set_left_repeat(int left);
  void set_right_repeat(int right);

  void set_top_repeat(int top);
  void set_bottom_repeat(int bottom);

  int get_left_repeat(int left) const;
  int get_right_repeat(int right) const;

  int get_top_repeat(int top) const;
  int get_bottom_repeat(int bottom) const;

  /** Draw the SDL_Surface to the screen */
  void render(SDL_Surface* screen);
};

/* EOF */
