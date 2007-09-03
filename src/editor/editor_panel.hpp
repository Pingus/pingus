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

#ifndef HEADER_PINGUS_EDITOR_PANEL_HXX
#define HEADER_PINGUS_EDITOR_PANEL_HXX

#include <vector>
#include <iostream>
#include "../gui/component.hpp"
#include "../gui/checkbox_listener.hpp"
#include "../gui/combobox_listener.hpp"


class DrawingContext;
class GUI::Checkbox;
class GUI::Combobox;

namespace Editor {

class PanelButton;
class EditorScreen;

/** This class contains all of the buttons, comboboxes, and other components */
class EditorPanel : public GUI::Component, 
                    public GUI::CheckboxListener, public GUI::ComboboxListener
{
private:
  /** The EditorScreen to which this panel belongs */
  EditorScreen* editor;

  /** Collection of buttons on this panel */
  std::vector<PanelButton*> panel_buttons;

  /** Currently selected object on this panel */
  PanelButton* pressed_button;

  /** Combobox which can change it's item list based on the button pressed */
  GUI::Combobox* combobox_1;
  GUI::Combobox* combobox_2;
  GUI::Combobox* combobox_3;

  /** Checkbox which controls the snap-to-grid functionality */
  GUI::Checkbox* snap_to_checkbox;

public:
  /** Constructor
      @param es The EditorScreen to which this panel belongs */
  EditorPanel(EditorScreen* es);

  /** Destructor - nothing really happens here */
  ~EditorPanel();

  /** Initialize the panel - Adds all of the buttons and text fields 
      Wait to run this until after the panel has been added to the gui_manager */
  void init();

  /** Add a button to the panel 
      @param button The PanelButton that you'd like to add. The given object will be
      deleted automaticly at the end. */
  void add(PanelButton* button);

  /** Draws the panel */
  void draw(DrawingContext& gc);

  /** Return the editor screen */
  EditorScreen* get_screen() { return editor; }
	
  /** Return the combobox object */
  GUI::Combobox* get_combobox(int i);
	
  /** Changes which button is currently pressed */
  void set_selected_button(PanelButton* pb);
	
  /** Event that fires when a checkbox gets changed */
  void checkbox_changed(bool new_value, GUI::Checkbox* box);
	
  /** Event that fires when a combobox gets changed */
  void combobox_changed(GUI::Combobox* box);


private:
  EditorPanel();
  EditorPanel (const EditorPanel&);
  EditorPanel& operator= (const EditorPanel&);

};	// EditorPanel
}		// Editor namespace

#endif

/* EOF */
