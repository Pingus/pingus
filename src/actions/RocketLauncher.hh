//  $Id: RocketLauncher.hh,v 1.3 2001/08/16 17:46:51 grumbel Exp $
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

#ifndef ROCKETLAUNCHER_HH
#define ROCKETLAUNCHER_HH

#include "../PingusResource.hh"
#include "../Sprite.hh"
#include "../PinguAction.hh"

class RocketLauncher
  : public PinguAction
{
private:
  Sprite sprite;
  bool launched;
public:
  RocketLauncher () {}
  ~RocketLauncher () {}

  void  init();
  std::string get_name() const { return "RocketLauncher"; }
  PinguEnvironment get_environment() const { return ENV_LAND; }
  void  update(float delta);
  void  draw_offset(int, int, float s);
};

#endif

/* EOF */
