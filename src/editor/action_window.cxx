//  $Id: action_window.cxx,v 1.1 2002/06/28 22:21:59 grumbel Exp $
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

#include "../string_converter.hxx"
#include "action_window.hxx"

/**********************
  Actions
 ----------------------
  Basher      20 [+][-]
  Bridger     20 [+][-]
  Digger      20 [+][-]
  [...]
 ----------------------
 [ok]          [cancel]
 **********************/

ActionWindow::ActionWindow (CL_Component* parent, std::vector<ActionData>* arg_actions)
  : actions (arg_actions)
{
  window = new CL_Window (CL_Rect(200, 30,
				  400, (actions->size() * 30) + 30), "Pingus Actions", parent);

  int y = 30;
  for (std::vector<ActionData>::iterator i = actions->begin (); i != actions->end (); ++i)
    {
      inputs.push_back(new CL_InputBox (CL_Rect(100, y, 190, 0), to_string(i->number_of), window));
      labels.push_back(new CL_Label (CL_Point(10, y), action_to_string(i->name), window));
      y += 20;
    }
}

void
ActionWindow::on_close ()
{
  // Sync the level data with the data from the window
}

/* EOF */
