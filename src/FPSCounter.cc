//  $Id: FPSCounter.cc,v 1.1 2000/06/12 20:33:47 grumbel Exp $
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

#include <cstdio>
#include <ClanLib/core.h>
#include "PingusResource.hh"
#include "FPSCounter.hh"

FPSCounter::FPSCounter()
{
  font = CL_Font::load("Fonts/pingus_small", PingusResource::get("fonts.dat"));
  start_time = CL_System::get_time();
  strcat(fps_string, "unknown");
  fps_count = 0;
}

FPSCounter::~FPSCounter()
{
}

void
FPSCounter::on_event()
{
  update_fps_counter();

  font->print_right(CL_Display::get_width(), 
		    CL_Display::get_height() - font->get_height(),
		    fps_string);
}

void 
FPSCounter::update_fps_counter()
{
  unsigned int current_time = CL_System::get_time();
  int current_fps;

  fps_count++;
  
  if (start_time + 1000 < current_time) {
    current_fps = fps_count * 1000 / (current_time - start_time);
    sprintf(fps_string, "%d fps", current_fps);
    fps_count = 0;
    start_time = CL_System::get_time();
  }
}

/* EOF */
