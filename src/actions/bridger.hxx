//  $Id: bridger.hxx,v 1.1 2002/06/12 19:01:42 grumbel Exp $
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

#ifndef BRIDGER_HH
#define BRIDGER_HH

#include "../game_counter.hxx"
#include "../pingu_action.hxx"
#include "../sprite.hxx"

/** 
 */
class Bridger : public PinguAction
{
private:
  Sprite walk_sprite;
  Sprite build_sprite;

  GameCounter counter;

  static bool static_surfaces_loaded;
  static CL_Surface static_surface;
  static CL_Surface brick_l;
  static CL_Surface brick_r;

  int bricks;
  int step;
  int do_steps;
  bool block_build;

  enum Mode { B_WALKING, B_BUILDING } mode;
public:
  Bridger();

  void   init();
  std::string get_name () const;
  PinguEnvironment get_environment() const { return ENV_LAND; }
  void   update(float delta);
  void   update_build (float delta);
  void   update_walk (float delta);
  void   draw_offset(int, int, float s);
  bool   way_is_free();
  void   place_a_brick();
  void   walk_one_step_up();
};

#endif

/* EOF */
