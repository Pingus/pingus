//  $Id: Walker.hh,v 1.4 2002/01/21 11:13:55 grumbel Exp $
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

#include "../PinguAction.hh"

#ifdef WIN32
  static const int max_steps=4; // max nr. of pixels that pingu can walk up/down
#endif

class Walker : public PinguAction
{
private:
  Sprite walker; 

// (Alberto)
// This stuff does not work under VS, but I don't know why.
// By now, the unique solution is by using a #define
#ifndef WIN32
  static const int max_steps=4; // max nr. of pixels that pingu can walk up/down
#endif

public:
  void  init(void);

  void  update(float delta);
  void  draw_offset(int x, int y, float s);

  PinguEnvironment get_environment() const { return ENV_ALWAYS; }
  std::string get_name() const { return "Walker"; }
};

#endif

/* EOF */
