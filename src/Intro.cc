//  $Id: Intro.cc,v 1.11 2000/06/19 20:10:38 grumbel Exp $
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

#include "PingusResource.hh"
#include "Display.hh"
#include "globals.hh"
#include "Loading.hh"
#include "algo.hh"
#include "Story.hh"
#include "Intro.hh"

Intro::Intro()
{
  event = new Intro::Event;

  event->intro = this;
}

Intro::~Intro()
{
  if (music_enabled) {
  }
  delete event;
}

void
Intro::draw()
{
  quit = false;

  if (verbose) std::cout << "Intro: adding event handler" << std::endl;

  CL_Input::chain_button_release.push_back(event);
  /*
  pingus_story.init();
  pingus_story.display();
  */
  int x_pos = CL_Display::get_width() / 2;
  int y_pos = 0;
  
  logo = CL_Surface::load("Game/logo_t", PingusResource::get("game.dat"));

  for(y_pos = -(int)(logo->get_height())/2; 
      (y_pos < CL_Display::get_height() / 2 - 110) && !quit;
      y_pos += 2)
    {
      CL_Display::fill_rect(0,0, CL_Display::get_width(), CL_Display::get_height(),
			    1.0, 1.0, 1.0, 1.0);
      logo->put_screen(x_pos - logo->get_width()/2,
		       y_pos - logo->get_height()/2);
      Display::flip_display(true);
      CL_System::keep_alive();
    }
  
  if (sound_enabled) 
    {
      ses_sound = sound->play();
    }
  
  for(float i=1.0; (i > 0.0) && !quit; i -= 0.05)
    {
      CL_Display::fill_rect(0,0, CL_Display::get_width(), CL_Display::get_height(),
			    i, i, i, 1.0);
      logo->put_screen(x_pos - logo->get_width()/2,
		       y_pos - logo->get_height()/2);
      Display::flip_display(true);
      CL_System::keep_alive();
    }

  if (quit)
    loading_screen.draw();

  if (verbose) std::cout << "Intro: Removing event handler" << std::endl;
  CL_Input::chain_button_release.remove(event);
}

bool
Intro::Event::on_button_release(CL_InputDevice* device, const CL_Key& key)
{
  switch (key.id)
    {
    case CL_KEY_SPACE:
    case CL_KEY_ESCAPE:
    case CL_KEY_ENTER:
      if (verbose) std::cout << "Intro::Event: Recieved event, stopping intro" << std::endl;
      intro->quit = true;
      break;
    }
  return true;
}

/* EOF */
