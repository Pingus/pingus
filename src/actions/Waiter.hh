//  $Id: Waiter.hh,v 1.8 2001/08/02 21:51:03 grumbel Exp $
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

#ifndef WAITER_HH
#define WAITER_HH

#include "../PinguAction.hh"

/** A Waiting action for the bridger, it gets activated when the
    bridger is out of bridges. It then waits two seconds (meanwhile doing a
    funny animation) and then he changes back to a normal walker. */
class Waiter : public PinguAction
{
private:
  float countdown;
  Sprite sprite;
public:
  ///
  Waiter();
  /// 
  void init();
  std::string get_name() const { return "Waiter"; }
  PinguEnvironment get_environment() const { return ENV_LAND; }
  void update(float delta);
  void  draw_offset(int x, int y, float s);
};

REGISTER_PINGUACTION(WaiterFactory, Waiter, "waiter");

#endif

/* EOF */
