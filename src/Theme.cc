//  $Id: Theme.cc,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

#include <fstream>

#include "PingusResource.hh"
#include "PingusError.hh"
#include "PingusGame.hh"
#include "PingusMessageBox.hh"
#include "algo.hh"
#include "globals.hh"
#include "Theme.hh"

Theme::Theme()
{
  current_level = 0;
  surface = 0;
  background = 0;
}

Theme::Theme(string filename)
{
  title = CL_Font::load("Fonts/clanfont", PingusResource::get("fonts.dat"));
  font  = CL_Font::load("Fonts/courier_small", PingusResource::get("fonts.dat"));
  
  load(filename);
}

Theme::~Theme()
{
}
  
void
Theme::load(string filename)
{
  cout << "Theme: loading: " << filename << endl;
  plt.parse(filename);
  cout << "Theme: Surface: " << plt.get_surface() << endl;

  levels = plt.get_levels();

  try {
    surface = CL_Surface::load(plt.get_surface().c_str(), PingusResource::get("global.dat"));
  }
  catch (CL_Error err) {
    cout << err.message << endl;
    surface = 0;
  }

  try {
    background = CL_Surface::load(plt.get_background().c_str(), PingusResource::get("global.dat"));
  }
  catch (CL_Error err) {
    cout << err.message << endl;
    background = 0;
  }
  load_status(filename);
}

void
Theme::draw_title()
{
  int x_center =  CL_Display::get_width() / 2;
  int x_pos = x_center;
  int y_pos = 10;

  if (!background) 
    {
      CL_Display::clear_display();
    }
  else 
    {
      // Fill the screen with the background surface
      for(int y=0; y < CL_Display::get_height(); y += background->get_height()) 
	{
	  for(int x=0; x < CL_Display::get_width(); x += background->get_width()) 
	    background->put_screen(x, y);
	}
    }
  
  if (plt.get_name() != "-") 
    {
      title->print_center(x_center, y_pos, plt.get_name().c_str());
      y_pos += 50;
    }

  if (surface) 
    {
      x_pos -= surface->get_width() / 2;
      surface->put_screen(x_pos, y_pos);
      
      y_pos += surface->get_height() + 20;
    }

  font->print_center(x_center, y_pos, plt.get_description().c_str());

  int j = 0;

  for(vector<string>::iterator i = levels.begin(); i < levels.end(); i++) 
    {
      y_pos += 20;
      
      if (j > accessible_levels) 
	{
	  font->print_center(x_center, y_pos, (*i).c_str());
	  CL_Display::fill_rect(x_center - 100, y_pos - 3,
				x_center + 100, y_pos + 13,
				0.0, 0.0, 0.0, 0.5);
	} 
      else if (j == current_level) 
	{
	  CL_Display::fill_rect(x_center - 100, y_pos - 3,
				x_center + 100, y_pos + 13,
				1.0, 1.0, 1.0, 0.9);
	  font->print_center(x_center, y_pos, (*i).c_str());
	} 
      else 
	{
	  font->print_center(x_center, y_pos, (*i).c_str());
	}
      
      j++;
    }

  CL_Display::flip_display();
}

void
Theme::load_status(string name)
{
  status_file = basename(name);
  string rawname  = status_file.substr(0, status_file.rfind("."));
  
  cout << "Filename: " << status_file << endl;
  cout << "Rawfile: " << status_file.substr(0, status_file.rfind(".")) << endl;
  
  status_file = pingus_homedir + "stat/" + rawname + ".pst";
  cout << "Filename to open: " << status_file << endl;

  if (exist(status_file)) {
    ifstream in;
    in.open(status_file.c_str());
    in >> accessible_levels;
    in.close();
  } else {
    cout << "Theme: No Savegame for this theme found" << endl;
    accessible_levels = 0;
  }
  if ((unsigned int)(accessible_levels) >= levels.size()) {
    cout << "Warrning: Accessible_Level is to high! " << accessible_levels << endl;
    accessible_levels = levels.size() - 1;
  }
  current_level = accessible_levels;
}

void
Theme::play()
{
  PingusGame game;
  ofstream out;
      
  try 
    {
      game.start(find_file(pingus_datadir, "levels/" + plt.get_levels()[current_level]));

      if (current_level == accessible_levels)
	++accessible_levels;
            
      if ((unsigned int)(accessible_levels) >= levels.size())
	accessible_levels = levels.size() - 1;
      
      out.open(status_file.c_str());
      out << accessible_levels;
      out.close();
    }

  catch (PingusError err) 
    {
      string str = "Pingus_Error: ";
      str += err.message;
      PingusMessageBox box(str);
    }
}

void
Theme::next_level()
{
  current_level++;

  if ((unsigned int)(current_level) >= levels.size()) 
    current_level = levels.size() - 1;
  
  if (current_level > accessible_levels) 
    current_level  = accessible_levels;
  
  cout << "Level: " << current_level << endl;
}

void
Theme::previous_level()
{
  current_level--;

  if (current_level < 0)
    current_level = 0;
  
  cout << "Level: " << current_level << endl;
}

/* EOF */
