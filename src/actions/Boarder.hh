//  $Id: Boarder.hh,v 1.4 2001/08/16 17:46:51 grumbel Exp $
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

#ifndef BOARDER_HH
#define BOARDER_HH

#include "../PinguAction.hh"

/** The Boarder action causes a pingu to use a skateboard to move
    forward. */
class Boarder : public PinguAction
{
private:
  double counter;
  double x_pos;
  double speed;
  Sprite sprite;
public:
  Boarder ();
  void  init();
  std::string get_name() const { return "Boarder"; }
  PinguEnvironment get_environment() const { return ENV_LAND; }
  void  update(float delta);
  void  draw_offset(int, int, float s);
private:
  bool on_ground ();
};

#endif

/* EOF */
