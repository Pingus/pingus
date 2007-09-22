//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_GUI_GROUP_COMPONENT_HPP
#define HEADER_PINGUS_GUI_GROUP_COMPONENT_HPP

#include "display/drawing_context.hpp"
#include "rect_component.hpp"

namespace GUI {

/** */
class GroupComponent : public RectComponent
{
private:
  typedef std::vector<Component*> Components;
  Components     children;
  Components     delete_children;
  DrawingContext drawing_context;

  /** Used to detect enter/leave events */
  Component*     mouse_over_comp;

  /** Used to decide where keyboard events should go */
  Component*     focused_comp;

  Component*     grabbed_comp;

  /** Used to do a mouse grab, as long as a button is pressed, the
      component that got the press gets moves and release events */
  Component* primary_pressed_comp;
  Component* secondary_pressed_comp;
  
public:
  GroupComponent(const Rect& rect, bool clip = true);
  virtual ~GroupComponent();  
  	
  void draw(DrawingContext& gc);
  virtual void draw_background(DrawingContext& gc) {}

  void update(float delta);

  void on_primary_button_press(int x, int y);
  void on_primary_button_release(int x, int y);
 
  void on_secondary_button_press(int x, int y);
  void on_secondary_button_release(int x, int y);
  
  void on_primary_button_click(int x, int y) {}
  void on_secondary_button_click(int x, int y) {}

  void on_key_pressed(const unsigned short c);

  void on_pointer_enter();
  void on_pointer_leave();

  void on_pointer_move(int x, int y);

  void add(Component*, bool delete_comp);

  void update_layout();

  bool is_at(int x, int y);

  // Causes all input directed to comp
  void grab(Component* comp);
  void ungrab(Component* comp);

  Component* component_at (const Vector2i& pos);
private:
  GroupComponent(const GroupComponent&);
  GroupComponent& operator=(const GroupComponent&);
};

} // namespace GUI

#endif

/* EOF */
