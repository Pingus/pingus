//  $Id: ThemeSelector.cc,v 1.2 2000/02/09 21:43:41 grumbel Exp $
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

#include "globals.hh"
#include "System.hh"
#include "PingusError.hh"
#include "PingusResource.hh"
#include "AlphaButton.hh"

#include "ThemeSelector.hh"

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
ListBox::add_item(string l)
{
  items.push_back(ListItem(l));
}

// ---=== List item ===---

ListItem::ListItem(string l)
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

ThemeSelector::ThemeSelector()
{
  dir_read = false;
  title_font = CL_Font::load("Fonts/pingus", PingusResource::get("fonts.dat"));
  theme_font = CL_Font::load("Fonts/smallfont_h", PingusResource::get("fonts.dat"));
}

ThemeSelector::~ThemeSelector()
{
}

void
ThemeSelector::select()
{
  vector<Theme*>::iterator temp_theme = 0;
  
  while(CL_Mouse::left_pressed() || CL_Mouse::middle_pressed()) 
    CL_System::keep_alive();
  
  if (!dir_read) 
    {
      readdir(pingus_datadir);
      dir_read = true;
    }
  current_theme = themes.begin();

  (*current_theme)->draw_title();

  while(!key_pressed(CL_KEY_ESCAPE)) 
    {
      CL_System::keep_alive();
      
      if (key_pressed(CL_KEY_LEFT)) 
	{
	  current_theme++;
	  if (current_theme ==  themes.end()) 
	    {
	      current_theme = themes.begin();
	    }
	} 
      else if (key_pressed(CL_KEY_RIGHT)) 
	{
	  if (current_theme == themes.begin()) 
	    {
	      current_theme = themes.end();
	    }
	  current_theme--;
	}
      else if (key_pressed(CL_KEY_ENTER)) 
	{
	  (*current_theme)->play();
	  (*current_theme)->draw_title();      
	}
      else if (key_pressed(CL_KEY_DOWN)) 
	{
	  (*current_theme)->next_level();
	  draw();
	}
      else if (key_pressed(CL_KEY_UP)) 
	{
	  (*current_theme)->previous_level();
	  draw();
	}
      
      if (temp_theme != current_theme) 
	{
	  (*current_theme)->draw_title();
	  temp_theme = current_theme;
	}
    }
}

void
ThemeSelector::draw()
{
  CL_Display::fill_rect(0,0,640,480,0.5,0.0,0.0,0.5);

  //title_font->print_center(CL_Display::get_width()/2, 30, "Theme Selector");

  (*current_theme)->draw_title();
  
  theme_font->print_left(0, CL_Display::get_height() / 2, "Previous");
  theme_font->print_right(CL_Display::get_width(), CL_Display::get_height() / 2, "Next");
  //CL_Display::flip_display();
}

void
ThemeSelector::readdir(string path)
{
  System::Directory dir; 
  string pathname;
  string::size_type pos = 0; 
  string::size_type last_pos = 0; 
  bool exit_for = false;

  for(pos = path.find(":", last_pos);
      !exit_for;
      last_pos = pos + 1, pos = path.find(":", last_pos))
    {
      if (pos == string::npos) 
	{
	  pathname = path.substr(last_pos, string::npos);
	  exit_for = true;
	} 
      else 
	{
	  pathname = path.substr(last_pos, pos - last_pos);
	}
      
      dir = System::opendir(pathname + "/themes", "*.plt");
      
      for(System::Directory::iterator entry = dir.begin(); entry != dir.end(); entry++)
	{
	  cout << "Entry Name: " << pathname + "/themes/" + entry->name << endl;
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
