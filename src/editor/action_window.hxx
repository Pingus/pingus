//  $Id: action_window.hxx,v 1.8 2002/09/27 11:26:45 torangan Exp $
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

#include <ClanLib/Signals/slot.h>
#include <vector>
#include "../action_data.hxx"

class CL_Button;
class CL_Window;
class CL_CheckBox;
class CL_InputBox;
class CL_Component;

namespace EditorNS {

class ActionWindow
{
private:
  CL_Component* parent;
  std::vector<ActionData>* actions;
  CL_Window* window;
  std::vector<CL_CheckBox*> checkboxes;
  std::vector<CL_InputBox*> inputs;

  CL_Button* ok_button;
  CL_Button* cancel_button;

  CL_Slot ok_button_slot;
  CL_Slot cancel_button_slot;
public:
  ActionWindow (CL_Component* parent, std::vector<ActionData>*); 

  void ok_clicked ();
  void cancel_clicked ();

  void show ();
  void hide ();
private:
  void read_data ();
  void write_data ();
  
  ActionWindow (const ActionWindow&);
  ActionWindow& operator= (const ActionWindow&);
};

} // namespace EditorNS

#endif

/* EOF */
