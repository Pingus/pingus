//  $Id: MenuBackground.hh,v 1.1 2001/06/14 11:08:40 grumbel Exp $
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

#ifndef MENUBACKGROUND_HH
#define MENUBACKGROUND_HH

#include "LayerManager.hh"
#include "PingusSubMenu.hh"

class PingusMenuManager;

class MenuBackground 
  : public PingusSubMenu
{
private:
  LayerManager layer_manager;
public:
  MenuBackground (PingusMenuManager* manager);
  ~MenuBackground ();

  void draw ();
  void update (float delta);
  void preload ();
};

#endif

/* EOF */
