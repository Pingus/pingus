//  $Id: Story.cc,v 1.12 2001/07/27 19:35:36 grumbel Exp $
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

#include <vector>
#include <string>

#include "worldmap/PingusWorldMapManager.hh"
#include "Display.hh"
#include "DeltaManager.hh"
#include "LayerManager.hh"
#include "PingusResource.hh"
#include "PingusMenuManager.hh"
#include "Story.hh"

Story::Story(PingusMenuManager* manager)
  : PingusSubMenu (manager)
{
  is_init = false;
}

Story::~Story()
{
  
}

void
Story::init()
{
  small_font = PingusResource::load_font("Fonts/pingus_small", "fonts");
  large_font = PingusResource::load_font("Fonts/pingus", "fonts");
  background = PingusResource::load_surface("Textures/stones", "textures");  
  story = PingusResource::load_surface("Story/story1", "story");

  is_init = true;
}

void 
Story::update (float delta)
{
}

void 
Story::draw()
{
  CL_Display::fill_rect (0,0,
			 CL_Display::get_width (),
			 CL_Display::get_height (),
			 0,0,0,0.5);

  story.put_screen(CL_Display::get_width()/2 - (story.get_width()/2),
		    50);
  CL_Display::fill_rect(CL_Display::get_width()/2 - (story.get_width()/2) + 25,
			50 + story.get_height() + 25,
			CL_Display::get_width()/2 + (story.get_width()/2) - 25,
			CL_Display::get_height() - 25,
			0.0, 0.0, 0.0, 0.5);

  small_font->print_left(125, 80 + story.get_height() + 25 + (1 * small_font->get_height()), "Insert extremly interesting story here...");
  small_font->print_left(125, 80 + story.get_height() + 25 + (2 * small_font->get_height()), "...");
  small_font->print_left(125, 80 + story.get_height() + 25 + (3 * small_font->get_height()), "...");
  small_font->print_left(125, 80 + story.get_height() + 25 + (4 * small_font->get_height()), "...");
  small_font->print_left(125, 80 + story.get_height() + 25 + (5 * small_font->get_height()), "...");
  small_font->print_left(125, 80 + story.get_height() + 25 + (6 * small_font->get_height()), "...");
}

void
Story::display_string(std::string current_line)
{
  /*  while ((pos = str.find("\n")) != string::npos) 
    {
      tmp_string = str.substr(0, pos);
      small_font->print_left(current_line + tmp_string);
      current_pos++;
      current_line = "";
      str = str.substr(pos+1);
    }
  */
  /*
  std::string tmp_string;
  std::vector<std::string> output_buffer;

  // FIXME: This could be optimized if xterm would be a fixed font...
  while(small_font->get_text_width(current_line.c_str()) > (CL_Display::get_width() - 100))
    {
      int pos;
      for (pos = current_line.size();
	   small_font->get_text_width(current_line.substr(0, pos).c_str())
	     > (CL_Display::get_width() - 100);
	   pos--);
      
      tmp_string = current_line.substr(0, pos);
      
      output_buffer.push_back(tmp_string);
      current_line = current_line.substr(pos);
    }
  
  int x_pos = 100;
  int y_pos = 200;
  int j=0;
  for(std::vector<std::string>::iterator i = output_buffer.begin();
      i != output_buffer.end();
      i++)
    {
      small_font->print_left(x_pos, y_pos + j*small_font->get_height(), 
			     i->c_str());
    }*/
}

void 
Story::on_button_press (CL_InputDevice* device,const CL_Key& key)
{
  manager->disable_events ();
  // FIXME: This looks ugly... 
  PingusWorldMapManager worldmap_manager;
  worldmap_manager.display();
  manager->enable_events ();
  manager->set_menu (&manager->mainmenu);
}

/* EOF */
