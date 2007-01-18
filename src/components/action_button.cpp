//  $Id: action_button.cxx,v 1.35 2003/10/21 11:01:52 grumbel Exp $
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

#include "../globals.hpp"
#include "../cheat.hpp"
#include "../resource.hpp"
#include "action_button.hpp"
#include "../true_server.hpp"
#include "../world.hpp"
#include "../display/drawing_context.hpp"
#include "../gui/display.hpp"
#include "../fonts.hpp"
#include "../string_util.hpp"
#include "../math/vector3f.hpp"


using namespace Actions;

ActionButton::ActionButton(ActionHolder* h)
: action_holder(h){}

ActionButton::~ActionButton() {}

void
ActionButton::init(int x, int y, ActionName name_, int owner_id)
{
  //  make_action = func;
  x_pos = x;
  y_pos = y;
  name = name_;

  font   = Fonts::pingus_small;
  font_b = Fonts::pingus_large;

  sprite = Resource::load_sprite("pingus/player0/" + action_to_string(name) + "/right");
}

bool
ActionButton::is_pressed()
{
  return false;
}

void
ActionButton::update(float delta)
{
  sprite.update(delta);
}

ActionName
ActionButton::get_action_name()
{
  return name;
}

VerticalActionButton::VerticalActionButton(ActionHolder* h,
                                           int x, int y, ActionName name_, int owner_id)
  : ActionButton(h),
    background (Resource::load_sprite("core/buttons/buttonbackground")),
    backgroundhl (Resource::load_sprite("core/buttons/buttonbackgroundhl"))
{
  init(x, y, name_, owner_id);
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
VerticalActionButton::draw (DrawingContext& gc)
{
  if (pressed)
    {
      if (fast_mode) 
        {
          gc.draw_fillrect((float)x_pos, (float)y_pos, (float)x_pos + 60, (float)y_pos + 35,
                           Color(255, 255, 255));
        } 
      else 
        {
          //CL_Display::fill_rect(x_pos, y_pos, x_pos + 60, y_pos + 35 ,
          //1.0, 1.0, 1.0, 0.5);
          gc.draw(backgroundhl, (float)x_pos, (float)y_pos);
        }
    }
  else
    {
      sprite.set_frame(0);

      if (fast_mode) {
        // do nothing
      } else {
        gc.draw(background, (float)x_pos, (float)y_pos);
      }
    }

  gc.draw(sprite, Vector3f((float)x_pos + 20, (float)y_pos + 32));

  Font myfont  = font;
  Font myfontb = font_b;

  // print the action name next to the button, when mouse pointer is on
  // the button.
  // FIXME: this should use the GUI events, not CL_Mouse
#if 0
  if (action_help
      && CL_Mouse::get_x() > x_pos      && CL_Mouse::get_x() < x_pos + 60
      && CL_Mouse::get_y() < y_pos + 35 && CL_Mouse::get_y() > y_pos)
    {
      gc.print_left(myfontb, (float)x_pos + 65, (float)y_pos, action_to_screenname(name));
    }
#endif

  if (Cheat::unlimited_actions)
    {
      // FIXME: insert unlimited surface here
      gc.print_center(myfont, (float)x_pos + 46, (float)y_pos + 5, "oo");
    }
  else
    {
      std::string str = StringUtil::to_string(action_holder->get_available(name));
      gc.print_center(myfont, (float)x_pos + 46, (float)y_pos + 5, str);
    }
}

ArmageddonButton::ArmageddonButton (TrueServer* s, int x, int y)
  : server (s),
    x_pos (x),
    y_pos (y),
    background  (Resource::load_sprite("core/buttons/hbuttonbgb")),
    backgroundhl(Resource::load_sprite("core/buttons/hbuttonbg"))
{
  pressed      = false;
  sprite       = Resource::load_sprite("core/buttons/armageddon_anim");
}

ArmageddonButton::~ArmageddonButton () { }

void
ArmageddonButton::draw (DrawingContext& gc)
{
  if (server->get_world()->check_armageddon ())
    {
      gc.draw(backgroundhl, Vector3f((float)x_pos, (float)y_pos));
      gc.draw(sprite, Vector3f((float)x_pos, (float)y_pos));
    }
  else
    {
      if (!fast_mode)
        gc.draw(background, Vector3f((float)x_pos, (float)y_pos));

      sprite.set_frame(7);
      gc.draw(sprite, Vector3f((float)x_pos, (float)y_pos));
    }
}

void
ArmageddonButton::update (float delta)
{
  sprite.update(delta);

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
  if (x > x_pos && x < x_pos + sprite.get_width()
      && y > y_pos && y < y_pos + sprite.get_height())
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
      server->send_armageddon_event();
    }
  else
    {
      pressed = true;
    }

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

ForwardButton::ForwardButton (TrueServer* s, int x, int y)
  : server (s),
    x_pos (x), y_pos (y),
    background  (Resource::load_sprite("core/buttons/hbuttonbgb")),
    backgroundhl(Resource::load_sprite("core/buttons/hbuttonbg"))
{
  surface = Resource::load_sprite("core/buttons/fast_forward");
}

ForwardButton::~ForwardButton () {}

void
ForwardButton::draw (DrawingContext& gc)
{
  if (server->get_fast_forward())
    {
      gc.draw(backgroundhl, Vector3f((float)x_pos, (float)y_pos));
    }
  else
    {
      if (!fast_mode)
        gc.draw(background, Vector3f((float)x_pos, (float)y_pos));
    }

  gc.draw(surface, Vector3f((float)x_pos, (float)y_pos));
}

bool
ForwardButton::is_at (int x, int y)
{
  if (   x > x_pos && x < x_pos + int(surface.get_width())
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

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

PauseButton::PauseButton (TrueServer* s, int x, int y)
  : server (s),
    x_pos(x), y_pos(y),
    background  (Resource::load_sprite("core/buttons/hbuttonbgb")),
    backgroundhl(Resource::load_sprite("core/buttons/hbuttonbg"))
{
  surface = Resource::load_sprite("core/buttons/pause");
}

PauseButton::~PauseButton () {}

void
PauseButton::draw (DrawingContext& gc)
{
  if (server->get_pause())
    {
      gc.draw(backgroundhl, Vector3f((float)x_pos, (float)y_pos));
    }
  else
    {
    if (!fast_mode)
      gc.draw(background, Vector3f((float)x_pos, (float)y_pos));
    }

  gc.draw(surface, Vector3f((float)x_pos, (float)y_pos));
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
  server->set_pause(!server->get_pause());

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}


/* EOF */
