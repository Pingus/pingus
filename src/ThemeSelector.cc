//  $Id: ThemeSelector.cc,v 1.17 2000/06/14 21:45:55 grumbel Exp $
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

#include <algorithm>
#include <config.h>
#include "globals.hh"
#include "System.hh"
#include "PingusError.hh"
#include "PingusResource.hh"
#include "AlphaButton.hh"
#include "Display.hh"
#include "Loading.hh"
#include "ThemeSelector.hh"
#include "PingusSound.hh"

ListBox::ListBox()
{
}

void
ListBox::draw_offset(int x, int y)
{
  int i = 0;
  CL_Display::fill_rect(x, y, x + 200, CL_Display::get_height() - 20,
			0.0, 0.0, 0.0, 0.5);
  for(ListIter item = items.begin(); item != items.end(); ++item) {
    item->draw_offset(x, y + (i++ * 20));
  }
}

void
ListBox::add_item(std::string l)
{
  items.push_back(ListItem(l));
}

// ---=== List item ===---

ListItem::ListItem(std::string l)
{
  label = l;
  font = CL_Font::load("Fonts/smallfont_h", PingusResource::get("fonts.dat"));
}

void
ListItem::draw_offset(int x, int y)
{
  font->print_left(x, y, label.c_str());
}

// ---=== ThemeSelector ===---

bool 
ThemeSelector::Event::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled) return true;  

  return true;
}

bool
ThemeSelector::Event::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled) return true;

  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	case CL_KEY_LEFT:
	  theme_selector->current_theme++;
	  if (theme_selector->current_theme == theme_selector->themes.end()) 
	    theme_selector->current_theme = theme_selector->themes.begin();
	  break;

	case CL_KEY_RIGHT:
	  if (theme_selector->current_theme == theme_selector->themes.begin()) 
	    theme_selector->current_theme = theme_selector->themes.end();
	  theme_selector->current_theme--;
	  break;
	  
	case CL_KEY_DOWN:
	  (*(theme_selector->current_theme))->next_level();
	  break;
	case CL_KEY_UP:
	  (*(theme_selector->current_theme))->previous_level();
	  break;

	case CL_KEY_ENTER:
	  enabled = false;
	  loading_screen.draw();
	  (*(theme_selector->current_theme))->play();
	  enabled = true;
#ifdef HAVE_LIBSDL_MIXER
	  PingusSound::play_mod("../data/music/pingus-1.it");
#endif
	  break;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      switch (key.id)
	{
	case 0:
	  // std::cout << "X: " << key.x << std::endl;
	  // std::cout << "Y: " << key.y << std::endl;

	  if (key.x > 0 && key.x < theme_selector->left_arrow->get_width()
	      && key.y > (CL_Display::get_height() - theme_selector->left_arrow->get_height()) / 2
	      && key.y < (CL_Display::get_height() + theme_selector->left_arrow->get_height()) / 2)
	    {
	      // std::cout << "Left pressed" << std::endl;
	      theme_selector->current_theme++;
	      if (theme_selector->current_theme == theme_selector->themes.end()) 
		theme_selector->current_theme = theme_selector->themes.begin();
	    }
	  else if (key.x > CL_Display::get_width() - theme_selector->right_arrow->get_width()
		   && key.x < CL_Display::get_width()
		   && key.y > (CL_Display::get_height() - theme_selector->right_arrow->get_height()) / 2
		   && key.y < (CL_Display::get_height() + theme_selector->right_arrow->get_height()) / 2)
	    {
	      // std::cout << "Right pressed" << std::endl;
	      if (theme_selector->current_theme == theme_selector->themes.begin()) 
		theme_selector->current_theme = theme_selector->themes.end();
	      theme_selector->current_theme--;
	    }

	  break;
	default:
	  break;
	}
    }
  
  theme_selector->draw();

  return true;
}

