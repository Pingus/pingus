//  $Id: action_button.cxx,v 1.13 2002/09/14 19:06:33 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Input/mouse.h>

#include "globals.hxx"
#include "pingus_resource.hxx"
#include "action_button.hxx"
#include "server.hxx"
#include "string_converter.hxx"

using namespace Actions;

ActionButton::ActionButton() {}

ActionButton::~ActionButton() {}

void
ActionButton::init(int x, int y, ActionName name_, int owner_id)
{
  //  make_action = func;
  x_pos = x;
  y_pos = y;
  name = name_;
 
  if (name == Digger || name == Bomber
      || name == Floater || name == Blocker)
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

  font   = PingusResource::load_font("Fonts/pingus_small", "fonts");
  font_h = PingusResource::load_font("Fonts/pingus_small", "fonts");
  font_b = PingusResource::load_font("Fonts/pingus",       "fonts");

  surface = PingusResource::load_surface("Pingus/" + action_to_string(name) + to_string(owner_id), "pingus");
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

ActionName
ActionButton::get_action_name()
{
  return name;
}

void
ActionButton::set_action_holder(ActionHolder* h)
{
  action_holder = h;
}

VerticalActionButton::VerticalActionButton(int x, int y, ActionName name, int owner_id) :
    background (PingusResource::load_surface("buttons/buttonbackground", "core")),
    backgroundhl (PingusResource::load_surface("buttons/buttonbackgroundhl", "core"))
{
  init(x, y, name, owner_id);
}

VerticalActionButton::~VerticalActionButton () {}

bool
VerticalActionButton::is_at (int x, int y)
{
  if (x > x_pos && x < x_pos + 60
      && y > y_pos && y <= y_pos + 35) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

void
VerticalActionButton::draw (GraphicContext& gc)
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
	font_b->print_left (x_pos + 65, y_pos, action_to_string(name).c_str());
  }

  surface.put_screen(x_pos + 20 - surface.get_width ()/2,
		     y_pos + 17 - surface.get_height ()/2,
		     action_c);
  UNUSED_ARG(gc);
}

ArmageddonButton::ArmageddonButton (Server* s, int x, int y)
  : server (s),
    x_pos (x), y_pos (y),
    background  (PingusResource::load_surface("buttons/hbuttonbgb", "core")),
    backgroundhl(PingusResource::load_surface("buttons/hbuttonbg", "core"))
{
  pressed      = false;
  surface      = PingusResource::load_surface("buttons/armageddon_anim", "core");
  counter.set_size(surface.get_num_frames());
  counter = 0;
}

ArmageddonButton::~ArmageddonButton () { }

void
ArmageddonButton::draw (GraphicContext& gc)
{
  if (server->get_armageddon ())
    {
      backgroundhl.put_screen (x_pos, y_pos);
      surface.put_screen(x_pos, y_pos, ++counter);
    } 
  else 
    {
      background.put_screen (x_pos, y_pos);
      surface.put_screen(x_pos, y_pos, 7);
    }
    
  UNUSED_ARG(gc);
}

void
ArmageddonButton::update (float delta)
{
  if (pressed)
    {
      press_time += delta;
      if (press_time > 1.0f)
	{
	  press_time = 0;
	  pressed = false;
	}
    }
  else
    {
      pressed = false;
      press_time = 0;
    }
}

bool
ArmageddonButton::is_at(int x, int y)
{
  if (x > x_pos && x < x_pos + int(surface.get_width())
      && y > y_pos && y < y_pos + int(surface.get_height()))
    {
      return true;
    } else  {
      return false;
    } 
}

void
ArmageddonButton::on_primary_button_click (int x, int y)
{
  if (pressed)
    {
      server->set_armageddon();
    }
  else
    {
      pressed = true;
    }
    
  if(x); if(y);
}

ForwardButton::ForwardButton (Server* s, int x, int y) 
  : server (s),
    x_pos (x), y_pos (y),
    background  (PingusResource::load_surface("buttons/hbuttonbgb", "core")),
    backgroundhl(PingusResource::load_surface("buttons/hbuttonbg", "core"))
{ 
  surface = PingusResource::load_surface("buttons/fast_forward", "core");
}

ForwardButton::~ForwardButton () {}

void
ForwardButton::draw (GraphicContext& gc)
{
  if (server->get_fast_forward())
    {
      backgroundhl.put_screen (x_pos, y_pos);
    }
  else
    {
      background.put_screen (x_pos, y_pos);
    }

  surface.put_screen(x_pos, y_pos);
  UNUSED_ARG(gc);
}

bool
ForwardButton::is_at (int x, int y)
{
  if (x > x_pos && x < x_pos + int(surface.get_width())
      && y > y_pos && y < y_pos + int(surface.get_height()))
    {
      return true;
    } else  {
      return false;
    } 
}

void
ForwardButton::on_primary_button_click (int x, int y)
{
  server->set_fast_forward(!server->get_fast_forward());
  
  if(x); if(y);
}

PauseButton::PauseButton (Server* s, int x, int y) 
  : server (s),
    x_pos(x), y_pos(y),
    background  (PingusResource::load_surface("buttons/hbuttonbgb", "core")),
    backgroundhl(PingusResource::load_surface("buttons/hbuttonbg", "core"))
{ 
  surface = PingusResource::load_surface("buttons/pause", "core");
}

PauseButton::~PauseButton () {}

void
PauseButton::draw (GraphicContext& gc)
{
  if (server->get_pause()) 
    backgroundhl.put_screen (x_pos, y_pos);
  else
    background.put_screen (x_pos, y_pos);
  
  surface.put_screen(x_pos, y_pos);
  UNUSED_ARG(gc);
}

bool
PauseButton::is_at (int x, int y)
{
  if (x > x_pos && x < x_pos + int(surface.get_width())
      && y > y_pos && y < y_pos + int(surface.get_height()))
    {
      return true;
    } else  {
      return false;
    } 
}

void
PauseButton::on_primary_button_click (int x, int y)
{
  server->set_pause(!server->get_pause ());
  
  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

/* EOF */
