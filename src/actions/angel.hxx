//  $Id: angel.hxx,v 1.6 2002/08/23 15:49:53 torangan Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_ANGEL_HXX
#define HEADER_PINGUS_ACTIONS_ANGEL_HXX

//
// Necessary for Windows environment
//
// Does it work really on Linux?
//
#ifdef WIN32
#include <math.h>
#endif

#include "../sprite.hxx"
#include "../pingu_action.hxx"

class Angel : public PinguAction
{
private:
  double counter;
  double x_pos;
  Sprite sprite;
public:
  Angel ();
  void  init();
  std::string get_name() const { return "Angel"; }
  Pingus::Actions::ActionName get_type() const { return Pingus::Actions::Angel; }
  void  update(float delta);
  void  draw_offset(int, int, float s);

private:
  Angel (const Angel&);
  Angel operator= (const Angel&);
};

#endif

/* EOF */
