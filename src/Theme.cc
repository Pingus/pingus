//  $Id: Theme.cc,v 1.17 2000/07/30 01:47:35 grumbel Exp $
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

#include "System.hh"
#include "PingusResource.hh"
#include "PingusError.hh"
#include "PingusGame.hh"
#include "PingusMessageBox.hh"
#include "algo.hh"
#include "globals.hh"
#include "Theme.hh"
#include "PLFPLF.hh"
#include "XMLPLF.hh"

Theme::Theme()
{
  current_level = 0;
  surface = 0;
  background = 0;
}

Theme::Theme(std::string filename)
{
  title = PingusResource::load_font("Fonts/pingus","fonts");
  font  = PingusResource::load_font("Fonts/pingus_small","fonts");
  
  load(filename);
}

Theme::~Theme()
{
}
  
void
Theme::load(std::string filename)
{
  if (verbose) std::cout << "Theme: loading: " << filename << std::endl;
  plt.parse(filename);
  if (verbose > 1) std::cout << "Theme: Surface: " << plt.get_surface() << std::endl;

  levels = plt.get_levels();
  load_levels();

  if (plt.get_description() != "-")
    {
      description.set_font(font);
      description.set_text(plt.get_description(), 350);
      has_description = true;
    }
  else
    {
      has_description = false;
    }

  try 
    {
      if (plt.get_surface() != "-")
	surface = PingusResource::load_surface(plt.get_surface(), "global");
      else
	surface = 0;
    }

  catch (CL_Error err) 
    {
      if (verbose) std::cout << "Theme:filename:" << err.message << std::endl;
      surface = 0;
    }

  try 
    {
      if (plt.get_background().res_name != "-")
	background = PingusResource::load_surface(plt.get_background());
      else
	background = 0;
    }
  catch (CL_Error err) 
    {
      if (verbose) 
	{
	  std::cout << "Theme:" << filename  << ":" << err.message << std::endl;
	  std::cout << "Theme: Ignoring missing resource, disable background." << std::endl;
	}
      background = 0;
    }
  load_status(filename);
}

int
Theme::mark_level_at_point(int x, int y)
{
  int j = 0;
  int y_pos = level_start_y_pos;

  for(std::vector<std::string>::iterator i = levelnames.begin();
      i < levelnames.end(); 
      i++, j++)
    {
      int width = font->get_text_width(i->c_str());
      
      if ((CL_Display::get_width()/2 - width/2) < x
	  && (CL_Display::get_width()/2 + width/2) > x
	  && y_pos < y
	  && (y_pos + font->get_height()) > y)
	{
	  if (j <= accessible_levels)
	    {
	      current_level = j;
	      // std::cout << "Current_level: " << current_level << std::endl;
	      return current_level;
	    }
	  return -1;
	}      
      y_pos += font->get_height() + 4;
    }
  return -1;
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

  y_pos += 15;
  if (has_description)
    {
      CL_Display::fill_rect(x_center - (description.get_width()/2) - 15,
			    y_pos - 15,
			    x_center + (description.get_width()/2) + 15,
			    y_pos + description.get_height() + 15,
			    1.0, 1.0, 1.0, 0.5);

      description.print_center(x_center, y_pos);
    }

  y_pos += description.get_height() + 15 + 20;
  int j = 0;
  
  level_start_y_pos = y_pos;

  for(std::vector<std::string>::iterator i = levelnames.begin(); i < levelnames.end(); i++) 
    {
      if (j > accessible_levels) 
	{
	  font->print_center(x_center, y_pos, (*i).c_str());
	  CL_Display::fill_rect(x_center - font->get_text_width(i->c_str())/2 - 1,
				y_pos - 1,
				x_center + font->get_text_width(i->c_str())/2 + 1, 
				y_pos + font->get_height() + 1,
				0.0, 0.0, 0.0, 0.5);
	} 
      else if (j == current_level) 
	{
	  CL_Display::fill_rect(x_center - font->get_text_width(i->c_str())/2 - 1,
				y_pos - 1,
				x_center + font->get_text_width(i->c_str())/2 + 1, 
				y_pos + font->get_height() + 1,
				1.0, 1.0, 1.0, 0.9);
	  font->print_center(x_center, y_pos, (*i).c_str());
	} 
      else 
	{
	  font->print_center(x_center, y_pos, (*i).c_str());
	}
      j++;
      y_pos += font->get_height() + 4;     
    }
}

void
Theme::load_status(std::string name)
{
  status_file = System::basename(name);
  std::string rawname  = status_file.substr(0, status_file.rfind("."));
  
  if (verbose > 1) std::cout << "Filename: " << status_file << std::endl;
  if (verbose > 1) std::cout << "Rawfile: " << status_file.substr(0, status_file.rfind(".")) << std::endl;
  
  status_file = System::get_statdir() + "stat/" + rawname + ".pst";
  
  if (verbose > 1) std::cout << "Filename to open: " << status_file << std::endl;

  if (System::exist(status_file)) 
    {
      std::ifstream in;
      in.open(status_file.c_str());
      in >> accessible_levels;
      in.close();
    } 
  else 
    {
      if (verbose) std::cout << "Theme: No Savegame for this theme found" << std::endl;
      accessible_levels = 0;
    }

  if ((unsigned int)(accessible_levels) >= levels.size()) 
    {
      if (verbose) std::cout << "Warrning: Accessible_Level is to high! " << accessible_levels << std::endl;
      accessible_levels = levels.size() - 1;
    }
  current_level = accessible_levels;
}

void
Theme::play()
{
  PingusGame game;
  std::ofstream out;
      
  try 
    {
      game.start_game(find_file(pingus_datadir, "levels/" + plt.get_levels()[current_level]));

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
      std::string str = "Pingus_Error: ";
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
  
  //std::cout << "Level: " << current_level << std::endl;
}

void
Theme::previous_level()
{
  current_level--;

  if (current_level < 0)
    current_level = 0;
  
  //  std::cout << "Level: " << current_level << std::endl;
}

void
Theme::load_levels()
{
  std::string filename;
  
  if (verbose) std::cout << "Theme opening levels... " << std::flush;

  for(std::vector<std::string>::iterator i = levels.begin(); i < levels.end(); i++)
    {
      filename = find_file(pingus_datadir, "levels/" + *i);

      try
	{
	  if (filename.substr(filename.size() - 4) == ".plf")
	    {
	      PLFPLF plf(filename);
	      levelnames.push_back(plf.get_levelname()[System::get_language()]);
	    }
	  else
	    {
	      XMLPLF plf(filename);
	      levelnames.push_back(plf.get_levelname()[System::get_language()]);
	    }
	}
      catch (PingusError err) 
	{
	  std::string str = "PingusError: ";
	  str += err.message;
	  std::cout << err.message << std::endl;
	}
    }
  if (verbose) std::cout << "done." << std::endl;
}

/* EOF */
