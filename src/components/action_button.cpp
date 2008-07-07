//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "../globals.hpp"
#include "../cheat.hpp"
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

ActionButton::ActionButton(ActionHolder* h,
                           int x, int y, ActionName name_, int owner_id)
  : RectComponent(Rect(Vector2i(x, y), Size(60, 38))),
    action_holder(h),
    background("core/buttons/buttonbackground"),
    backgroundhl("core/buttons/buttonbackgroundhl"),
    name(name_)
{
  sprite = Sprite("pingus/player0/" + action_to_string(name) + "/right");
  sprite.set_play_loop(true);
}

ActionButton::~ActionButton() {}

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

void
ActionButton::draw (DrawingContext& gc)
{
  Vector2i pos(rect.left, rect.top);

  if (pressed)
    {
      gc.draw(backgroundhl, pos);
    }
  else
    {
      sprite.set_frame(0);
      gc.draw(background, pos);
    }
  
  gc.draw(sprite, Vector2i(pos.x + 20, pos.y + 32));
  
  std::string str = StringUtil::to_string(action_holder->get_available(name));
  gc.print_center(Fonts::pingus_small, pos.x + 46, pos.y + 5, str);
}

ArmageddonButton::ArmageddonButton(Server* s, int x, int y)
  : RectComponent(Rect(Vector2i(x, y), Size(38, 60))),
    server(s),
    background  ("core/buttons/hbuttonbgb"),
    backgroundhl("core/buttons/hbuttonbg")
{
  pressed      = false;
  sprite       = Sprite("core/buttons/armageddon_anim");
}

ArmageddonButton::~ArmageddonButton () 
{
}

void
ArmageddonButton::draw (DrawingContext& gc)
{
  Vector2i pos(rect.left, rect.top);

  if (server->get_world()->check_armageddon ())
    {
      gc.draw(backgroundhl, pos);
      gc.draw(sprite, pos);
    }
  else
    {
      gc.draw(background, pos);

      sprite.set_frame(7);
      gc.draw(sprite, pos);
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
  : RectComponent(Rect(Vector2i(x, y), Size(38, 60))),
    session(s),
    background ("core/buttons/hbuttonbgb"),
    backgroundhl("core/buttons/hbuttonbg")
{
  surface = Sprite("core/buttons/fast_forward");
}

ForwardButton::~ForwardButton () {}

void
ForwardButton::draw (DrawingContext& gc)
{
  Vector2i pos(rect.left, rect.top);

  if (session->get_fast_forward())
    {
      gc.draw(backgroundhl, pos);
    }
  else
    {
      gc.draw(background, pos);
    }

  gc.draw(surface, pos);
}

void
ForwardButton::on_primary_button_click (int x, int y)
{
  session->set_fast_forward(!session->get_fast_forward());

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

PauseButton::PauseButton(GameSession* s, int x, int y)
  : RectComponent(Rect(Vector2i(x, y), Size(38, 60))),
    session(s),
    background ("core/buttons/hbuttonbgb"),
    backgroundhl("core/buttons/hbuttonbg")
{
  surface = Sprite("core/buttons/pause");
}

PauseButton::~PauseButton () 
{
}

void
PauseButton::draw (DrawingContext& gc)
{
  Vector2i pos(rect.left, rect.top);

  if (session->get_pause())
    {
      gc.draw(backgroundhl, pos);
    }
  else
    {
      gc.draw(background, pos);
    }

  gc.draw(surface, pos);
}

void
PauseButton::on_primary_button_click (int x, int y)
{
  session->set_pause(!session->get_pause());

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

/* EOF */
