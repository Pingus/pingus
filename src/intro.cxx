//  $Id: intro.cxx,v 1.3 2002/08/16 17:15:31 grumbel Exp $
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

#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/keyboard.h>
#include "pingus_menu_manager.hxx"
#include "pingus_resource.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"


Intro::Intro(PingusMenuManager* m)
  : PingusSubMenu (m)
{
  blink_time = 0;
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
      CL_Display::clear_display (0.0, 0.0, 0.0, 
				 abs(int(CL_Display::get_height ()/2 - pos.y)) / 75.0f);
      break;
    case WAITING:
      break;
    case FINISHED:
      if (blink_time == 0)
	blink_time = CL_System::get_time ();

      // One second has passed
      if (blink_time + 1500 < CL_System::get_time ())
	blink_time = CL_System::get_time ();
      
      // A half second has passed so draw the image
      if (blink_time + 500 < CL_System::get_time ())
	font->print_center (CL_Display::get_width ()/2, 
			    CL_Display::get_height ()/2 + CL_Display::get_height ()/4,
			    _("..:: Press Start ::.."));
      break;
    }

  logo.put_screen ((int)pos.x - (logo.get_width ()/2),
		   (int)pos.y - (logo.get_height ()/2));
}

void 
Intro::update (float delta)
{
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
	start_time = CL_System::get_time ();
      }
      break;      
    case WAITING:
      if (start_time + 1000 < CL_System::get_time ())
	stage = FINISHED;
      break;
    case FINISHED:
      break;
    }
}

void 
Intro::preload ()
{
  font = PingusResource::load_font ("Fonts/pingus","fonts");
  logo = PingusResource::load_surface ("misc/logo", "core");

  velocity = CL_Vector (0, -75);
  pos = CL_Vector (CL_Display::get_width ()/2,
		   CL_Display::get_height () + logo.get_height ());
  
  
  font->print_center (CL_Display::get_width ()/2, 
		      CL_Display::get_height ()/2 + CL_Display::get_height ()/4,
		      _("..:: Press Start ::.."));
  stage = SCROLL_UP;
}


/*void 
Intro::on_button_press (CL_InputDevice* device,const CL_Key& key)
{
  if (device == CL_Input::keyboards[0] && key.id == CL_KEY_SPACE)
    manager->set_menu (&(manager->mainmenu));
}*/

/* EOF */
