//  $Id: HurryUp.hh,v 1.2 2000/07/04 22:59:13 grumbel Exp $
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

#ifndef HURRYUP_HH
#define HURRYUP_HH

#include <ClanLib/core.h>
#include "GuiObj.hh"

/** Hurry up if the time goes over... */
class HurryUp : public GuiObj
{
private:
  CL_Font* font;
  bool is_running;
  bool center_reached;

  int  wait_counter;
  float x_pos;

  float speed;

public:
  HurryUp();
  virtual ~HurryUp();

  virtual void draw();
  virtual void let_move();
};

#endif
