//  $Id: pingus_menu.hxx,v 1.5 2002/08/17 00:28:30 grumbel Exp $
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

#ifndef HEADER_PINGUS_PINGUS_MENU_HXX
#define HEADER_PINGUS_PINGUS_MENU_HXX

#include <list>
#include "game_delta.hxx"
#include "pingus_sub_menu.hxx"
#include "layer_manager.hxx"

class CL_Key;
class CL_InputDevice;
class SurfaceButton;
class GameDelta;

namespace GUI
{
  class GUIManager;
}

namespace Input
{
  class Controller;
}

class PingusMenu : public PingusSubMenu
{
public:
  bool is_init;
private:
  Input::Controller* controller;

  SurfaceButton * temp_button;
  SurfaceButton * current_button;
  CL_Surface background;
  LayerManager layer_manager;
  CL_Surface cursor_sur;
  
  void on_resize(int w, int h);
public:
  PingusMenu(PingusMenuManager* m);
  ~PingusMenu();

  /// Load all images and other stuff for the menu
  void preload ();

  void on_escape_press ();
};

#endif

/* EOF */
