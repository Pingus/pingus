//  $Id: PanelIcons.cc,v 1.11 2001/06/11 20:40:17 grumbel Exp $
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

#include "../PingusResource.hh"
#include "PanelIcons.hh"

PanelIconLoad::PanelIconLoad()
{
  tooltip = "Load a level";
  sur = PingusResource::load_surface("editor/open", "core");
}

void
PanelIconLoad::on_click()
{
  std::cout << "Load level..." << std::endl;
  editor->get_event()->editor_load_level();
}

PanelIconExit::PanelIconExit()
{
  tooltip = "Exit the editor";
  sur = PingusResource::load_surface("editor/exit", "core");  
}

void 
PanelIconExit::on_click()
{
  editor->get_event()->editor_exit();
}

PanelIconSave::PanelIconSave()
{
  tooltip = "Save this level";
  sur = PingusResource::load_surface("editor/save", "core");
}

void 
PanelIconSave::on_click()
{
  editor->get_event()->editor_save_level_as();
}

PanelIconDelete::PanelIconDelete()
{
  tooltip = "Delete marked objects";
  sur = PingusResource::load_surface("editor/delete", "core");
}

void 
PanelIconDelete::on_click()
{
  editor->get_event()->editor_delete_selected_objects();
}

PanelIconCopy::PanelIconCopy()
{
  sur = PingusResource::load_surface("editor/copy", "core");
}

void
PanelIconCopy::on_click()
{
  editor->get_event()->editor_duplicate_current_selection();
}

PanelIconEdit::PanelIconEdit()
{
  tooltip = "???";
  sur = PingusResource::load_surface("editor/edit", "core");  
}

PanelIconPreferences::PanelIconPreferences()
{
  tooltip = "???";
  sur = PingusResource::load_surface("editor/preferences", "core");
}

void
PanelIconPreferences::on_click()
{
  std::cout << "Preferences not implemented" << std::endl;
}

void 
PanelIconEdit::on_click()
{
  // std::cout << "Edit not implemented" << std::endl;
  editor->edit_current_objs();
}

PanelIconRun::PanelIconRun()
{
  tooltip = "Start the level and test it";
  sur = PingusResource::load_surface("editor/clock_run", "core");  
}

void 
PanelIconRun::on_click()
{
  editor->get_event()->editor_start_current_level();
}

PanelIconNew::PanelIconNew()
{
  tooltip = "Create a new level from scratch";
  sur = PingusResource::load_surface("editor/new", "core");  
}

void
PanelIconNew::on_click()
{
  editor->get_event()->editor_new_level();
}

PanelIconInsertObject::PanelIconInsertObject()
{
  tooltip = "Insert an object";
  sur = PingusResource::load_surface("editor/index", "core");  
}

void
PanelIconInsertObject::on_click()
{
  editor->get_event()->editor_insert_new_object();
}

PanelIconRegionZoom::PanelIconRegionZoom ()
{
  tooltip = "Zoom into a region";
  sur = PingusResource::load_surface("editor/region-zoom", "core");
}

void 
PanelIconRegionZoom::on_click()
{
  editor->get_event ()->zoom_mode ();
}

PanelIconZoomIn::PanelIconZoomIn()
{
  tooltip = "Zoom in";
  sur = PingusResource::load_surface("editor/zoom-in", "core");
}

void 
PanelIconZoomIn::on_click()
{
  editor->get_event ()->editor_zoom_in ();
}

PanelIconZoomOut::PanelIconZoomOut()
{
  tooltip = "Zoom out";
  sur = PingusResource::load_surface("editor/zoom-out", "core");
}

void 
PanelIconZoomOut::on_click()
{
  editor->get_event ()->editor_zoom_out ();
}

/* EOF */
