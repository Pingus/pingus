//  $Id: playfield.cxx,v 1.8 2002/08/02 11:25:47 grumbel Exp $
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

#include <stdio.h>
#include <ClanLib/Display/Display/display.h>
#include "globals.hxx"
#include "playfield.hxx"
#include "plf.hxx"
#include "pingu_holder.hxx"
#include "world.hxx"
#include "server.hxx"
#include "pingu.hxx"
#include "button_panel.hxx"

using boost::shared_ptr;

Playfield::Playfield(Client* client, PLF* level_data, World* w)
  : current_view(0)
{
  world = w;
  mouse_scrolling = false;
  View::set_world(world);

  // Create a default view
  {
    int x1, x2, y1, y2;

    x1 = (CL_Display::get_width() - world->get_width()) / 2;
    x2 = x1 + world->get_width() - 1;

    y1 = (CL_Display::get_height() - world->get_height()) / 2;
    y2 = y1 + world->get_height() - 1;

    if (x1 < 0)  
      x1 = 0;
    if (x2 >= CL_Display::get_width()) 
      x2 = CL_Display::get_width() - 1;
    if (y1 < 0)
      y1 = 0;
    if (y2 >= CL_Display::get_height())
      y2 = CL_Display::get_height() - 1;

    if (x1 > 0 || x2 < (CL_Display::get_width() - 1)
	|| y1 > 0 || y2 < (CL_Display::get_height() - 1))
      {
	std::cout << "Playfield:: Activating clear screen" << std::endl;
	needs_clear_screen = true;
	generate_clipping_rects(x1, y1, x2, y2);
      }

    if (gimmicks_enabled) 
      {
	if (verbose)
	  std::cout << "Playfield: Using gimmick" << std::endl;
	view.push_back(new View(client, 0, 21, x2/2, y2/2, 0.5f));
	view.push_back(new View(client, x2/2, y1, x2, y2, 1.0f));
	view.push_back(new View(client, 0, y2/2, x2/2, y2, 2.0f));
      } 
    else
      { // !gimmicks_enabled
	view.push_back(new View(client, x1, y1, x2, y2));
	
	view[0]->set_x_offset(((x2 - x1) / 2) - level_data->get_startx());
	view[0]->set_y_offset(((y2 - y1) / 2) - level_data->get_starty());
	
	world->set_view (view[0]);
      }
  }
}

Playfield::~Playfield()
{
  if (verbose)
    std::cout << "Playfield going down" << std::endl;
    
  for (std::vector<View*>::iterator it = view.begin(); it != view.end(); ++it) {
    delete *it;
  }
}

void
Playfield::draw()
{ 
  for(std::vector<View*>::iterator i = view.begin();
      i != view.end(); 
      ++i)
    {
      (*i)->draw();
    }

  if (needs_clear_screen)
    {
      //CL_Display::clear_display();
      for(std::vector<Playfield::Rect>::iterator i = clipping_rectangles.begin();
	  i != clipping_rectangles.end();
	  i++)
	{
	  CL_Display::fill_rect(i->x1, i->y1,
				i->x2 + 1, i->y2 + 1,
				0.0, 0.0, 0.0, 1.0);
	}
    }
}

Pingu*
Playfield::current_pingu_find(int x_pos, int y_pos)
{
  double min_dist = 500.0;
  double dist;
  Pingu* c_pingu = 0;

  for (PinguIter pingu = world->get_pingu_p()->begin(); pingu != world->get_pingu_p()->end(); pingu++)
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
  // FIXME: This should be replaced with something getting relative mouse co's
#if 0
  if (auto_scrolling)
    {
      scroll_speed = 30;

      if (controller->get_x() < 2) 
	{
	  view[current_view]->set_x_offset(view[current_view]->get_x_offset() + scroll_speed);
	} 
      else if (controller->get_x() > CL_Display::get_width() - 3) 
	{
	  view[current_view]->set_x_offset(view[current_view]->get_x_offset() - scroll_speed);
	}
  
      if (controller->get_y() < 2) 
	{
	  view[current_view]->set_y_offset(view[current_view]->get_y_offset() + scroll_speed);
	}
      else if (controller->get_y() > CL_Display::get_height() - 3) 
	{
	  view[current_view]->set_y_offset(view[current_view]->get_y_offset() - scroll_speed);
	}
    }
#endif
}

///
void
Playfield::updateX()
{
  process_input();
  do_scrolling(); 
}

void
Playfield::update(float /*delta*/)
{
#if 0
  for(unsigned int i=0; i < view.size(); ++i)
    {
      if (view[i]->is_current() && !mouse_scrolling)
	{ 
	  current_view = i;
	  current_pingu = current_pingu_find(controller->get_x() - view[i]->get_x_pos() - (view[i]->get_x_offset()),
					     controller->get_y() - view[i]->get_y_pos() - (view[i]->get_y_offset())); 
	  view[i]->set_pingu(current_pingu);
	  break;
	}
    }
#endif 
}

bool 
Playfield::on_button_press(const CL_Key & /*key*/)
{
  if (current_pingu)
    {
      char str[256];
      sprintf(str, "Pingu: %d:%s", current_pingu->get_id(), 
	      action_to_string(buttons->get_action_name()).c_str());
      server->send_event(str);
      return true;
    }
  return false;
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
  if (verbose) std::cout << "Started scrolling..." << std::flush;
  mouse_scrolling = true;

#if 0
  scroll_center_x = controller->get_x();
  scroll_center_y = controller->get_y();
#endif
}  

void
Playfield::do_scrolling()
{
#if 0
  if (mouse_scrolling)
    {
      view[current_view]->shift_x_offset((scroll_center_x - controller->get_x()) / 5);
      view[current_view]->shift_y_offset((scroll_center_y - controller->get_y()) / 5);
    }
#endif 
}

void
Playfield::disable_scroll_mode()
{
  if (verbose) std::cout << "done" << std::endl;
  mouse_scrolling = false;
}

int
Playfield::get_x_offset()
{
  return view[0]->get_x_offset();
}

int
Playfield::get_y_offset()
{
  return view[0]->get_y_offset();  
}

void
Playfield::set_viewpoint(int x, int y)
{
  view[0]->set_x_offset((CL_Display::get_width() / 2) - x);
  view[0]->set_y_offset((CL_Display::get_height() / 2) - y);
}

void 
Playfield::generate_clipping_rects(int x1, int y1, int x2, int y2)
{
  clipping_rectangles.push_back(Playfield::Rect(0, 0, CL_Display::get_width() - 1, y1));
  clipping_rectangles.push_back(Playfield::Rect(0, y1, x1, y2+1));
  clipping_rectangles.push_back(Playfield::Rect(x2+1, y1, CL_Display::get_width() - 1, y2+1));
  clipping_rectangles.push_back(Playfield::Rect(0, y2+1, CL_Display::get_width() - 1, CL_Display::get_height() - 1));
}

void 
Playfield::scroll (int /*x*/, int /*y*/)
{
  
}

/* EOF */

