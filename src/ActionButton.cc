//  $Id: ActionButton.cc,v 1.29 2002/06/01 18:05:34 torangan Exp $
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

#include <cstdio>

#include "algo.hh"
#include "globals.hh"
#include "PingusResource.hh"
#include "ActionButton.hh"

Button::Button()
{
}

Button::~Button()
{
}

EventButton::EventButton(int x, int y, std::string str)
{
  surface = PingusResource::load_surface(str, "global");
  x_pos = x;
  y_pos = y;
}

EventButton::~EventButton() {}

void
EventButton::draw()
{
  if (mouse_over(CL_Vector() /* FIXME */))
    {
      CL_Display::fill_rect(x_pos, y_pos, 
			    x_pos + surface.get_width(), y_pos + surface.get_height(),
			    1.0, 1.0, 1.0, 1.0);
    }
  surface.put_screen(x_pos, y_pos);
}

bool
EventButton::mouse_over(const CL_Vector& pos)
{
  if (pos.x > x_pos && pos.x < x_pos + int(surface.get_width())
      && pos.y > y_pos && pos.y < y_pos + int(surface.get_height()))
    {
      return true;
    }
  else 
    {
      return false;
    }
}

ActionButton::ActionButton() {}
ActionButton::~ActionButton() {}

void
ActionButton::init(int x, int y, std::string str, int owner_id)
{
  //  make_action = func;
  x_pos = x;
  y_pos = y;
  name = str;
 
  if (name == "digger" || name == "bomber" 
      || name == "floater" || name == "blocker")
    {
      is_multi_direct = false;
    }
  else
    {
      is_multi_direct = true;
    }

  /*
  font = PingusResource::load_font("Fonts/courier_small", "fonts");
  font_h = PingusResource::load_font("Fonts/smallfont","fonts");
  font_b = PingusResource::load_font("Fonts/pingus","fonts");
  */

  font = PingusResource::load_font("Fonts/pingus_small", "fonts");
  font_h = PingusResource::load_font("Fonts/pingus_small","fonts");
  font_b = PingusResource::load_font("Fonts/pingus","fonts");

  if (str != "empty") 
    {
      surface   = PingusResource::load_surface("Pingus/" + str + to_string(owner_id),
					       "pingus");
      if (is_multi_direct)
	{
	  action_c.set_size(surface.get_num_frames()/2);
	}
      else
	{
	  action_c.set_size(surface.get_num_frames());
	}

      action_c.set_speed(50);
    }
}

bool
ActionButton::is_pressed()
{
  return false;
}

void
ActionButton::update(float /*delta*/)
{
  ++action_c;
}

std::string
ActionButton::get_action_name()
{
  return name;
}

void
ActionButton::set_action_holder(ActionHolder* h)
{
  action_holder = h;
}

VerticalActionButton::VerticalActionButton(int x, int y, std::string str, int owner_id)
  : background (PingusResource::load_surface("buttons/buttonbackground", "core")),
    backgroundhl (PingusResource::load_surface("buttons/buttonbackgroundhl", "core"))
{
  init(x, y, str, owner_id);
}

VerticalActionButton::~VerticalActionButton() {}

