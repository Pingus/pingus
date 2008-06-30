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

#include <iostream>
#include <algorithm>
#include "../globals.hpp"
#include "../server.hpp"
#include "../game_session.hpp"
#include "button_panel.hpp"


using namespace Actions;

struct action_sorter {
  bool operator() (const ActionName& a, const ActionName& b) {
    return a < b;
  }
};

ButtonPanel::ButtonPanel(GameSession* s, int arg_x_pos, int arg_y_pos)
  : session(s),
    left_pressed(0),
    last_press(0),
    x_pos (arg_x_pos),
    y_pos (arg_y_pos)
{
  ActionHolder* aholder = session->get_server()->get_action_holder();

  std::vector<ActionName> actions = aholder->get_available_actions();

  // Sort the action so that they always have the same order in the
  // panel
  std::sort(actions.begin(), actions.end(), action_sorter());

  y_pos -= ((int)actions.size() * 38)/2 + 70;

  for(std::vector<ActionName>::iterator i = actions.begin();
      i != actions.end(); ++i)
    {
      a_buttons.push_back(new VerticalActionButton (aholder,
                                                    x_pos, int(i - actions.begin()) * 38 + y_pos,
						    *i,
						    0)); //FIXMEcontroller->get_owner ()));
    }

  if (a_buttons.empty())
    {
      std::cout << "Error: ButtonPanel: No a_buttons! " << std::endl;
    }

  pressed_button = 0;
}

ButtonPanel::~ButtonPanel()
{
  for (AButtonIter it = a_buttons.begin(); it != a_buttons.end(); ++it)
  {
    delete *it;
  }
}

void
ButtonPanel::update(float delta)
{
  if (!a_buttons.empty())
    a_buttons[pressed_button]->update(delta);
}

ActionName
ButtonPanel::get_action_name()
{
  if (!a_buttons.empty())
    return a_buttons[pressed_button]->get_action_name();
  else
    return Actions::Bridger;
}

void
ButtonPanel::draw(DrawingContext& gc)
{
  float alpha;

  if (fast_mode)
    alpha = 1.0;
  else
    alpha = 0.5;

  for(int i = 0; i < static_cast<int>(a_buttons.size()); ++i)
    {
      if (i == pressed_button)
	a_buttons[i]->pressed = true;
      else
        a_buttons[i]->pressed = false;

      a_buttons[i]->draw(gc);
    }
}

void
ButtonPanel::set_button(int n)
{
  if (n < 0 || n >= static_cast<int>(a_buttons.size()))
    {
      // FIXME: Play 'boing' sound here
    }
  else
    {
      pressed_button = n;
    }
}

void
ButtonPanel::on_primary_button_press(int x, int y)
{
  for(AButtonIter button = a_buttons.begin(); button != a_buttons.end(); button++)
    {
      if ((*button)->is_at(x, y))
	pressed_button = int(button - a_buttons.begin());
    }
}

bool
ButtonPanel::is_at (int x, int y)
{
  for(AButtonIter button = a_buttons.begin(); button != a_buttons.end(); button++)
    {
      if ((*button)->is_at(x, y))
	return true;
    }
  return false;
}

void
ButtonPanel::on_primary_button_release(int x, int y)
{
  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

/// Select the next action
void
ButtonPanel::next_action ()
{
  pressed_button = (pressed_button + 1) % (int)a_buttons.size();
}

/// Select the previous action
void
ButtonPanel::previous_action ()
{
  pressed_button = (pressed_button - 1 + (int)a_buttons.size()) % (int)a_buttons.size();
}


/* EOF */
