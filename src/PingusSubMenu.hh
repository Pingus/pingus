//  $Id: PingusSubMenu.hh,v 1.1 2001/06/14 11:07:19 grumbel Exp $
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

#ifndef PINGUSSUBMENU_HH
#define PINGUSSUBMENU_HH

#include <ClanLib/display.h>

class PingusMenuManager;

class PingusSubMenu
{
protected:
  PingusMenuManager* manager;

public:
  PingusSubMenu (PingusMenuManager* m);
  virtual ~PingusSubMenu ();

  /// draw the menu
  virtual void draw () =0;

  /// update the menu
  virtual void update (float delta) =0;
  
  /// load all the data which is needed for the menu
  virtual void preload () =0;

  virtual void on_button_press (CL_InputDevice*,const CL_Key &) {}
  virtual void on_button_release (CL_InputDevice*,const CL_Key &) {}
  virtual void on_mouse_move (CL_InputDevice*, int, int) {}

  /// Return a handle to the parent menu manager
  PingusMenuManager* get_manager () { return manager; }
};

#endif

/* EOF */
