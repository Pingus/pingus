//  $Id: Splashed.hh,v 1.12 2001/08/10 10:56:14 grumbel Exp $
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

#ifndef SPLASHED_HH
#define SPLASHED_HH

#include "../Sprite.hh"
#include "../PinguAction.hh"

///
class Splashed : public PinguAction
{
private:
  bool particle_thrown;
  bool sound_played;
  Sprite sprite;
public:
  ///
  Splashed();
  
  void   init(void);
  std::string get_name() const { return "Splashed"; }
  PinguEnvironment get_environment() const { return ENV_LAND; }
  void update(float delta);
  void draw_offset(int x, int y, float s);
  bool catchable () { return false; }
};

REGISTER_PINGUACTION(SplashedFactory, Splashed, "splashed");

#endif

/* EOF */
