//  $Id: animtest.cxx,v 1.2 2003/03/04 11:26:17 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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
#include <ClanLib/png.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "animtest.hxx"

Animtest::Animtest()
{
}

int
Animtest::main(int argc, char** argv)
{
  int screen_width  = 200;
  int screen_height = 200;

  if (argc != 4)
    {
      std::cout << "Usage: animtest [PNG-FILE] [SLEEP] [X-STEP]" << std::endl;
      return 0;
    }
  
  unsigned int sleep_value = atoi(argv[2]);
  float step_value = atof(argv[3]);
  
  CL_SetupCore::init ();
  CL_SetupDisplay::init();
  CL_SetupPNG::init ();

  CL_Display::set_videomode(screen_width, screen_height, 16, 
                            false, false);

  CL_SurfaceProvider* prov = new CL_PNGProvider(argv[1]);
  CL_Surface* sur = CL_SpriteSubarrayProvider::create(prov,
                                                      0, 0,
                                                      32, 64, 8, 1);
  int frame = 0;
  bool multi_direct = false;
  float x_of = 100;
  while (1)
    {
      CL_Display::clear_display(1.0f, 1.0f, 0.0f, 1.0f);

      sur->put_screen(-(sur->get_width()/2) + int(x_of),
                      100 - (sur->get_height()/2), 
                      frame);
      frame += 1;
      if (frame >= (sur->get_num_frames()/2))
        frame = 0; 
      if (x_of < -100)
        x_of = 100;
      x_of -= step_value;
      
      CL_Display::flip_display();
      CL_System::sleep(sleep_value);
    }
}

/* EOF */
