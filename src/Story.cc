//  $Id: Story.cc,v 1.5 2000/10/18 20:16:36 grumbel Exp $
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

#include "LayerManager.hh"
#include "PingusResource.hh"
#include "Story.hh"

Story pingus_story;

Story::Story()
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

  is_init = true;
}

void 
Story::display()
{
  LayerManager layer_manager;
  CL_Surface* sur = PingusResource::load_surface ("NewButtons/create_on", "menu");

  layer_manager.add_layer (PingusResource::load_surface ("Layer/layer1", "story"),  0, 0, 2, 0);
  layer_manager.add_layer (PingusResource::load_surface ("Layer/layer2", "story"),  0, 150, 5, 0);
  layer_manager.add_layer (PingusResource::load_surface ("Layer/layer3", "story"), 0, 200, 10, 0);
  layer_manager.add_layer (PingusResource::load_surface ("Layer/layer4", "story"), 0, 377, 25, 0);
  layer_manager.add_layer (PingusResource::load_surface ("Layer/layer5", "story"), 0, 500, 35, 0);

  while (true)
    {
      layer_manager.update ();
      layer_manager.draw ();
      
      sur->put_screen (400, 300);

      CL_System::keep_alive ();
      CL_Display::flip_display ();
    }

  /*
  CL_Surface* story;

  story = PingusResource::load_surface("Story/story1", "story");

  for(int y = 0; y < CL_Display::get_height(); y += background->get_height())
    for(int x = 0; x < CL_Display::get_width(); x += background->get_width())
      background->put_screen(x, y);

  story->put_screen(CL_Display::get_width()/2 - (story->get_width()/2),
		    50);
  CL_Display::fill_rect(CL_Display::get_width()/2 - (story->get_width()/2) + 25,
			50 + story->get_height() + 25,
			CL_Display::get_width()/2 + (story->get_width()/2) - 25,
			CL_Display::get_height() - 25,
			1.0, 1.0, 1.0, 0.5);

  //display_string("Dies ist ein Test....\nBla oeuboeu\naoeuoeuaoue");
  small_font->print_left(105, 80 + story->get_height() + 25 + (1 * small_font->get_height()), "aoeuoaeu");
  small_font->print_left(105, 80 + story->get_height() + 25 + (2 * small_font->get_height()), "Bloeuoeua");
  small_font->print_left(105, 80 + story->get_height() + 25 + (3 * small_font->get_height()), "Bloeuoea");
  small_font->print_left(105, 80 + story->get_height() + 25 + (4 * small_font->get_height()), "Blaoeuoea");
  small_font->print_left(105, 80 + story->get_height() + 25 + (5 * small_font->get_height()), "oeuBla");
  small_font->print_left(105, 80 + story->get_height() + 25 + (6 * small_font->get_height()), "Beouoeula");

  CL_Display::flip_display();

  while(true);
*/
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

/* EOF */
