//  $Id: gui_manager.hxx,v 1.4 2002/07/30 14:57:26 grumbel Exp $
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

#ifndef HEADER_PINGUS_GUI_GUI_MANAGER_HXX
#define HEADER_PINGUS_GUI_GUI_MANAGER_HXX

#include <vector>
#include <list>
#include "component.hxx"

class GameDelta;

namespace Input
{
  class Event;
}

namespace GUI
{
  /** interface class which holds all GUI components and displays
      them */
  class GUIManager : public Component
  {
  private:
    std::vector<Component*> components;

    /** The component which recieved the last pressed event */
    Component* pressed_component;

    /** The component over which the mouse was in the last update,
	used to detecte enter/leave events */
    Component* mouse_over_component;

    // FIXME: Hack: should be handled inside the controller
    int x_pos;
    int y_pos;

    void process_input (const std::list<Input::Event*>& events);
  public:
    GUIManager ();
    virtual ~GUIManager () {}
    
    virtual void draw ();
    virtual void update (const GameDelta& delta);
    virtual void update (float delta) {}

    /** Add a component to the manager, if delete_component is true
	the component will get deleted on destruction of the manager,
	if false is supplied the user has to handle the component
	itself */
    void add (Component*, bool delete_component = true);

    /** */
    void remove (Component*);

    Component* component_at (int x, int y);  
    bool is_at (int x, int y);
  };
}

#endif

/* EOF */
