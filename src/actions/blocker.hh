//  $Id: blocker.hh,v 1.11 2001/08/02 21:51:03 grumbel Exp $
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

#ifndef BLOCKER_HH
#define BLOCKER_HH

#include "../PinguAction.hh"

///
class Blocker : public PinguAction
{
private:
  Sprite sprite;
public:
  ///
  Blocker();
  ///
  int   y_offset();
  ///
  void  init();
  std::string get_name () const { return "Blocker"; }
  PinguEnvironment get_environment() const { return PinguEnvironment(ENV_LAND); }
 
  void  update(float delta);
  void   draw_offset(int, int, float s);
  bool  standing_on_ground();
  ///
  bool  need_catch();
  ///
  void  catch_pingu(Pingu* pingu);
};

REGISTER_PINGUACTION(BlockerFactory, Blocker, "blocker");

#endif

/* EOF */
