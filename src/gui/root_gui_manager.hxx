//  $Id: root_gui_manager.hxx,v 1.2 2002/07/29 11:57:38 grumbel Exp $
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

#ifndef HEADER_PINGUS_GUI_ROOT_GUI_MANAGER_HXX
#define HEADER_PINGUS_GUI_ROOT_GUI_MANAGER_HXX

#include "../input/controller.hxx"
#include "gui_manager.hxx"

namespace GUI
{
  /** Root GUI manager 
   */
  class RootGUIManager : public GUIManager
  {
  private:
    Input::Controller* controller;
    std::vector<Component*> components;

    /** The component which recieved the last pressed event */
    Component* pressed_component;

    /** The component over which the mouse was in the last update,
	used to detecte enter/leave events */
    Component* mouse_over_component;

    // FIXME: Hack:
    int x_pos;
    int y_pos;
  public:
    RootGUIManager (Input::Controller* c);
    ~RootGUIManager ();
    
    void draw ();
    void update (float delta);

    bool is_at (int x, int y);

    void process_input (float delta);
    Component* component_at (int x, int y);
    

    void add (Component* c) { components.push_back(c); }
    void remove (Component* c) { /* components.erase(c); */ }
  };
}

#endif

/* EOF */
