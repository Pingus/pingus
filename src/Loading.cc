//  $Id: Loading.cc,v 1.6 2000/06/12 14:42:10 grumbel Exp $
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

#include "PingusResource.hh"
#include "Display.hh"
#include "Loading.hh"

Loading loading_screen;

Loading::Loading()
{
  is_init = false;
}

Loading::~Loading()
{ 
}

void
Loading::init()
{
  sur = CL_Surface::load("Game/loading", PingusResource::get("game.dat"));
  font = CL_Font::load("Fonts/courier_small",PingusResource::get("fonts.dat"));
  is_init = true;
}

void
Loading::draw()
{
  bool cursor_shown = Display::cursor_shown();

  if (!is_init)
    init();
  
  if (cursor_shown)
    Display::hide_cursor();

  CL_Display::clear_display();

  sur->put_screen((CL_Display::get_width() - sur->get_width())/2,
		  (CL_Display::get_height() - sur->get_height())/2);

  Display::flip_display();

  if (cursor_shown)
    Display::show_cursor();
}

void
Loading::draw_progress(std::string str, float progress)
{
  bool cursor_shown = Display::cursor_shown();

  if (!is_init)
    init();

  if (cursor_shown)
    Display::hide_cursor();

  CL_Display::clear_display();

  sur->put_screen((CL_Display::get_width() - sur->get_width())/2,
		  (CL_Display::get_height() - sur->get_height())/2);

  font->print_center(CL_Display::get_width() / 2, 
		     CL_Display::get_height() - 130,
		     str.c_str());

  CL_Display::fill_rect(50, CL_Display::get_height() - 100,
			CL_Display::get_width() - 50, CL_Display::get_height() - 50,
			0.5, 0.5, 0.5, 1.0);

  CL_Display::fill_rect(50, CL_Display::get_height() - 100,
			50 + (int)((CL_Display::get_width()-100) * progress), CL_Display::get_height() - 50,
			1.0, 0.0, 0.0, 1.0);

  Display::flip_display();

  if (cursor_shown)
    Display::show_cursor();
}

/* EOF */



