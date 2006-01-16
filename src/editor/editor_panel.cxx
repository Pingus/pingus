//  $Id: editor_panel.cxx,v 1.00 2005/11/07 23:41:12 Jave27 Exp $
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

#include <ClanLib/Display/color.h>
#include <vector>
#include "../resource.hxx"
#include "../vector.hxx"
#include "../display/drawing_context.hxx"
#include "../gui/gui_manager.hxx"
#include "../gui/combobox.hxx"
#include "../fonts.hxx"
#include "editor_panel.hxx"
#include "editor_screen.hxx"
#include "panel_buttons.hxx"

namespace Pingus {

	class GUI::Component;

namespace Editor {

// Constructor
EditorPanel::EditorPanel(EditorScreen* es)
: editor(es), 
	pressed_button(0), 
	combobox_1(0),
	combobox_2(0)
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

	// Create Combobox
	combobox_1 = new GUI::Combobox(Vector(500,  0));
	combobox_2 = new GUI::Combobox(Vector(500, 30));
	get_screen()->get_gui_manager()->add(combobox_1);
	get_screen()->get_gui_manager()->add(combobox_2);
	combobox_1->set_enabled(false);
	combobox_2->set_enabled(false);
}

// Draw the panel
void
EditorPanel::draw (DrawingContext& gc)
{
	// Draw the panel
	gc.draw_fillrect(0, 0, (float)CL_Display::get_width(), 
		50.0f, CL_Color::lightgray, -50);
}

// Add the button to the vector, set it's position, and add to the gui_manager
void
EditorPanel::add(PanelButton* button)
{
	// Determind where to place this buttons
	Vector new_pos;

	if (panel_buttons.empty())
		new_pos = Vector(0.0f, 0.0f, 0.0f);
	else
		new_pos = Vector(panel_buttons.back()->get_pos() + 
			Vector((float)(5 + panel_buttons.back()->get_width()), 0.0f));
	// Add button to collection of buttons
	panel_buttons.push_back(button);
	
	// Add a 5 pixel spacing between all buttons
	button->set_pos(new_pos);

	// Add the button to the GUI Manager
	get_screen()->get_gui_manager()->add((GUI::Component*)button);
}

void 
EditorPanel::set_selected_button(PanelButton* pb)
{
	if (pressed_button)
		pressed_button->select(false);
	
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
		default :
			return 0;
	}
}

} // Editor namespace
} // Pingus namespace
