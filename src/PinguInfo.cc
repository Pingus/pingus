//  $Id: PinguInfo.cc,v 1.1 2000/02/04 23:45:18 mbn Exp $
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

#include "algo.hh"
#include "PingusResource.hh"
#include "PinguInfo.hh"

PinguInfo::PinguInfo()
{
  font = CL_Font::load("Fonts/courier_small", PingusResource::get("fonts.dat"));
  pingu = 0;
}

void
PinguInfo::draw()
{
  CL_Display::fill_rect(CL_Display::get_width()  - 220,
			CL_Display::get_height() - 55,
			CL_Display::get_width()  - 5,
			CL_Display::get_height() - 5,
			0.2, 0.2, 0.2, 1.0);
  if (pingu) {
    char str1[256];
    char str2[256];

    sprintf(str1, "pingu_id %d", pingu->get_id());
    font->print_left(CL_Display::get_width() - 185, 
		     CL_Display::get_height() - 50,
		     tolowerstr(str1));

    if (pingu->get_action()) {
      sprintf(str2, "action %s", pingu->get_action()->name().c_str());
    } else {
      sprintf(str2, "action none");
    }

    font->print_left(CL_Display::get_width() - 185, 
		     CL_Display::get_height() - 30,
		     tolowerstr(str2));
  } else {
    font->print_left(CL_Display::get_width() - 200, 
		     CL_Display::get_height() - 50,
		     "none selected");
  }
}

void
PinguInfo::set_pingu(Pingu* p)
{
  pingu = p;
}

/* EOF */
