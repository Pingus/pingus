//  $Id: PingusMenu.cc,v 1.3 2000/02/11 16:58:26 grumbel Exp $
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

#include "globals.hh"
#include "AlphaButton.hh"
#include "PingusGame.hh"
// #include "FileSelector.hh"
#include "PingusResource.hh"
#include "PingusMessageBox.hh"
#include "PingusError.hh"
#include "PingusMenu.hh"

PingusMenu::PingusMenu()
{
  bg         = CL_Surface::load("Game/logo_t", PingusResource::get("game.dat"));
  background = CL_Surface::load("Game/logo_bg", PingusResource::get("game.dat"));
  cursor_sur = CL_Surface::load("Cursors/cursor", PingusResource::get("game.dat"));
 
  buttons.push_back(new AlphaButton("Quit", 260, 200, 60, 40));
  buttons.push_back(new AlphaButton("Play", 260, 240, 60, 40));
  buttons.push_back(new AlphaButton("Load", 320, 200, 60, 40));
  buttons.push_back(new AlphaButton("Theme", 320, 240, 60, 40));
  buttons.push_back(new AlphaButton("Options", 320, 280, 60, 40));

  do_quit = false;
  
  event = new Event;
}

PingusMenu::~PingusMenu()
{
  delete event;
}

void
PingusMenu::draw(void)
{
  CL_Display::clear_display();
  background->put_screen(0, CL_Display::get_height() - 300);
  bg->put_screen(CL_Display::get_width()/2 - bg->get_width()/2,
		 3);
  options_button.draw();
  play_button.draw();
  quit_button.draw();
  // load_button.draw();
  editor_button.draw();
  theme_button.draw();
  CL_Display::flip_display();
  
  /*
  CL_Display::fill_rect(250, 190, 390, 330,
			0.0, 0.0, 0.0, 0.3);

  for(ButtonIter button = buttons.begin(); button != buttons.end(); ++button) {
    (*button)->draw();
  }

  if (cursor_enabled) cursor_sur->put_screen(CL_Mouse::get_x(), CL_Mouse::get_y());
  */  
}

void
PingusMenu::select(void)
{
  /*  buffer = CL_SoundBuffer::create(new CL_Streamed_MikModSample("../data/music/gd-walk.it"), true);
  buffer2 = CL_SoundBuffer::create(new CL_Streamed_MikModSample("../data/music/strut.it"), true);

  ses = new CL_SoundBuffer_Session(buffer->play());
  */
  if (quick_play) {
    PingusGame game;
    game.start ();
    quick_play = false;
  }
  
  if (start_editor) {
    editor_button.on_click ();
  }
  
  while(!do_quit) {
    CL_System::keep_alive();

    /*if (CL_Keyboard::get_keycode(CL_KEY_ESCAPE)) {
      do_quit = true;
    }*/

    draw();
    if (CL_Mouse::left_pressed()) {
      draw();
      while(CL_Mouse::left_pressed()) {
	CL_System::keep_alive();
      }
      draw();  

      if (play_button.mouse_over()) {
	play_button.on_click();
      } else if (options_button.mouse_over()) {
	options_button.on_click();
      } else if (quit_button.mouse_over()) {
	do_quit = true;
	/*      } else if (load_button.mouse_over()) {
		load_button.on_click();*/
      } else if (editor_button.mouse_over()) {
	editor_button.on_click();
      } else if (theme_button.mouse_over()) {
	theme_button.on_click();
      }
    }
  }
}

bool
PingusMenu::Event::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::keyboards[0])
    {
    }
  else if (device == CL_Input::pointers[0])
    {
      if (verbose) std::cout << "PingusMenu::Event: on_button_press" << std::endl;
    }  
  return true;
}

bool
PingusMenu::Event::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	case CL_KEY_L:
	  break;
	case CL_KEY_F:
	  break;
	default:
	  if (verbose) std::cout << "PingusMenu::Event: Unknown key pressed: " << key.id << std::endl;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      
    }
  return true;
}

/* EOF */
