//  $Id: digger.hh,v 1.10 2001/04/23 08:00:08 grumbel Exp $
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

#ifndef DIGGER_HH
#define DIGGER_HH

#include "../PinguAction.hh"

/** FIXME: Not delta capable! */
class Digger : public PinguAction
{
private:
  ///
  CL_Surface digger_radius;
  ///
  Sprite sprite;
  ///
  Counter digger_c;
public:
  ///
  Digger();

  ///
  PinguAction* allocate(void);

  ///
  bool   have_something_to_dig();
  ///
  void   dig();
  ///
  void   init(void);
  ///
  void   update(float delta);
  void  draw_offset(int x, int y, float s);
};

REGISTER_PINGUACTION(DiggerFactory, Digger, "digger");

#endif

/* EOF */
