//  $Id: Panel.hh,v 1.2 2000/02/09 21:43:43 grumbel Exp $
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

#ifndef PANEL_HH
#define PANEL_HH

#include <vector>
#include <ClanLib/core.h>

#include "EditorEvent.hh"

class EditorEvent;

class PanelIcon
{
protected:
  CL_Surface* sur;
  CL_Surface* button;
  CL_Surface* button_pressed;
public:
  static EditorEvent* event;

  PanelIcon();
  virtual ~PanelIcon();
  void start();
  void put_screen(int x, int y);
  virtual void on_click();
};

class Panel
{
private:
  bool is_init;
  vector<PanelIcon*> buttons;
  vector<PanelIcon*>::iterator pressed_button;
  EditorEvent* event;
  CL_Surface* background;
  CL_Surface* logo;

public:
  Panel();
  ~Panel();

  void draw();
  void init();
  void on_click();
  void on_release();
  bool mouse_over(int x, int y);
  void set_event(EditorEvent* e);
};

#endif

/* EOF */
