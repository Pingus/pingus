//  $Id: action_window.cxx,v 1.14 2003/04/18 09:22:29 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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
#include <ClanLib/GUI/window.h>
#include <ClanLib/GUI/checkbox.h>
#include <ClanLib/GUI/inputbox.h>
#include "../string_converter.hxx"
#include "action_window.hxx"

/***************************
  Actions
 ---------------------------
  [x] Basher      20 [+][-]
  [ ] Bridger     20 [+][-]
  [x] Digger      20 [+][-]
  [...]
 ---------------------------
 [    ok     ]  [  cancel  ]
****************************/

// FIXME: [+][-] buttons are missing, need an own widget, anybody
// FIXME: wants to write that?

using namespace Actions;

namespace EditorNS {

ActionWindow::ActionWindow (CL_Component* arg_parent, std::vector<ActionData>* arg_actions)
  : parent (arg_parent), actions (arg_actions)
{
  window = new CL_Window (CL_Rect(0, 0,
  				  200, (actions->size() * 20) + 80), "Pingus Actions", parent);
  
  //window = new CL_Frame (CL_Rect (0, 0, 200, (actions->size() * 20) + 80), parent);
  //window->enable_fill (true);

  // Create all the default gui stuff
  int y = 30;

  for (std::vector<ActionData>::iterator i = default_actions.begin (); i != default_actions.end (); ++i)
    {
      checkboxes.push_back(new CL_CheckBox(CL_Point (10, y), action_to_string(i->name, TRANSLATED), window));
      inputs.push_back(new CL_InputBox (CL_Rect(100, y, 190, 0), to_string(i->number_of), window));
      y += 20;
    }

  y += 10;
  ok_button = new CL_Button(CL_Rect (10, y, 90, y + 20), "Ok", window);
  cancel_button = new CL_Button(CL_Rect (110, y, 190, y + 20), "Cancel", window);

  ok_button_slot     = ok_button->sig_clicked ().connect (this, &ActionWindow::ok_clicked);
  cancel_button_slot = cancel_button->sig_clicked ().connect (this, &ActionWindow::cancel_clicked);
  
  hide ();
  //window->run ();
  window->set_position(100, 100);
}

void
ActionWindow::show ()
{
  //std::cout << "show" << std::endl;
  read_data ();
  window->show (true);
}

void
ActionWindow::hide ()
{
  //std::cout << "hide" << std::endl;
  window->show (false);
}

void 
ActionWindow::ok_clicked ()
{
  //std::cout << "OK Clicked" << std::endl;
  write_data ();
  hide ();
}

void
ActionWindow::cancel_clicked ()
{
  //std::cout << "Cancel Clicked" << std::endl;
  hide ();
}

class is_equal
{
private:
  ActionName action;

public:
  is_equal (ActionName a) : action (a)
  {
  }

  bool operator() (const ActionData& b)
  {
    return action == b.name;
  }
};

void
ActionWindow::read_data ()
{
  //std::cout << "Reading data" << std::endl;
  for (unsigned int i = 0; i <  default_actions.size(); ++i)
    {
      std::vector<ActionData>::iterator act = std::find_if (actions->begin (), actions->end (),
							    is_equal (default_actions[i].name));
      if (act == actions->end ())
	{
	  checkboxes[i]->set_checked (false);
	  //inputs[i]->set_text (to_string(0));
	}
      else
	{
	  checkboxes[i]->set_checked (true);
	  inputs[i]->set_text (to_string(act->number_of));
	}
    }
}

void
ActionWindow::write_data ()
{
  //std::cout << "Writing data" << std::endl;

  for (unsigned int i = 0; i < default_actions.size(); ++i)  
    {
      if (checkboxes[i]->is_checked ())
	{
	  std::vector<ActionData>::iterator act = std::find_if (actions->begin (), actions->end (),
								is_equal (default_actions[i].name));
	  if (act == actions->end ())
	    {
	      ActionData data;
	      from_string(inputs[i]->get_text (), data.number_of);
	      data.name = default_actions[i].name;
	      
	      actions->push_back (data);
	    }
	  else
	    {
	      from_string(inputs[i]->get_text (), act->number_of); 
	    }
	}
      else
	{
	  std::vector<ActionData>::iterator pos
	    = std::remove_if (actions->begin (), actions->end (), is_equal(default_actions[i].name));
	  actions->erase(pos, actions->end ());
	}
    }
}

} // namespace EditorNS

/* EOF */
