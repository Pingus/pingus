//  $Id: exit_menu.hxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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

#ifndef EXITMENU_HH
#define EXITMENU_HH

#include "pingus_sub_menu.hxx"

class PingusMenuManager;

class ExitMenu : public PingusSubMenu
{
private:
  CL_Surface sur; 
public:
  ExitMenu (PingusMenuManager* manager);
  ~ExitMenu ();

  void draw ();
  void update (float delta);
  void preload ();

  void on_button_press (CL_InputDevice*,const CL_Key &) {}
  void on_button_release (CL_InputDevice*,const CL_Key &);
};

#endif

/* EOF */
