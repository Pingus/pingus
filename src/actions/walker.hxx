//  $Id: walker.hxx,v 1.1 2002/06/12 19:01:43 grumbel Exp $
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

#ifndef WALKER_HH
#define WALKER_HH

#include "../pingu_action.hxx"
#include "../sprite.hxx"

class Walker : public PinguAction
{
private:
  Sprite walker; 

  static const int max_steps; // max nr. of pixels that pingu can walk up/down

public:
  void  init(void);

  void  update(float delta);
  void  draw_offset(int x, int y, float s);

  PinguEnvironment get_environment() const { return ENV_ALWAYS; }
  std::string get_name() const { return "Walker"; }
};

#endif

/* EOF */
