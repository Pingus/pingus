//  $Id: message_box.cxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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
#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/Input/mouse.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include "display.hxx"
#include "pingus_resource.hxx"
#include "message_box.hxx"

PingusMessageBox::PingusMessageBox(std::string s)
{
  str = s;
  std::cout << "Error: " << str << std::endl;
  font = PingusResource::load_font("Fonts/xterm","fonts");
  draw();
}

void
PingusMessageBox::draw()
{
  int width  = CL_Display::get_width();
  int height = CL_Display::get_height();

  CL_Display::fill_rect( 80, 200, width -  80, height - 200, 1.0, 1.0, 1.0, 0.5);
  CL_Display::fill_rect( 90, 210, width -  90, height - 210, 0.0, 0.0, 0.0, 1.0);
  font->print_center(CL_Display::get_width() / 2 - 25, CL_Display::get_height() / 2 - 8,
		     str.c_str());

  Display::flip_display();
  while(!CL_Mouse::left_pressed())
    CL_System::keep_alive();
  while(CL_Mouse::left_pressed())
    CL_System::keep_alive();
}

/* EOF */



