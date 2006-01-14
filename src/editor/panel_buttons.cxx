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

#include <ClanLib/Display/sprite.h>
#include "panel_buttons.hxx"
#include "../gui/combobox.hxx"
#include "../display/scene_context.hxx"
#include "../path_manager.hxx"
#include "../fonts.hxx"
#include "../resource.hxx"
#include "editor_screen.hxx"
#include "editor_panel.hxx"

namespace Pingus {

class Resource;

namespace Editor {

// Constructor
PanelButton::PanelButton(EditorPanel* p) :
  hover(false),
  panel(p)
{
  button = Resource::load_sprite("core/editor/button");
  button_pressed = Resource::load_sprite("core/editor/button_pressed");
}

// Draw the button
void
PanelButton::draw(Pingus::DrawingContext &gc)
{
	//if (is_pressed())
	//FIXME: Component::is_pressed() not implemented
	if (false)
		gc.draw(button_pressed, pos);
	else
		gc.draw(button, pos);

	// Print the tooltip
	if (hover)
		gc.print_left(Fonts::pingus_small, pos.x, pos.y + (float)get_width(), 
			tooltip, 50);

	// Draw the actual button image
	gc.draw(sur, pos);
}

// Set the position of this panelbutton
void
PanelButton::set_pos(Vector p)
{
	pos = p;
}

// Is this button at these x,y coordinates?
bool
PanelButton::is_at(int x, int y)
{
  return (x > pos.x && x < pos.x + sur.get_width()
	  && y > pos.y && y < pos.y + sur.get_height());
}

// Standard exit button
PanelButtonExit::PanelButtonExit(EditorPanel *p) :
	PanelButton(p)
{
	tooltip = "Exit the editor";
  sur = Resource::load_sprite("core/editor/exit");
}

// When clicked, close the EditorScreen
void
PanelButtonExit::on_primary_button_click(int x, int y)
{
	panel->get_screen()->on_escape_press();
}

// Standard Load button
PanelButtonLoad::PanelButtonLoad(EditorPanel *p) :
	PanelButton(p)
{
	tooltip = "Load existing level";
  sur = Resource::load_sprite("core/editor/open");
}

// When clicked, close the EditorScreen
void
PanelButtonLoad::on_primary_button_click(int x, int y)
{
	panel->get_screen()->show_file_dialog(true);
}

// Standard save button
PanelButtonSave::PanelButtonSave(EditorPanel *p) :
	PanelButton(p)
{
	tooltip = "Save current level";
  sur = Resource::load_sprite("core/editor/save");
}

// When clicked, close the EditorScreen
void
PanelButtonSave::on_primary_button_click(int x, int y)
{
	// TODO: Open a file dialog box to save the level.
	panel->get_screen()->save(path_manager.complete("levels/test.pingus"), ".pingus");
}


// Load the groundpieces
PanelButtonGroundpiece::PanelButtonGroundpiece(EditorPanel *p) :
	PanelButton(p)
{
	sur = Resource::load_sprite("core/editor/save"); // FIXME: Update this sprite 
	tooltip = "Load a groundpiece";
}

void
PanelButtonGroundpiece::on_primary_button_click(int x, int y)
{
	UNUSED_ARG(x);
	UNUSED_ARG(y);
	
	// FIXME: Add actual groundpieces
	panel->get_combobox()->clear();
	panel->get_combobox()->set_label("Groundpieces");
	panel->get_combobox()->add(new GUI::ComboItem(1, "Testing 1"));
	panel->get_combobox()->add(new GUI::ComboItem(2, "Testing 2"));
	panel->get_combobox()->set_enabled(true);
}

} // Editor namespace
} // Pingus namespace
