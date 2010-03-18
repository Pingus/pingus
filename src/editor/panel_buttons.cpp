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
#include "../gui/combobox.hpp"
#include "../gui/gui_manager.hpp"
#include "../display/scene_context.hpp"
#include "../path_manager.hpp"
#include "../fonts.hpp"
#include "../resource.hpp"
#include "panel_buttons.hpp"
#include "editor_level.hpp"
//#include "editor_panel.hpp"
#include "editor_screen.hpp"
#include "level_objs.hpp"
#include "level_head.hpp"

class Resource;

namespace Editor {

// Constructor
PanelButton::PanelButton(EditorPanel* p) 
 : hover(false),
   panel(p),
   is_selected(false)
{
  button = Resource::load_sprite("core/editor/button");
  button_pressed = Resource::load_sprite("core/editor/button_pressed");
}

// Draw the button
void
PanelButton::draw(DrawingContext &gc)
{
  if (is_selected)
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
PanelButton::set_pos(Vector3f p)
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

// Something all buttons should do.
void
PanelButton::on_primary_button_click(int x, int y)
{
      panel->set_selected_button(this);
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
  PanelButton::on_primary_button_click(x, y);
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
  PanelButton::on_primary_button_click(x, y);
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
  PanelButton::on_primary_button_click(x, y);
  panel->get_screen()->show_file_dialog(false);	
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
  PanelButton::on_primary_button_click(x, y);

  panel->get_combobox(1)->clear();
  panel->get_combobox(1)->set_label("Type");
  panel->get_combobox(2)->set_label("Subtype");
  panel->get_combobox(3)->set_label("Groundpiece");

  std::vector<std::string> groundpieces; //// = Resource::get_sections("groundpieces");
  for (unsigned i = 0; i < groundpieces.size(); i++)
    {
      // We add each ComboItem to the first box:
      // - The ID field of the ComboItem should be the full resource name.
      // - The displayed field should only be the section name.
      panel->get_combobox(1)->add(new GUI::ComboItem("groundpieces/" + groundpieces[i], 
                                                     groundpieces[i]));
    }
  panel->get_combobox(1)->set_enabled(true);
}

// Populate the other comboboxes with the available resources.
void
PanelButtonGroundpiece::combobox_changed(int i, const std::string &value)
{
  std::string section;
  if (i == 1)
    {
      // Populate the 2nd combobox with the next resource section & disable box 3.
      panel->get_combobox(2)->clear();
      panel->get_combobox(2)->set_enabled(true);
      panel->get_combobox(3)->set_enabled(false);

      std::vector<std::string> groundpieces;//// = Resource::get_sections(
      ////panel->get_combobox(1)->get_selected_item()->get_displayed_string());
      for (std::vector<std::string>::const_iterator it = groundpieces.begin();
           it != groundpieces.end(); it++)
        panel->get_combobox(2)->add(new GUI::ComboItem(value + "/" + (*it), (*it)));		
    }
  else if (i == 2)
    {
      // Populate the 3rd combobox with the Resource ID's
      panel->get_combobox(3)->clear();
      panel->get_combobox(3)->set_enabled(true);

      std::vector<std::string> groundpieces;//// = Resource::get_resources(
      ////	"sprite", value);
      for (std::vector<std::string>::const_iterator it = groundpieces.begin();
           it != groundpieces.end(); it++)
        {
          // new_value is the "displayed_string" for the combobox.
          std::string new_value = (*it).substr(value.length()+1);
          panel->get_combobox(3)->add(new GUI::ComboItem(value + "/" + new_value, 
                                                         new_value));		
        }
    }
  else
    {
      // 3rd box was selected - add the resource to the current level.
      LevelObj* obj = new LevelObj("groundpiece", panel->get_screen()->get_level()->get_level_impl());
      obj->set_res_desc(ResDescriptor(value));
      obj->set_pos(Vector3f(0, 0, 0));
      // 1st combobox displayed the "type" of resource (solid, transparent, etc.)
      obj->set_type(panel->get_combobox(1)->get_selected_item()->get_displayed_string());
      panel->get_screen()->add_object(obj);
    }
}

PanelButtonHead::PanelButtonHead(EditorPanel *p)
  : PanelButton(p),
    head(0)
{  
  // FIXME: Update this sprite  
  sur = Resource::load_sprite("core/editor/save"); 
  tooltip = "Set specific level information"; 
}

void
PanelButtonHead::on_primary_button_click(int x, int y)
{
  PanelButton::on_primary_button_click(x, y); 
  if (!head)
    {  
      head = new LevelHead(panel, this);
    }
  else
    {  
      remove_head(); 
    }  
} 

void
PanelButtonHead::remove_head()
{
  delete head;
  head = 0; 
}
 
 
} // Editor namespace
