//  $Id: action_window.hxx,v 1.1 2002/06/28 22:21:59 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_ACTION_WINDOW_HXX
#define HEADER_PINGUS_EDITOR_ACTION_WINDOW_HXX

#include <ClanLib/gui.h>
#include <vector>
#include "../action_data.hxx"

class ActionWindow
{
private:
  std::vector<ActionData>* actions;
  CL_Window* window;
  std::vector<CL_InputBox*> inputs;
  std::vector<CL_Label*> labels;

public:
  ActionWindow (CL_Component* parent, std::vector<ActionData>*); 

private:
  void on_close ();
};

#endif

/* EOF */
