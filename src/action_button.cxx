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

#include <ClanLib/display.h>

#include "globals.hxx"
#include "cheat.hxx"
#include "pingus_resource.hxx"
#include "action_button.hxx"
#include "true_server.hxx"
#include "world.hxx"
#include "gui/graphic_context.hxx"
#include "string_converter.hxx"
#include "gui/display.hxx"
#include "fonts.hxx"

namespace Pingus {

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

  sprite = Sprite("Pingus/" + action_to_string(name) + to_string(owner_id), "pingus", 25.0f);
  sprite.set_align_center_bottom();

  // FIXME: Big fat hack
  if (   name == Digger  || name == Bomber
      || name == Floater || name == Blocker)
    {
      is_multi_direct = false;
    }
  else
    {
      is_multi_direct = true;
    }
}

bool
ActionButton::is_pressed()
{
  return false;
}

void
ActionButton::update(float delta)
{
  sprite.update(0.010f); // FIXME: Dirty hack
  UNUSED_ARG(delta);
}

ActionName
ActionButton::get_action_name()
{
  return name;
}

VerticalActionButton::VerticalActionButton(ActionHolder* h,
                                           int x, int y, ActionName name_, int owner_id)
  : ActionButton(h),
    background (PingusResource::load_surface("buttons/buttonbackground", "core")),
    backgroundhl (PingusResource::load_surface("buttons/buttonbackgroundhl", "core"))
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
VerticalActionButton::draw (GraphicContext& gc)
{
  UNUSED_ARG(gc);

  if (pressed)
    {
      if (fast_mode) {
        CL_Display::fill_rect(CL_Rect(x_pos, y_pos, x_pos + 60, y_pos + 35),
                              Display::to_color(1.0, 1.0, 1.0, 1.0));
      } else {
        //CL_Display::fill_rect(x_pos, y_pos, x_pos + 60, y_pos + 35 ,
        //1.0, 1.0, 1.0, 0.5);
        backgroundhl.draw(x_pos, y_pos);
      }
    }
  else
    {
      sprite.set_frame(0);

      if (fast_mode) {
        // do nothing
      } else {
        background.draw (x_pos, y_pos);
      }
    }

  if (is_multi_direct)
    sprite.set_direction(Sprite::RIGHT);
  gc.draw(sprite, Vector(x_pos + 20, y_pos + 32));

#ifdef CLANLIB_0_6
  // print the action name next to the button, when mouse pointer is on
  // the button.
  // FIXME: this should use the GUI events, not CL_Mouse
  if (action_help
      && CL_Mouse::get_x() > x_pos      && CL_Mouse::get_x() < x_pos + 60
      && CL_Mouse::get_y() < y_pos + 35 && CL_Mouse::get_y() > y_pos)
    {
      font_b->print_left(x_pos + 65, y_pos, action_to_screenname(name));
    }

  if (Cheat::unlimited_actions)
    {
      // FIXME: insert unlimited surface here
      font->print_center(x_pos + 46, y_pos + 5, "oo");
    }
  else
    {
      std::string str = to_string(action_holder->get_available(name));
      font->print_center(x_pos + 46, y_pos + 5, str.c_str ());
    }
#endif
}

ArmageddonButton::ArmageddonButton (TrueServer* s, int x, int y)
  : server (s),
    x_pos (x),
    y_pos (y),
    background  (PingusResource::load_surface("buttons/hbuttonbgb", "core")),
    backgroundhl(PingusResource::load_surface("buttons/hbuttonbg", "core"))
{
  pressed      = false;
  sprite       = Sprite("buttons/armageddon_anim", "core");
}

ArmageddonButton::~ArmageddonButton () { }

void
ArmageddonButton::draw (GraphicContext& gc)
{
  if (server->get_world()->check_armageddon ())
    {
      backgroundhl.draw(x_pos, y_pos);
      gc.draw(sprite, Vector(x_pos, y_pos));
    }
  else
    {
      if (!fast_mode)
        background.draw(x_pos, y_pos);

      sprite.set_frame(7);
      gc.draw(sprite, Vector(x_pos, y_pos));
    }
}

void
ArmageddonButton::update (float delta)
{
  sprite.update(0.010f); // FIXME: Dirty hack

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
      backgroundhl.draw(x_pos, y_pos);
    }
  else
    {
      if (!fast_mode)
        background.draw(x_pos, y_pos);
    }

  surface.draw(x_pos, y_pos);
  UNUSED_ARG(gc);
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
    backgroundhl.draw(x_pos, y_pos);
  else
    {
    if (!fast_mode)
      background.draw(x_pos, y_pos);
    }

  surface.draw(x_pos, y_pos);
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
  server->set_pause(!server->get_pause());

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

} // namespace Pingus

/* EOF */
