//  $Id: PingusLevelResult.cc,v 1.9 2000/04/21 09:47:36 grumbel Exp $
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

#include <cstdio>
#include <ClanLib/core.h>

#include <config.h>

#include "globals.hh"
#include "PingusResource.hh"
#include "PingusLevelResult.hh"
#include "PingusSound.hh"

PingusLevelResult::PingusLevelResult(World* w)
{
  font = CL_Font::load("Fonts/pingus_small", PingusResource::get("fonts.dat"));
  title = CL_Font::load("Fonts/pingus", PingusResource::get("fonts.dat"));
  background = CL_Surface::load("Textures/stone", PingusResource::get("textures.dat"));
  //result = r;
  world = w;
}

void
PingusLevelResult::draw(void)
{
  char  str[1024];

#ifdef HAVE_LIBSDL_MIXER
  PingusSound::play("../data/music/pingus-2.it");
#endif

  for(int y = 0; y < CL_Display::get_height(); y += background->get_height())
    for(int x = 0; x < CL_Display::get_width(); x += background->get_width())
      background->put_screen(x, y);

  CL_Display::fill_rect(0, 0, CL_Display::get_width(), CL_Display::get_height(), 0.0, 0.0, 0.0, 0.5);
  
  title->print_center(CL_Display::get_width() / 2, 50, "Results:");
  font->print_center(CL_Display::get_width() / 2, 100,
		     get_message(100 * world->get_saved_pingus() / world->get_allowed_pingus()).c_str());

  sprintf(str,  "Pingus saved:   %3d/%3d", 
	  world->get_saved_pingus(),
	  world->get_allowed_pingus());
  font->print_center(CL_Display::get_width() / 2, 140, str);

  sprintf(str,  "Pingus killed:  %3d/%3d", 
	  world->get_allowed_pingus() - world->get_saved_pingus(),
	  world->get_allowed_pingus());
  font->print_center(CL_Display::get_width() / 2, 160, str);

  /*
  sprintf(str,  "Required Time: %2d:%2d:%2d", 
	  result.time / (60 * game_speed),
	  result.time / game_speed % 60, 
	  (result.time * 100) / game_speed % 100);
  */

  //font->print_center(CL_Display::get_width() / 2, 180, str);

  font->print_center(CL_Display::get_width()/2, CL_Display::get_height() - 80,
		     "Press mouse button to continue...");  
  CL_Display::flip_display();

  while(!CL_Mouse::left_pressed())
    CL_System::keep_alive();
  
  while(CL_Mouse::left_pressed())
    CL_System::keep_alive();
}

string
PingusLevelResult::get_message(int saved)
{
  if (saved == 100) {
    // (100%--) survival: Aww, and we wanted to say: 'No Pingus were harmed in the completion of this level'
    return "As many Pingus escaped as entered the level. Thats going to be hard to beat.... unless this game gets pornographic.";
  } else if (saved > 90) {
    return "Very impressive indeed.";
  } else if (saved > 80) {
    return "Good work. Still room for improvement though.";
  } else if (saved > 70) {
    return "Not too shabby, not too shabby at all.";
  } else if (saved > 60) {
    return "That was OK, but Pingu life insurance premiums have just gotten more expensive.";
  } else if (saved > 60) {
    return "Maybe this level calls for a different stratagy.";
  } else if (saved > 50) {
    return "Exactly half. Are you saving only the female ones, you kinky sod!?";
  } else if (saved > 40) {
    return "If I were a Pingu, I never would have left that entrance.";
  } else if (saved > 30) {
    return "Maybe you would feel more at home playing Quake.";
  } else if (saved > 20) {
    return "Maybe this level calls for a different stratagy. Like attempting to save them, for example.";
  } else if (saved > 10) {
    return "Considered a career as a Pingu exterminator?";
  } else if (saved > 0) {
    return "You missed one! Whats your excuse!?";
  } else if (saved == 0) {
    return "Please reassure me that you hit the Armageddon button.";
  } else {
    return "You've got a negative save/total value, something is buggy.";
  }
}

/* EOF */
