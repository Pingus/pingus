//  $Id: digger.hh,v 1.6 2000/12/14 21:35:55 grumbel Exp $
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

///
class Digger : public PinguAction
{
private:
  ///
  static bool static_surfaces_loaded;
  ///
  static CL_Surface digger_radius;
  ///
  static CL_Surface static_surface;
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
  void   let_move();
}///
;

#endif

/* EOF */
