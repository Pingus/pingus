//  $Id: Story.cc,v 1.1 2000/06/18 22:19:48 grumbel Exp $
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
  small_font = CL_Font::load("Fonts/pingus", PingusResource::get("fonts.dat"));
  large_font = CL_Font::load("Fonts/pingus_small", PingusResource::get("fonts.dat"));
  background = CL_Surface::load("Textures/stones", PingusResource::get("textures.dat"));  

  is_init = true;
}

void 
Story::display()
{
  CL_Surface* story;

  story = CL_Surface::load("Story/story1", PingusResource::get("story.dat"));  

  for(int y = 0; y < CL_Display::get_height(); y += background->get_height())
    for(int x = 0; x < CL_Display::get_width(); x += background->get_width())
      background->put_screen(x, y);

  story->put_screen(CL_Display::get_width()/2 - (story->get_width()/2),
		    80);
  CL_Display::fill_rect(CL_Display::get_width()/2 - (story->get_width()/2) + 25,
			80 - story->get_height()/2 + 25,
			CL_Display::get_width()/2 + (story->get_width()/2) + 25,
			80 + story->get_height()/2 + 25,
			0.0, 0.0, 0.0, 0.8);

  display_string("Dies ist ein Test....\nBla oeuboeu\naoeuoeuaoue");
  CL_Display::flip_display();

  while(true);
}

void
Story::display_string(std::string current_line)
{
  std::string tmp_string;
  std::vector<std::string> output_buffer;
  int current_pos;
  //  std::string current_line = "Dies ist ein Test....\nBla oeuboeu\naoeuoeuaoue"

  // FIXME: This could be optimized if xterm would be a fixed font...
  while(small_font->get_text_width(current_line.c_str()) > (CL_Display::get_width() - 100))
    {
      int pos = current_line.size();
      
      while (small_font->get_text_width(current_line.substr(0, pos).c_str())
	     > (CL_Display::get_width() - 100))
	{
	  pos--;
	}
      
      tmp_string = current_line.substr(0, pos);
      
      output_buffer.push_back(tmp_string);
      //current_pos++;
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
    }
}

/* EOF */
