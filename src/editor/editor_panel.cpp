//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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

#include <vector>
#include "../resource.hpp"
#include "../display/display.hpp"
#include "../math/vector3f.hpp"
#include "../display/drawing_context.hpp"
#include "../gui/gui_manager.hpp"
#include "../gui/checkbox.hpp"
#include "../gui/combobox.hpp"
#include "../fonts.hpp"
#include "editor_panel.hpp"
#include "editor_screen.hpp"
#include "viewport.hpp"
#include "panel_buttons.hpp"


class GUI::Component;

namespace Editor {

// Constructor
EditorPanel::EditorPanel(EditorScreen* es)
  : editor(es), 
    pressed_button(0), 
    combobox_1(0),
    combobox_2(0),
    snap_to_checkbox(0)
{

}

// Destructor
EditorPanel::~EditorPanel()
{
}

// Wait to run this until after the panel has been added to the gui_manager
// This sets up the buttons, text fields, etc. for this panel
// No need to delete the buttons because the gui_manager will do it for us.
void
EditorPanel::init()
{
  // Create panel buttons
  add((PanelButton*)(new PanelButtonExit(this)));
  add((PanelButton*)(new PanelButtonLoad(this)));
  add((PanelButton*)(new PanelButtonSave(this)));
  add((PanelButton*)(new PanelButtonGroundpiece(this)));
  add((PanelButton*)(new PanelButtonHead(this)));
  // Create Checkboxes
  snap_to_checkbox = new GUI::Checkbox(Vector3f(370, 5), "Snap To Grid: ", this);
  get_screen()->get_gui_manager()->add(snap_to_checkbox, true);
	
  // Create Comboboxes
  combobox_3 = new GUI::Combobox(Vector3f(500, 36), this);
  combobox_2 = new GUI::Combobox(Vector3f(500, 18), this);
  combobox_1 = new GUI::Combobox(Vector3f(500,  0), this);
  get_screen()->get_gui_manager()->add(combobox_3, true);
  get_screen()->get_gui_manager()->add(combobox_2, true);
  get_screen()->get_gui_manager()->add(combobox_1, true);
  combobox_3->set_enabled(false);
  combobox_2->set_enabled(false);
  combobox_1->set_enabled(false);
}

// Draw the panel
void
EditorPanel::draw (DrawingContext& gc)
{
  // Draw the panel
  gc.draw_fillrect(0, 0, (float)Display::get_width(), 
                   50.0f, Color(80,80,80), -50);
}

// Add the button to the vector, set it's position, and add to the gui_manager
void
EditorPanel::add(PanelButton* button)
{
  // Determind where to place this buttons
  Vector3f new_pos;

  if (panel_buttons.empty())
    new_pos = Vector3f(0.0f, 0.0f, 0.0f);
  else
    new_pos = Vector3f(panel_buttons.back()->get_pos() + 
                       Vector3f((float)(5 + panel_buttons.back()->get_width()), 0.0f));
  // Add button to collection of buttons
  panel_buttons.push_back(button);
	
  // Add a 5 pixel spacing between all buttons
  button->set_pos(new_pos);

  // Add the button to the GUI Manager
  get_screen()->get_gui_manager()->add((GUI::Component*)button, true);
}

void 
EditorPanel::set_selected_button(PanelButton* pb)
{
  if (pressed_button)
    {
      combobox_1->set_enabled(false);
      combobox_2->set_enabled(false);
      combobox_3->set_enabled(false);
      pressed_button->select(false);
    }
	
  pressed_button = pb;
  if (pressed_button)
    pressed_button->select(true);
}

GUI::Combobox*
EditorPanel::get_combobox(int i)
{
  switch(i)
    {
      case 1 :
        return combobox_1;
      case 2 :
        return combobox_2;
      case 3:
        return combobox_3;
      default :
        return 0;
    }
}

void
EditorPanel::checkbox_changed(bool new_value, GUI::Checkbox* box)
{
  if (box == snap_to_checkbox)
    editor->get_viewport()->set_snap_to(new_value);
}

void
EditorPanel::combobox_changed(GUI::Combobox* box)
{
  if (pressed_button)
    {	
      int i;
      if (box == combobox_1)
        i = 1;
      else if (box == combobox_2)
        i = 2;
      else
        i = 3;

      // Send the ID field of the Combobox to whichever button is currently selected.
      pressed_button->combobox_changed(i, 
                                       get_combobox(i)->get_selected_item()->get_id());
    }
	
}

} // Editor namespace
