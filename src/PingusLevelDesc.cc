//  $Id: PingusLevelDesc.cc,v 1.20 2001/04/13 11:26:54 grumbel Exp $
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

#include <iostream>
#include <cstdio>
#include <string>

#include "my_gettext.hh"

#include "Display.hh"
#include "PingusResource.hh"
#include "PingusLevelDesc.hh"
#include "System.hh"

PingusLevelDesc::PingusLevelDesc(boost::shared_ptr<PLF> arg_plf)
{
  plf = arg_plf;

  background = PingusResource::load_surface("Textures/stone", "textures");
  font       = PingusResource::load_font("Fonts/pingus_small","fonts");
  title      = PingusResource::load_font("Fonts/pingus","fonts");

  description.set_font(font);
  description.set_text(System::translate(plf->get_description()), 350);
  levelname = System::translate(plf->get_levelname());
}

void
PingusLevelDesc::draw(PingusLevelDesc::LoadingStatus status)
{
  int x_pos = CL_Display::get_width() / 2 ;
  int y_pos = 120;
    
  for(int y = 0; y < CL_Display::get_height(); y += background.get_height())
    for(int x = 0; x < CL_Display::get_width(); x += background.get_width())
      background.put_screen(x, y);

  CL_Display::fill_rect(0, 0, CL_Display::get_width(), CL_Display::get_height(), 0.0, 0.0, 0.0, 0.5);

  title->print_center(CL_Display::get_width() / 2, 50, levelname.c_str());

  {
    CL_Display::fill_rect(x_pos - description.get_width()/2 - 15,
			  y_pos - 15,
			  x_pos + description.get_width()/2 + 15,
			  y_pos + description.get_height() + 15,
			  0.0, 0.0, 0.0, 0.5);
    description.print_center(x_pos, y_pos);
    y_pos += description.get_height() + 40;
  }
  
  if (!plf->get_author().empty())
    {
      font->print_center(CL_Display::get_width() / 2, 
			 CL_Display::get_height() - 30,
			 (_("Designed by ") + plf->get_author()).c_str());
    }

  {
    char str[1024];
    sprintf(str, _("Pingus to Save: %d"), plf->get_number_to_save());
    font->print_center(CL_Display::get_width() / 2, y_pos, str);
    sprintf(str, _("Number of Pingus: %d"), plf->get_pingus());
    font->print_center(CL_Display::get_width() / 2, (y_pos += 20), str);
  }

  if (status == LOADING) {
    font->print_center(CL_Display::get_width() / 2, CL_Display::get_height() - 80, _("Loading..."));
    Display::flip_display();    
  } else {
    font->print_center(CL_Display::get_width() / 2, CL_Display::get_height() - 80, _("Loading finished. Press a mouse button to start the level"));
    Display::flip_display();
    
    // FIXME: Busy waiting... ugly
    while (!CL_Mouse::left_pressed() && !CL_Mouse::right_pressed() && !CL_Mouse::middle_pressed())
      {
	CL_System::keep_alive();
	CL_System::sleep (50);
      }
  }
}

/* EOF */
  
