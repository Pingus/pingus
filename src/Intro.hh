//  $Id: Intro.hh,v 1.14 2002/01/15 10:48:49 grumbel Exp $
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

#ifndef INTRO_HH
#define INTRO_HH

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <strstream>

#include "PingusSubMenu.hh"

///
class Intro : public PingusSubMenu
{
private:
  CL_Surface logo;
  CL_Vector  pos;
  CL_Vector  velocity;
  unsigned int start_time;
  enum { SCROLL_UP, SLOWDOWN, WAITING, FINISHED } stage;
  CL_Font* font;

  unsigned int blink_time;
public:
  Intro(PingusMenuManager* m);
  ~Intro();

  void draw();
  void update (float delta);
  void preload ();
  void on_click ();
  void on_button_press (CL_InputDevice* device,const CL_Key& key);
};

#endif

/* EOF */
