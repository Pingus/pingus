//  $Id: level_result.cxx,v 1.18 2003/10/18 23:17:27 grumbel Exp $
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

#include <stdio.h>
#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/display.h>
#include <ClanLib/Display/font.h>
#include <ClanLib/Display/mouse.h>

#include "gui/display.hxx"
#include "resource.hxx"
#include "level_result.hxx"
#include "sound/sound.hxx"
#include "world.hxx"
#include "pingu_holder.hxx"
#include "fonts.hxx"
#include "gettext.h"
#include "input\controller.hxx"

namespace Pingus {

PingusLevelResult::PingusLevelResult(World* w, Controller* c)
{
  font  = Fonts::pingus_small;
  title = Fonts::pingus_large;
  background = Resource::load_surface("Textures/stone", "textures");
  //result = r;
  world = w;
  controller = c;
}

void
PingusLevelResult::draw(void)
{
  char str[128];

  PingusSound::play_music("pingus-2.it");

  for(int y = 0; y < CL_Display::get_height(); y += background.get_height())
    for(int x = 0; x < CL_Display::get_width(); x += background.get_width())
      background.draw(x, y);

  CL_Display::fill_rect(0, 0, CL_Display::get_width(), CL_Display::get_height(), 0.0, 0.0, 0.0, 0.5);

  title->print_center(CL_Display::get_width() / 2, 50, _("Results:"));

  /* Ending messages are censored for the momement
    font->print_center(CL_Display::get_width() / 2, 100,
		     get_message(100 * world->get_saved_pingus() / world->get_allowed_pingus()).c_str());
  */
  snprintf(str, 128, _("Pingus saved:   %3d/%3d"),
           world->get_pingus()->get_number_of_exited(),
	   world->get_pingus()->get_number_of_allowed());
  font->print_center(CL_Display::get_width() / 2, 140, str);

  snprintf(str, 128, _("Pingus died:  %3d/%3d"),
	  world->get_pingus()->get_number_of_allowed()
           - world->get_pingus()->get_number_of_exited(),
	  world->get_pingus()->get_number_of_allowed());
  font->print_center(CL_Display::get_width() / 2, 160, str);

  /*
  snprintf(str, 128, _("Required Time: %2d:%2d:%2d"),
	  result.time / (60 * game_speed),
	  result.time / game_speed % 60,
	  (result.time * 100) / game_speed % 100);
  */

  //font->print_center(CL_Display::get_width() / 2, 180, str);

  font->print_center(CL_Display::get_width()/2, CL_Display::get_height() - 80,
		     _("Press button to continue..."));
  Display::flip_display();

  while(!CL_Mouse::left_pressed())
    CL_System::keep_alive();

  while(CL_Mouse::left_pressed())
    CL_System::keep_alive();
}

std::string
PingusLevelResult::get_message (int saved)
{
  if (saved == 100) {
    return _("As many Pingus escaped as entered the level. That's going to be hard to beat.... unless this game becomes pornographic.");
  } else if (saved > 90) {
    return _("Very impressive indeed.");
  } else if (saved > 80) {
    return _("Good work. Still room for improvement though.");
  } else if (saved > 70) {
    return _("Not too shabby, not too shabby at all.");
  } else if (saved > 60) {
    return _("That was OK, but Pingu life insurance premiums have just gotten more expensive.");
  } else if (saved > 55) {
    return _("Maybe this level calls for a different strategy.");
  } else if (saved > 50) {
    return _("Exactly half. Are you saving only the female ones?");
  } else if (saved > 40) {
    return _("If I were a Pingu, I never would have left that entrance.");
  } else if (saved > 30) {
    return _("Maybe you would feel more at home playing Quake.");
  } else if (saved > 20) {
    return _("Maybe this level calls for a different strategy. Like attempting to save them, for example.");
  } else if (saved > 10) {
    return _("Ever considered a career as a Pingu exterminator?");
  } else if (saved > 0) {
    return _("You missed one! What's your excuse!?");
  } else if (saved == 0) {
    return _("Please reassure me that you hit the Armageddon button.");
  } else {
    return _("You've got a negative save/total value, something is buggy.");
  }
}

} // namespace Pingus

/* EOF */
