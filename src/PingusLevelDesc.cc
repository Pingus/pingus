//  $Id: PingusLevelDesc.cc,v 1.1 2000/02/04 23:45:18 mbn Exp $
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

#include "PingusResource.hh"
#include "PingusLevelDesc.hh"

PingusLevelDesc::PingusLevelDesc(PLF* tplf)
{
  typedef string::size_type ST;
  ST pos, pos_next;
  string desc_string;
  plf = tplf;

  desc_string = plf->get_description();
  levelname = plf->get_levelname();

  // BUG: Begin, this is very buggy and don't work in all situations
  pos = 0;
  pos_next = desc_string.find("\n", 0);
  
  if (pos_next == string::npos) {
    description.push_back(desc_string);
  }

  while(pos_next != string::npos)
    {
      description.push_back(desc_string.substr(pos, pos_next - pos));
      
      pos = pos_next + 1;
      if (pos >= desc_string.size())
	break;

      pos_next = desc_string.find("\n", pos + 1);
    }
  if (pos_next == string::npos) {
    description.push_back(desc_string.substr(pos, desc_string.size() - pos));
  }
  // BUG: End
  
  background = CL_Surface::load("Textures/stone", PingusResource::get("global.dat"));
  font       = CL_Font::load("Fonts/courier_small", PingusResource::get("fonts.dat"));
  title      = CL_Font::load("Fonts/clanfont", PingusResource::get("fonts.dat"));
}

void
PingusLevelDesc::draw(PingusLevelDesc::LoadingStatus status)
{
  CL_Display::clear_display();
  background->put_screen(0, 0, CL_Display::get_width(), CL_Display::get_height());
  CL_Display::fill_rect(0, 0, CL_Display::get_width(), CL_Display::get_height(), 0.0, 0.0, 0.0, 0.5);
  title->print_center(CL_Display::get_width() / 2, 50, levelname.c_str());

  for(unsigned int i=0; i < description.size(); ++i)
    font->print_center(CL_Display::get_width() / 2 - 20, 100 + (20 * i), description[i].c_str());


  if (status == LOADING) {
    font->print_center(CL_Display::get_width() / 2 - 20, CL_Display::get_height() - 80, "Loading...");
  } else {
    font->print_center(CL_Display::get_width() / 2 - 20, CL_Display::get_height() - 80, "Loading finished, the level will start in the next seconds.");
  }

  {
    char str[1024];
    sprintf(str, "Pingus to Save: %d", plf->get_number_to_save());
    font->print_center(CL_Display::get_width() / 2 - 20, 200, str);
    sprintf(str, "Number of Pingus: %d", plf->get_pingus());
    font->print_center(CL_Display::get_width() / 2 - 20, 220, str);
  }

  CL_Display::flip_display();
}

/* EOF */
