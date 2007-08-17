//  $Id: fps_counter.cxx,v 1.11 2003/10/21 21:37:06 grumbel Exp $
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

#include <stdio.h>
#include "fonts.hpp"
#include "fps_counter.hpp"
#include "gettext.h"


FPSCounter fps_counter;

FPSCounter::FPSCounter()
{
}

FPSCounter::~FPSCounter()
{
	font = Font();
}

// We are not initialising the fpscounter in the constructor, 'cause
// that doesn't work (ClanLib hasn't init the display at that point)
void
FPSCounter::init()
{
  font = Fonts::pingus_small;
  start_time = SDL_GetTicks();
  strcat(fps_string, "");
  fps_count = 0;
}

// Get rid of any ClanLib objects that might want to linger after
// we unload all of our resources.
void FPSCounter::deinit()
{
  font = Font();
}

void
FPSCounter::on_event()
{
  update_fps_counter();

  if (odd_frame)
    {
      font.draw(Display::get_width(),
                Display::get_height() - (2 * font.get_height()),
                "o");

      odd_frame = false;
    }
  else
    {
      odd_frame = true;
    }

  font.draw(Display::get_width() - 100,
            Display::get_height() - font.get_height(),
            fps_string);
}

void
FPSCounter::update_fps_counter()
{
  unsigned int current_time = SDL_GetTicks();
  int current_fps;

  fps_count++;

  if (start_time + 1000 < current_time)
    {
      current_fps = fps_count * 1000 / (current_time - start_time);
      snprintf(fps_string, 16, "%d fps", current_fps);
      fps_count = 0;
      start_time = SDL_GetTicks();
    }
}


/* EOF */
