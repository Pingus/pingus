//  $Id: Playfield.cc,v 1.3 2000/02/11 16:58:26 grumbel Exp $
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
#include <cstdio>
#include <cassert>

#include "globals.hh"
#include "Pingu.hh"
#include "Playfield.hh"
#include "PLF.hh"
#include "Result.hh"
#include "LevelInterrupt.hh"
#include "CaptureRectangle.hh"
#include "GameTime.hh"
#include "QuestionDialog.hh"

Playfield::Playfield()
{
  current_pingu = 0;
  //  x_offset = 0;
  //y_offset = 0;
  server = 0;
  client = 0;
  //  pause = false;
  mouse_scrolling = false;
}

Playfield::Playfield(PLF* level_data, World* w)
{
  world = w;
  current_pingu = 0;
  current_view = 0;
  pingus = world->get_pingu_p();

  //x_offset = 0; 
  //y_offset = 0; 

  //  x_offset2 = x_offset;
  // y_offset2 = y_offset;

  View::set_world(world);

  // Create a default view
  {
    int x1, x2, y1, y2;

    x1 = (CL_Display::get_width() - world->get_width()) / 2;
    x2 = x1 + world->get_width();

    y1 = (CL_Display::get_height() - world->get_height()) / 2;
    y2 = y1 + world->get_height();

    if (x1 < 0)  
      x1 = 0;
    if (x2 > CL_Display::get_width() - 1) 
      x2 =  CL_Display::get_width() - 1;
    if (y1 < 0)
      y1 = 0;
    if (y2 >  CL_Display::get_height())
      y2 =  CL_Display::get_height();

    if (verbose) 
      {
	cout << "Playfield: Using screen size: " 
	     << x1 << ", "
	     << y1 << ", " 
	     << x2 << ", "
	     << y2 << std::endl;
      }
    
    if (gimmicks_enabled) 
      {
	if (verbose)
	  std::cout << "Playfield: Using gimmick" << std::endl;
	view.push_back(View(x2/2, y1, x2, y2));
	view.push_back(View(0, 21, x2/2, y2/2, 0.5));
	view.push_back(View(0, y2/2, x2/2, y2, 2.0));
      } 
    else
      { // !gimmicks_enabled
	view.push_back(View(x1, y1, x2, y2));
	
	cout << "Playfield: start_X: " << ((x2 - x1) / 2) - level_data->get_startx() << std::endl;
	cout << "Playfield: start_Y: " << ((y2 - y1) / 2) - level_data->get_starty() << std::endl;
	
	view[0].set_x_offset(((x2 - x1) / 2) - level_data->get_startx());
	view[0].set_y_offset(((y2 - y1) / 2) - level_data->get_starty());
      }
  }
}

Playfield::~Playfield()
{
  if (verbose)
    std::cout << "Playfield going down" << std::endl;
}

void
Playfield::draw()
{ 
  for(unsigned int i=0; i < view.size(); ++i) {
    view[i].draw();
  }
}

Pingu*
Playfield::current_pingu_find(int x_pos, int y_pos)
{
  double min_dist = 500.0;
  double dist;
  Pingu* c_pingu = 0;

  for (PinguIter pingu=pingus->begin(); pingu != pingus->end(); pingu++)
    {
      if ((*pingu)->is_over(x_pos, y_pos))
	{
	  dist = (*pingu)->dist(x_pos, y_pos);
	    
	  if (dist < min_dist) 
	    {
	      min_dist = dist;
	      c_pingu = *pingu;
	    }
	}
    }
  return c_pingu; 
}

void
Playfield::set_world(World* w)
{
  world = w;
}

void 
Playfield::process_input()
{
  process_input_interactive();
}

void
Playfield::process_input_demomode()
{
}

