//  $Id: menu_background.hxx,v 1.3 2002/07/30 14:57:25 grumbel Exp $
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

#ifndef HEADER_PINGUS_MENU_BACKGROUND_HXX
#define HEADER_PINGUS_MENU_BACKGROUND_HXX

#include "layer_manager.hxx"
#include "pingus_sub_menu.hxx"

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
  void update (const GameDelta& delta);
  void preload ();
};

#endif

/* EOF */