bool
VerticalActionButton::mouse_over(const CL_Vector& pos)
{
  if (pos.x > x_pos && pos.x < x_pos + 60
      && pos.y > y_pos && pos.y <= y_pos + 35) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

void
VerticalActionButton::draw()
{
  std::string str;
  // FIXME: This could need some optimization, throwing strings all
  // FIXME: around, doesn't look like a good idea. 
  available = action_holder->get_available(name);

  if (unlimited_actions) {
    str = "";
  } else {
    str = to_string(available);
  }

  if (pressed) 
    {
      if (fast_mode) {
	CL_Display::fill_rect(x_pos, y_pos, x_pos + 60, y_pos + 35 ,
			      1.0, 1.0, 1.0, 1.0);
      } else {
	//CL_Display::fill_rect(x_pos, y_pos, x_pos + 60, y_pos + 35 ,
	//1.0, 1.0, 1.0, 0.5);
	backgroundhl.put_screen (x_pos, y_pos);
      }
      font_h->print_center(x_pos + 46, y_pos + 10, str.c_str ());
    }
  else
    {
      action_c = 0;

      if (fast_mode) {
	// do nothing
      } else {
	background.put_screen (x_pos, y_pos);
      }
      font->print_center(x_pos + 46, y_pos + 10, str.c_str ());
    }

  // print the action name next to the button, when mouse pointer is on
  // the button.
  if(action_help 
     && CL_Mouse::get_x() > x_pos && CL_Mouse::get_x() < x_pos + 60
     && CL_Mouse::get_y() < y_pos + 35 && CL_Mouse::get_y() > y_pos) 
  {
	font_b->print_left (x_pos + 65, y_pos, name.c_str());
  }

  surface.put_screen(x_pos + 20 - surface.get_width ()/2,
		     y_pos + 17 - surface.get_height ()/2,
		     action_c);
}

ArmageddonButton::ArmageddonButton(int x, int y)
{
  surface = PingusResource::load_surface("buttons/armageddon_anim", "core");

  background = PingusResource::load_surface("buttons/hbuttonbgb", "core");
  backgroundhl = PingusResource::load_surface("buttons/hbuttonbg", "core");

  counter.set_size(surface.get_num_frames());
  counter = 0;
  x_pos = x;
  y_pos = y;
  pressed = false;
}

ArmageddonButton::~ArmageddonButton() {}

void
ArmageddonButton::draw()
{
  if (pressed)
    {
      backgroundhl.put_screen (x_pos, y_pos);
      surface.put_screen(x_pos, y_pos, ++counter);
    } 
  else 
    {
      background.put_screen (x_pos, y_pos);
      surface.put_screen(x_pos, y_pos, 7);
    }
}

bool
ArmageddonButton::mouse_over(const CL_Vector& pos)
{
  if (pos.x > x_pos && pos.x < x_pos + int(surface.get_width())
      && pos.y > y_pos && pos.y < y_pos + int(surface.get_height()))
    {
      return true;
    } else  {
      return false;
    } 
}

ForwardButton::ForwardButton(int x, int y)
{
  surface = PingusResource::load_surface("buttons/fast_forward", "core");
  background = PingusResource::load_surface("buttons/hbuttonbgb", "core");
  backgroundhl = PingusResource::load_surface("buttons/hbuttonbg", "core");

  x_pos = x;
  y_pos = y;
}

ForwardButton::~ForwardButton() {}

void
ForwardButton::draw()
{
  if (server->get_fast_forward())
    backgroundhl.put_screen (x_pos, y_pos);
  else
    background.put_screen (x_pos, y_pos);
  
  surface.put_screen(x_pos, y_pos);
}

bool
ForwardButton::mouse_over(const CL_Vector& pos)
{
  if (pos.x > x_pos && pos.x < x_pos + int(surface.get_width())
      && pos.y > y_pos && pos.y < y_pos + int(surface.get_height()))
    {
      return true;
    } else  {
      return false;
    } 
}

PauseButton::PauseButton(int x, int y)
{
  surface = PingusResource::load_surface("buttons/pause", "core");

  background = PingusResource::load_surface("buttons/hbuttonbgb", "core");
  backgroundhl = PingusResource::load_surface("buttons/hbuttonbg", "core");

  x_pos = x;
  y_pos = y;
}

PauseButton::~PauseButton() {}

void
PauseButton::draw()
{
  if (server->get_pause()) 
    backgroundhl.put_screen (x_pos, y_pos);
  else
    background.put_screen (x_pos, y_pos);
  
  surface.put_screen(x_pos, y_pos);
}

bool
PauseButton::mouse_over(const CL_Vector& pos)
{
  if (pos.x > x_pos && pos.x < x_pos + int(surface.get_width())
      && pos.y > y_pos && pos.y < y_pos + int(surface.get_height()))
    {
      return true;
    } else  {
      return false;
    } 
}

/* EOF */
