//  $Id: button_panel.cxx,v 1.19 2002/11/02 17:43:10 grumbel Exp $
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
#include <ClanLib/Core/System/system.h>
#include "globals.hxx"
#include "button_panel.hxx"
#include "true_server.hxx"
#include "plf.hxx"

using namespace Actions;

CL_Surface ButtonPanel::button_cap;

struct action_sorter {
  bool operator() (const ActionData& a, const ActionData& b) {
    return a.name < b.name;
  }
};

ButtonPanel::ButtonPanel(PLF* plf, int arg_x_pos, int arg_y_pos)
  : armageddon_pressed(false),
    left_pressed(0),
    last_press(0),
    x_pos (arg_x_pos), 
    y_pos (arg_y_pos)
{

  std::vector<ActionData> buttons_data = plf->get_actions();

  // Sort the action so that they always have the same order in the
  // panel
  std::sort(buttons_data.begin(), buttons_data.end(), action_sorter());

  y_pos -= (buttons_data.size() * 38)/2 + 70;

  if (buttons_data.size() == 0)
    {
      std::cout << "ButtonPanel: No actions given in this level, fall back to default" << std::endl;
      buttons_data.push_back(ActionData(Bridger, 20));
      buttons_data.push_back(ActionData(Basher, 20));
      buttons_data.push_back(ActionData(Digger, 20));
      buttons_data.push_back(ActionData(Miner, 20));
    }

  for(std::vector<ActionData>::size_type i = 0; i < buttons_data.size(); ++i)
    {
      a_buttons.push_back(new VerticalActionButton (x_pos, i * 38 + y_pos,
						    buttons_data[i].name,
						    0)); //FIXMEcontroller->get_owner ()));
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
  a_buttons[pressed_button]->update(delta);

  if (last_press + 350 < CL_System::get_time()) 
    {
      armageddon_pressed = 0;
    } 
}

ActionName
ButtonPanel::get_action_name()
{
  return a_buttons[pressed_button]->get_action_name();
}

void 
ButtonPanel::draw(GraphicContext& gc) 
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
ButtonPanel::set_server(TrueServer* s)
{
  server = s;

  for(AButtonIter button = a_buttons.begin(); button != a_buttons.end(); ++button) 
    {
      (*button)->set_action_holder(server->get_action_holder());
    }
}

void
ButtonPanel::set_client(Client* c)
{
  client = c;
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
	pressed_button = button - a_buttons.begin();
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
  pressed_button = (pressed_button + 1) % a_buttons.size();
}

/// Select the previous action
void 
ButtonPanel::previous_action ()
{
  pressed_button = (pressed_button - 1 + a_buttons.size()) % a_buttons.size();
}

/* EOF */
