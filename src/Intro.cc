//  $Id: Intro.cc,v 1.17 2001/06/14 11:07:18 grumbel Exp $
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

#include "PingusMenuManager.hh"
#include "PingusResource.hh"
#include "Display.hh"
#include "globals.hh"
#include "Loading.hh"
#include "algo.hh"
#include "Story.hh"
#include "Intro.hh"

Intro::Intro(PingusMenuManager* m)
  : PingusSubMenu (m)
{
}

Intro::~Intro()
{
}

void 
Intro::draw()
{
  switch (stage)
    {
    case SCROLL_UP:
      CL_Display::clear_display (0.0, 0.0, 0.0);
      break;
    case SLOWDOWN:
      CL_Display::clear_display (0.0, 0.0, 0.0, 0.5);
      break;
    case FINISHED:
      font->print_center (CL_Display::get_width ()/2, 
			  CL_Display::get_height ()/2 + CL_Display::get_height ()/4,
			  "..:: Press Start ::..");
      break;
    }

  logo.put_screen (pos.x - (logo.get_width ()/2),
		   pos.y - (logo.get_height ()/2));
}

void 
Intro::update (float delta)
{
  if (CL_Keyboard::get_keycode (CL_KEY_SPACE))
    manager->set_menu (&manager->mainmenu);
  
  switch (stage)
    {
    case SCROLL_UP:
      pos += velocity * delta;
      if (pos.y < CL_Display::get_height ()/2 + 75) {
	//pos.y = CL_Display::get_height ()/2;
	stage = SLOWDOWN;
      }
      break;
    case SLOWDOWN:
      velocity *= 1 - (0.5 * delta);
      pos += velocity * delta;
      if (pos.y < CL_Display::get_height ()/2) {
	pos.y = CL_Display::get_height ()/2;
	stage = FINISHED;
      }
      break;      
    case FINISHED:
      break;
    }
}

void 
Intro::on_click ()
{
  std::cout << "Start menu" << std::endl;
}

void 
Intro::preload ()
{
  font = PingusResource::load_font ("Fonts/pingus","fonts");
  logo = PingusResource::load_surface ("misc/logo", "core");

  velocity = CL_Vector (0, -75);
  pos = CL_Vector (CL_Display::get_width ()/2,
		   CL_Display::get_height () + logo.get_height ());
  
  stage = SCROLL_UP;
}

/* EOF */
