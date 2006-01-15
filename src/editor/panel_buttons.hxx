//  $Id: editor_panel.hxx,v 1.00 2005/11/07 23:41:12 Jave27 Exp $
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

#ifndef HEADER_PINGUS_PANEL_BUTTONS_HXX
#define HEADER_PINGUS_PANEL_BUTTONS_HXX

#include <ClanLib/Display/sprite.h>
#include "../gui/component.hxx"
#include "../vector.hxx"

namespace Pingus {

class DrawingContext;

namespace Editor {

class EditorPanel;

/** Generic PanelButton class - all buttons in the Editor should inherit from this */
class PanelButton : public GUI::Component
{
private:
	/** Whether or not the mouse is over the button */
	bool hover;

protected:
	/** The actual button image */
	CL_Sprite sur;

	/** The button background image when not pressed */
	CL_Sprite button;

	/** The button background image while pressed */
	CL_Sprite button_pressed;

	/** The panel to which this button belongs */
	EditorPanel* panel;

	/** The string that appears when the mouse is hovering over this button */
	std::string tooltip;

	/** The location of this button on the screen  (set by the EditorPanel) */
	Vector pos;

	/** Is this button currently selected? */
	bool is_selected;

public:
	/** Constructor 
		@param p The EditorPanel to which this button belongs */
	PanelButton(EditorPanel* p);

	/** Destructor - nothing really happens here */
	virtual ~PanelButton() { }

	/** Set the position of this button on the screen - used by the EditorPanel
		@param p the x,y,z Vector where this button belongs */
	void set_pos (Vector p);

	/** Returns the Vector of this button's location */
	Vector get_pos () { return pos; }

	/** Returns the width of the sur sprite */
	int get_width() { return sur.get_width(); }

	/** Returns the height of the sur sprite */
	int get_height() { return sur.get_height(); }
	
	/** Draws the button */
	void draw(DrawingContext& gc);

	/** Return true if the button is located at this x,y coordinate */
	bool is_at(int x, int y);
	
	/** Action taken when the button is clicked */
	virtual void on_primary_button_click(int x, int y);

	/** Action taken when the mouse enters the button area */
	virtual void on_pointer_enter () { hover = true; }

	/** Action taken when the mouse leaves the button area */
	virtual void on_pointer_leave () { hover = false; }
	
	/** Action taken when the button is selected or not */
	virtual void select(bool s) { is_selected = s; }

private:
	PanelButton (const PanelButton&);
	PanelButton& operator= (const PanelButton&);
};	// PanelButton class


/** Standard exit button */
class PanelButtonExit : PanelButton
{
public:
	/** Constructor
		@param p the EditorPanel to which this button belongs */
	PanelButtonExit (EditorPanel* p);

	/** This function is called by the gui_manager when the button is clicked */
	void on_primary_button_click (int x, int y);

private:
	PanelButtonExit ();
  PanelButtonExit (const PanelButtonExit&);
  PanelButtonExit& operator= (const PanelButtonExit&);
};		// PanelButtonExit class


/** Standard load button */
class PanelButtonLoad : PanelButton
{
public:
	/** Constructor
		@param p the EditorPanel to which this button belongs */
	PanelButtonLoad (EditorPanel* p);

	/** This function is called by the gui_manager when the button is clicked */
	void on_primary_button_click (int x, int y);

private:
	PanelButtonLoad ();
  PanelButtonLoad (const PanelButtonLoad&);
  PanelButtonLoad& operator= (const PanelButtonLoad&);
};		// PanelButtonLoad class


/** Standard save button */
class PanelButtonSave : PanelButton
{
public:
	/** Constructor
		@param p the EditorPanel to which this button belongs */
	PanelButtonSave (EditorPanel* p);

	/** This function is called by the gui_manager when the button is clicked */
	void on_primary_button_click (int x, int y);

private:
	PanelButtonSave ();
  PanelButtonSave (const PanelButtonSave&);
  PanelButtonSave& operator= (const PanelButtonSave&);
};		// PanelButtonSave class


/** Display all of the groundpiece objects */
class PanelButtonGroundpiece : PanelButton
{
public:
	/** Constructor
		@param p the EditorPanel to which this button belongs */
	PanelButtonGroundpiece (EditorPanel* p);

	/** This function is called by the gui_manager when the button is clicked */
	void on_primary_button_click (int x, int y);

private:
	PanelButtonGroundpiece ();
  PanelButtonGroundpiece (const PanelButtonGroundpiece&);
  PanelButtonGroundpiece& operator= (const PanelButtonGroundpiece&);
};		// PanelButtonGroundpiece class

}		// Editor namespace
}		// Pingus namespace


#endif

/* EOF */
