//  $Id: PingusMenu.cc,v 1.8 2000/02/28 03:50:42 grumbel Exp $
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
#include "Credits.hh"
// #include "FileSelector.hh"
#include "PingusResource.hh"
#include "PingusMessageBox.hh"
#include "PingusError.hh"
#include "PingusMenu.hh"
#include "Loading.hh"

PingusMenu::PingusMenu()
{
  bg         = CL_Surface::load("Game/logo_t", PingusResource::get("game.dat"));
  background = CL_Surface::load("Textures/stones", PingusResource::get("textures.dat"));
 
  event = new Event;
  event->enabled = false;
  event->menu = this;
  
  CL_Input::chain_button_press.push_back(event);
  CL_Input::chain_button_release.push_back(event);
  CL_Input::chain_mouse_move.push_back(event);

  buttons.push_back(&options_button);
  buttons.push_back(&play_button);
  buttons.push_back(&quit_button);
  buttons.push_back(&editor_button);
  buttons.push_back(&theme_button);
}

PingusMenu::~PingusMenu()
{
  CL_Input::chain_mouse_move.remove(event);
  CL_Input::chain_button_release.remove(event);
  CL_Input::chain_button_press.remove(event);

  delete event;
}

void
PingusMenu::draw()
{
  CL_Display::clear_display();

  for(int y = 0; y < CL_Display::get_height(); y += background->get_height())
    for(int x = 0; x < CL_Display::get_width(); x += background->get_width())
      background->put_screen(x, y);

  bg->put_screen(CL_Display::get_width()/2 - bg->get_width()/2, 3);

  current_button = 0;
  for(list<SurfaceButton*>::iterator i = buttons.begin(); i != buttons.end(); i++)
    {
      current_button = *i;
      (*i)->draw();
    }
  
  CL_Display::flip_display();
}

void
PingusMenu::select(void)
{
  if (quick_play) 
    {
      PingusGame game;
      game.start ();
      quick_play = false;
    }
  
  if (start_editor) {
    editor_button.on_click ();
  }

  do_quit = false;

  draw();

  event->enabled = true;

  CL_MouseCursor::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor", PingusResource::get("game.dat")));

  CL_MouseCursor::show();
 
  while(!do_quit) 
    {
      CL_System::keep_alive();
      
      current_button = 0;
      for(list<SurfaceButton*>::iterator i = buttons.begin(); i != buttons.end(); i++)
	{
	  if ((*i)->mouse_over())
	    {
	      current_button = *i;
	      break;
	    }
	}
      
      if (CL_Mouse::left_pressed()) 
	{
	  draw();

	  while(CL_Mouse::left_pressed()) {
	    CL_System::keep_alive();
	  }

	  for(list<SurfaceButton*>::iterator i = buttons.begin(); i != buttons.end(); i++)
	    {
	      if ((*i)->mouse_over())
		{
		  event->enabled = false;
		  if (&quit_button == *i)
		    {
		      do_quit = true;
		    }
		  CL_MouseCursor::hide();
		  (*i)->on_click();
		  CL_MouseCursor::show();
  
		  event->enabled = true;
		  break;
		}
	    }
	  draw();  
	}
    }
  event->enabled = false;

  CL_MouseCursor::hide();
}

bool
PingusMenu::Event::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled) return true;
  
  if (device == CL_Input::keyboards[0])
    {
      switch(key.id)
	{
	case CL_KEY_C:
	  {
	    enabled = false;
	    Credits credits;
	    credits.display();
	    menu->draw();
	    enabled = true;
	  }
	  break;
	default:
	  cout << "Unknown key pressed:" << key.ascii << endl;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      if (verbose) std::cout << "PingusMenu::Event: on_button_press" << std::endl;
    }  
  return true;
}

bool
PingusMenu::Event::on_mouse_move(CL_InputDevice *device)
{
  if (!enabled) return true;
 
  if (device == CL_Input::pointers[0])
    {
      menu->draw();
    }
  return true;
}

bool
PingusMenu::Event::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled) return true;
  
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
	  break;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      
    }
  return true;
}

/* EOF */
