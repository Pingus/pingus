//  $Id: PingusMenu.hh,v 1.25 2002/06/10 15:01:23 torangan Exp $
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

#ifndef PINGUSMENU_HH
#define PINGUSMENU_HH

#include <list>
#include "PingusSubMenu.hh"
#include "LayerManager.hh"
#include "SurfaceButton.hh"

class CL_Key;
class CL_InputDevice;
class SurfaceButton;

///
class PingusMenu : public PingusSubMenu
{
public:
  ///
  bool is_init;
private:
  ///
  std::list<boost::shared_ptr<SurfaceButton> > buttons;
  ///
  SurfaceButton * temp_button;
  ///
  SurfaceButton * current_button;
  ///
  CL_Surface background;
  ///
  LayerManager layer_manager;
  ///
  CL_Surface cursor_sur;
  
  void on_resize(int w, int h);
public:
  PingusMenu(PingusMenuManager* m);
  ~PingusMenu();

  /// Load all images and other stuff for the menu
  void preload ();

  void update (float /*delta*/) {}
  void draw(void);

  void select(void);
  bool event_enabled;

  void on_button_press(CL_InputDevice *device, const CL_Key &key);
  void on_button_release(CL_InputDevice *device, const CL_Key &key);
  void on_mouse_move(CL_InputDevice *,int mouse_x, int mouse_y);
};

#endif

/* EOF */
