//  $Id: KeyboardController.hh,v 1.2 2001/04/21 14:40:22 grumbel Exp $
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

#ifndef KEYBOARDCONTROLLER_HH
#define KEYBOARDCONTROLLER_HH

#include "Controller.hh"

class KeyboardController : public Controller
{
private:
  CL_Vector pos;
  
public:
  KeyboardController (int owner_id = 0);
  virtual ~KeyboardController ();

  int get_x () { return (int)pos.x; }
  int get_y () { return (int)pos.y; }
  
  CL_Vector get_pos () { return pos; }
  
  void keep_alive ();
};

#endif

/* EOF */
