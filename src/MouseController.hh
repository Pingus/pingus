//  $Id: MouseController.hh,v 1.3 2001/04/13 09:31:37 grumbel Exp $
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

#ifndef MOUSECONTROLLER_HH
#define MOUSECONTROLLER_HH

#include "Controller.hh"

class MouseController : public Controller
{
private:
  
public:
  MouseController (int arg_owner_id = 0);
  virtual ~MouseController ();

  int get_x ();
  int get_y ();
  
  CL_Vector get_pos ();

  /** Limit the range to the given area, might be used in multiplayer
      modes to limit the cursor to the players view */
  void set_range (int x1, int y1, int x2, int y2);
};

#endif

/* EOF */