void 
Playfield::process_input_interactive()
{

  // Scroll around if the mouse button is pressed
  /*  if (CL_Mouse::right_pressed())
    {
      if (!mouse_scrolling) 
	{
	  char str[1024];

	  mouse_scrolling = true;
	  scroll_center_x = CL_Mouse::get_x();
	  scroll_center_y = CL_Mouse::get_y();
	  x_offset = view[current_view].get_x_offset();
	  y_offset = view[current_view].get_y_offset();

	  sprintf(str, "Playfield: Start scrolling: X: %d Y: %d", x_offset, y_offset);
	  //      event.send(str);
	}
      x_offset += (scroll_center_x - CL_Mouse::get_x()) / 5;
      y_offset += (scroll_center_y - CL_Mouse::get_y()) / 5;
    
      view[current_view].set_x_offset(x_offset);
      view[current_view].set_y_offset(y_offset);
    }
  else 
    {
      if (mouse_scrolling) 
	{
	  char str[1024];
	  sprintf(str, "Playfield: Stop scrolling: X: %d Y: %d", x_offset, y_offset);
	  //      event.send(str);
	}
      mouse_scrolling = false;
    }
*/
  // FIXME: This should be replaced with something getting relative mouse co's
  if (auto_scrolling)
    {
      if (CL_Mouse::get_x() < 2) 
	{
	  view[current_view].set_x_offset(view[current_view].get_x_offset() + scroll_speed);
	} 
      else if (CL_Mouse::get_x() > CL_Display::get_width() - 3) 
	{
	  view[current_view].set_x_offset(view[current_view].get_x_offset() - scroll_speed);
	}
  
      if (CL_Mouse::get_y() < 2) 
	{
	  view[current_view].set_y_offset(view[current_view].get_y_offset() + scroll_speed);
	}
      else if (CL_Mouse::get_y() > CL_Display::get_height() - 3) 
	{
	  view[current_view].set_y_offset(view[current_view].get_y_offset() - scroll_speed);
	}
    }
  /*
  if (CL_Mouse::middle_pressed()) 
    {
      std::cout << "Mouse Coordinates ="
	   << " X: " << CL_Mouse::get_x() + x_offset
	   << " Y: " << CL_Mouse::get_y() + y_offset << std::endl;
      world->print_status();
    }*/
}

void
Playfield::let_move()
{
  process_input();
  do_scrolling();
  for(unsigned int i=0; i < view.size(); ++i)
    {
      if (view[i].is_current() && !mouse_scrolling)
	{ 
	  current_view = i;
	  current_pingu = current_pingu_find(CL_Mouse::get_x() - view[i].get_x_pos() - (view[i].get_x_offset()),
					     CL_Mouse::get_y() - view[i].get_y_pos() - (view[i].get_y_offset())); 
	  view[i].set_pingu(current_pingu);
	  break;
	}
    }
  
  if (current_pingu && CL_Mouse::left_pressed())
    {
      char str[256];
      sprintf(str, "Pingu: %d:%s", current_pingu->get_id(), buttons->get_action_name().c_str());
      server->send_event(str);
    }
}

void
Playfield::set_buttons(ButtonPanel* b)
{
  buttons = b;
}

void 
Playfield::set_pingu_info(PinguInfo* p)
{
  pingu_info = p;
}

void
Playfield::set_server(Server* s)
{
  server = s;
}

void
Playfield::set_client(Client* c)
{
  client = c;
}

void
Playfield::enable_scroll_mode()
{
  std::cout << "Started scrolling..." << std::flush;
  mouse_scrolling = true;

  scroll_center_x = CL_Mouse::get_x();
  scroll_center_y = CL_Mouse::get_y();
  
  //x_offset = view[current_view].get_x_offset();
  //  y_offset = view[current_view].get_y_offset(); 
}  

void
Playfield::do_scrolling()
{
  if (mouse_scrolling)
    {
      view[current_view].shift_x_offset((scroll_center_x - CL_Mouse::get_x()) / 5);
      view[current_view].shift_y_offset((scroll_center_y - CL_Mouse::get_y()) / 5);
    }
}

void
Playfield::disable_scroll_mode()
{
  std::cout << "done" << std::endl;
  mouse_scrolling = false;
}

/* EOF */

