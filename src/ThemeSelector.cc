//  $Id: ThemeSelector.cc,v 1.46 2001/11/18 23:21:33 grumbel Exp $
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
#include "PathManager.hh"

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
  font = PingusResource::load_font("Fonts/smallfont_h","fonts");
}

void
ListItem::draw_offset(int x, int y)
{
  font->print_left(x, y, label.c_str());
}

// ---=== ThemeSelector ===---

void 
ThemeSelector::Event::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled) return;  
}

void
ThemeSelector::Event::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled) return;

  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	case CL_KEY_ESCAPE:
	  theme_selector->finished = true;
	  break;
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
	  PingusSound::play_music("../data/music/pingus-1.it");
	  break;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      int tmp_level;
      switch (key.id)
	{
	case CL_MOUSE_LEFTBUTTON: // Left mouse button
	  tmp_level = theme_selector->mark_level_at_point(int(key.x), int(key.y));
	  if (tmp_level != -1) 
	    {
	      // We clicked on a level, start it now.
	      enabled = false;
	      loading_screen.draw();
	      (*(theme_selector->current_theme))->play();
	      enabled = true; 
	    } 
	  else
	    {
	      // Check if we clicked on one of the red buttons
	      if (key.x > 0 && key.x < theme_selector->left_arrow.get_width()
		  && key.y > (CL_Display::get_height() - theme_selector->left_arrow.get_height()) / 2
		  && key.y < (CL_Display::get_height() + theme_selector->left_arrow.get_height()) / 2)
		{
		  theme_selector->current_theme++;
		  if (theme_selector->current_theme == theme_selector->themes.end()) 
		    theme_selector->current_theme = theme_selector->themes.begin();
		}
	      else if (key.x > CL_Display::get_width() - theme_selector->right_arrow.get_width()
		       && key.x < CL_Display::get_width()
		       && key.y > (CL_Display::get_height() - theme_selector->right_arrow.get_height()) / 2
		       && key.y < (CL_Display::get_height() + theme_selector->right_arrow.get_height()) / 2)
		{
		  if (theme_selector->current_theme == theme_selector->themes.begin()) 
		    theme_selector->current_theme = theme_selector->themes.end();
		  theme_selector->current_theme--;
		}
	      else if (key.x < theme_selector->back.get_width()
		       && key.y < theme_selector->back.get_height())
		{
		  theme_selector->finished = true;
		}
	    }
	  break;
	case CL_MOUSE_MIDDLEBUTTON: // Middle mouse
	  theme_selector->finished = true;	  
	  break;
	default:
	  break;
	}
    }
  
  theme_selector->draw();

  return;
}

void
ThemeSelector::Event::on_mouse_move(CL_InputDevice *, int mouse_x, int mouse_y)
{
  //std::cout << "Event: on_mouse_move called.." << std::endl;
  if (!enabled) return;  
  //std::cout << "Event: on_mouse_move active.." << std::endl;  
  theme_selector->mark_level_at_point(mouse_x, mouse_y);
} 

ThemeSelector::ThemeSelector()
{
  dir_read = false;
  title_font = PingusResource::load_font("Fonts/pingus", "fonts");
  theme_font = PingusResource::load_font("Fonts/pingus_small", "fonts");

  left_arrow  = PingusResource::load_surface("Hotspots/left_arrow", "global");
  right_arrow = PingusResource::load_surface("Hotspots/right_arrow", "global");
  back        = PingusResource::load_surface("buttons/back", "core");

  event = new ThemeSelector::Event;
  event->enabled = false;

  event->theme_selector = this;
  
  //CL_Input::chain_button_press.push_back(event);
  //CL_Input::chain_button_release.push_back(event);
  //CL_Input::chain_mouse_move.push_back(event);

  current_theme = themes.end();

  on_button_press_slot = CL_Input::sig_button_press.connect (CL_CreateSlot(event, &ThemeSelector::Event::on_button_press));
  on_button_release_slot = CL_Input::sig_button_release.connect (CL_CreateSlot(event, &ThemeSelector::Event::on_button_release));
  on_mouse_move_slot = CL_Input::sig_mouse_move.connect (CL_CreateSlot(event, &ThemeSelector::Event::on_mouse_move));
}

ThemeSelector::~ThemeSelector()
{
  // FIXME: Deleting the stuff which was allocated in readdir()
  // FIXME: Should be rewritten
  for(std::vector<Theme*>::iterator i = themes.begin(); i != themes.end(); i++)
    delete (*i);

  // CL_Input::chain_mouse_move.remove(event);
  // CL_Input::chain_button_press.remove(event);
  // CL_Input::chain_button_release.remove(event);
  
  CL_Input::sig_button_press.disconnect (on_button_press_slot);
  CL_Input::sig_button_release.disconnect (on_button_release_slot);
  CL_Input::sig_mouse_move.disconnect (on_mouse_move_slot);
  
  delete event;
}

void
ThemeSelector::display()
{
  event->enabled = true;
  
  while(CL_Mouse::left_pressed() || CL_Mouse::middle_pressed()) 
    CL_System::keep_alive();
  
  if (!dir_read) 
    {
      loading_screen.draw();
      
      readdir(path_manager.complete("themes/"));
      dir_read = true;
    }
  current_theme = themes.end()-1;

#ifdef HAVE_LIBSDL_MIXER
  PingusSound::play_music(path_manager.complete("music/pingus-1.it"));
#endif

  Display::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor", 
						   PingusResource::get("game")));
  Display::show_cursor(true);

  finished = false;
  while(!finished)
    {
      CL_System::keep_alive();
      draw();
      CL_System::sleep (50);
    }

  while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
    CL_System::keep_alive ();

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
    int item_index = themes.size() - std::distance(themes.begin(), current_theme) - 1;
    
    Display::draw_rect(item_index * item_width, CL_Display::get_height() - 15, 
		       (item_index + 1) * item_width, CL_Display::get_height(),
		       0.0, 1.0, 0.0, 1.0);
  }
  
  left_arrow.put_screen(0, (CL_Display::get_height() - left_arrow.get_height()) / 2);
  right_arrow.put_screen(CL_Display::get_width() - right_arrow.get_width(),
			  (CL_Display::get_height() - right_arrow.get_height()) / 2);
  back.put_screen(0, 0);
  if (CL_Mouse::get_x() < (int)back.get_width()
      && CL_Mouse::get_y() < (int)back.get_height())
    CL_Display::fill_rect(0, 0, back.get_width(), back.get_height(),
			  1.0, 1.0, 1.0, 0.3);

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

  // Remove this and pingus_datadir should become a vector
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
      
      std::cout << "ThemeSelector: " << pathname << std::endl;
      dir = System::opendir(pathname, "*.xml");
      
      for(System::Directory::iterator entry = dir.begin(); entry != dir.end(); entry++)
	{
	  if (verbose) std::cout << "ThemeSelelector: Name of entry: " << pathname + entry->name << std::endl;
	  try {
	    themes.push_back(new Theme(pathname + entry->name));
	  } catch (PingusError& err) {
	    std::cout << "ThemeSelector: PingusError: " << err.get_message () << std::endl;
	  }
	}
    }

  if (themes.size() == 0)
    {
      throw PingusError ("ThemeSelector: Couldn't find any themes");
    }
  else
    {
      std::cout << "ThemesSelector: " << themes.size() << " Themes loaded." << std::endl;
    }
}

int
ThemeSelector::mark_level_at_point(int x, int y)
{
  if (current_theme == themes.end())
    return 0;
  else
    return (*current_theme)->mark_level_at_point(x, y);
}

/* EOF */