ThemeSelector::ThemeSelector()
{
  dir_read = false;
  title_font = CL_Font::load("Fonts/pingus", PingusResource::get("fonts.dat"));
  theme_font = CL_Font::load("Fonts/pingus_small", PingusResource::get("fonts.dat"));

  left_arrow = CL_Surface::load("Hotspots/left_arrow", PingusResource::get("global.dat"));
  right_arrow = CL_Surface::load("Hotspots/right_arrow", PingusResource::get("global.dat"));

  event = new ThemeSelector::Event;
  event->enabled = false;

  event->theme_selector = this;
  
  CL_Input::chain_button_press.push_back(event);
  CL_Input::chain_button_release.push_back(event);
}

ThemeSelector::~ThemeSelector()
{
  // FIXME: Deleting the stuff, which was allocated in readdir()
  // FIXME: Should be rewritten
  for(std::vector<Theme*>::iterator i = themes.begin(); i != themes.end(); i++)
    delete (*i);

  CL_Input::chain_button_press.remove(event);
  CL_Input::chain_button_release.remove(event);
  
  delete event;
}

void
ThemeSelector::select()
{
  event->enabled = true;
  
  while(CL_Mouse::left_pressed() || CL_Mouse::middle_pressed()) 
    CL_System::keep_alive();
  
  if (!dir_read) 
    {
      loading_screen.draw();
      
      readdir(pingus_datadir);
      dir_read = true;
    }
  current_theme = themes.end()-1;

#ifdef HAVE_LIBSDL_MIXER
  PingusSound::play_mod("../data/music/pingus-1.it");
#endif

  Display::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor", PingusResource::get("game.dat")));
  Display::show_cursor(true);

  while(!key_pressed(CL_KEY_ESCAPE)) 
    {
      CL_System::keep_alive();
      draw();
    }

  Display::hide_cursor();
  event->enabled = false;
}

void
ThemeSelector::draw()
{
  CL_Display::fill_rect(0,0,640,480,0.5,0.0,0.0,0.5);

  (*current_theme)->draw_title();
  
  {
    int item_width = (CL_Display::get_width() / themes.size());
    int item_index = themes.size() - distance(themes.begin(), current_theme) - 1;
    
    Display::draw_rect(item_index * item_width, CL_Display::get_height() - 15, 
		       (item_index + 1) * item_width, CL_Display::get_height(),
		       0.0, 1.0, 0.0, 1.0);
  }
  
  left_arrow->put_screen(0, (CL_Display::get_height() - left_arrow->get_height()) / 2);
  right_arrow->put_screen(CL_Display::get_width() - right_arrow->get_width(),
			  (CL_Display::get_height() - right_arrow->get_height()) / 2);
  
  theme_font->print_center(CL_Display::get_width()/2, CL_Display::get_height() - 50,
			   "..:: Use the cursor keys to select a level ::..");

  Display::flip_display();
}

void
ThemeSelector::readdir(std::string path)
{
  System::Directory dir; 
  std::string pathname;
  std::string::size_type pos = 0; 
  std::string::size_type last_pos = 0; 
  bool exit_for = false;

  for(pos = path.find(":", last_pos);
      !exit_for;
      last_pos = pos + 1, pos = path.find(":", last_pos))
    {
      if (pos == std::string::npos) 
	{
	  pathname = path.substr(last_pos, std::string::npos);
	  exit_for = true;
	} 
      else 
	{
	  pathname = path.substr(last_pos, pos - last_pos);
	}
      
      dir = System::opendir(pathname + "/themes", "*.plt");
      
      for(System::Directory::iterator entry = dir.begin(); entry != dir.end(); entry++)
	{
	  if (verbose) std::cout << "Entry Name: " << pathname + "/themes/" + entry->name << std::endl;
	  themes.push_back(new Theme(pathname + "/themes/" + entry->name));
	}
    }
}

bool
ThemeSelector::key_pressed(int key)
{
  if (CL_Keyboard::get_keycode(key)) {
    while(CL_Keyboard::get_keycode(key)) {
      CL_System::keep_alive();
    }
    return true;
  } else {
    return false;
  } 
}

/* EOF */
