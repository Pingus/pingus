//  $Id: object_selector_window.hxx,v 1.1 2002/12/02 10:40:19 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_WINDOW_HXX
#define HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_WINDOW_HXX

#include <ClanLib/GUI/window.h>
#include <ClanLib/GUI/button.h>

/** */
class ObjectSelectorWindow : public CL_Window
{
private:
  int y_pos;
  
  CL_Button groundpiece_ground_button;
  CL_Button groundpiece_solid_button;
  CL_Button groundpiece_transparent_button;
  CL_Button groundpiece_remove_button;

  CL_Button hotspot_button;
  CL_Button entrance_button;
  CL_Button exit_button;

  CL_Button liquid_button;
  CL_Button weather_button;
  CL_Button trap_button;
  CL_Button worldobj_button;

  CL_Button background_button;
  CL_Button prefab_button;
  CL_Button file_button;

  CL_Button close_button;

  CL_Slot close_button_slot;

public:
  ObjectSelectorWindow(CL_Component*);
  
  void on_close_press();
  
private:
  ObjectSelectorWindow (const ObjectSelectorWindow&);
  ObjectSelectorWindow& operator= (const ObjectSelectorWindow&);
};

#endif

/* EOF */
