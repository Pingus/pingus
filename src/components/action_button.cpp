//  $Id$
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
#include "../server.hpp"
#include "../game_session.hpp"
#include "../world.hpp"
#include "../display/drawing_context.hpp"
#include "../display/display.hpp"
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
  sprite.set_play_loop(true);
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
          gc.draw_fillrect(x_pos, y_pos, x_pos + 60, y_pos + 35,
                           Color(255, 255, 255));
        } 
      else 
        {
          //CL_Display::fill_rect(x_pos, y_pos, x_pos + 60, y_pos + 35 ,
          //1.0, 1.0, 1.0, 0.5);
          gc.draw(backgroundhl, Vector2i(x_pos, y_pos));
        }
    }
  else
    {
      sprite.set_frame(0);

      if (fast_mode) {
        // do nothing
      } else {
        gc.draw(background, Vector2i(x_pos, y_pos));
      }
    }

  if (this->get_action_name() == Climber) {
	  gc.draw(sprite, Vector2i(x_pos + 32, y_pos + 32));
  } else {
	  gc.draw(sprite, Vector2i(x_pos + 20, y_pos + 32));
  }

  Font myfont  = font;
  Font myfontb = font_b;

  // print the action name next to the button, when mouse pointer is on
  // the button.
  // FIXME: this should use the GUI events, not SDL_GetMouseState
  int x, y;
  SDL_GetMouseState(&x, &y);
  if (action_help
      && x > x_pos      && x < x_pos + 60
      && y < y_pos + 35 && y > y_pos)
    {
      gc.print_left(myfontb, x_pos + 65, y_pos-10, action_to_screenname(name));
    }

  if (Cheat::unlimited_actions)
    {
      // FIXME: insert unlimited surface here
      gc.print_center(myfont, x_pos + 46, y_pos + 5, "oo");
    }
  else
    {
      std::string str = StringUtil::to_string(action_holder->get_available(name));
      gc.print_center(myfont, x_pos + 46, y_pos + 5, str);
    }
}

ArmageddonButton::ArmageddonButton(Server* s, int x, int y)
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
      gc.draw(backgroundhl, Vector2i(x_pos, y_pos));
      gc.draw(sprite, Vector2i(x_pos, y_pos));
    }
  else
    {
      if (!fast_mode)
        gc.draw(background, Vector2i(x_pos, y_pos));

      sprite.set_frame(7);
      gc.draw(sprite, Vector2i(x_pos, y_pos));
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

ForwardButton::ForwardButton(GameSession* s, int x, int y)
  : session(s),
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
  if (session->get_fast_forward())
    {
      gc.draw(backgroundhl, Vector2i(x_pos, y_pos));
    }
  else
    {
      if (!fast_mode)
        gc.draw(background, Vector2i(x_pos, y_pos));
    }

  gc.draw(surface, Vector2i(x_pos, y_pos));
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
  session->set_fast_forward(!session->get_fast_forward());

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

PauseButton::PauseButton(GameSession* s, int x, int y)
  : session(s),
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
  if (session->get_pause())
    {
      gc.draw(backgroundhl, Vector2i(x_pos, y_pos));
    }
  else
    {
    if (!fast_mode)
      gc.draw(background, Vector2i(x_pos, y_pos));
    }

  gc.draw(surface, Vector2i(x_pos, y_pos));
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
  session->set_pause(!session->get_pause());

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}


/* EOF */
