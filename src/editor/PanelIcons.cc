//  $Id: PanelIcons.cc,v 1.4 2000/06/25 20:22:18 grumbel Exp $
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
  sur = CL_Surface::load("Icons/open", PingusResource::get("editor.dat"));
}

void
PanelIconLoad::on_click()
{
  std::cout << "Load level..." << std::endl;
  event->editor_load_level();
}

PanelIconExit::PanelIconExit()
{
  sur = CL_Surface::load("Icons/exit", PingusResource::get("editor.dat"));  
}

void 
PanelIconExit::on_click()
{
  event->editor_exit();
}

PanelIconSave::PanelIconSave()
{
  sur = CL_Surface::load("Icons/save", PingusResource::get("editor.dat"));
}

void 
PanelIconSave::on_click()
{
  event->editor_save_level_as();
}

PanelIconDelete::PanelIconDelete()
{
  sur = CL_Surface::load("Icons/delete", PingusResource::get("editor.dat"));
}

void 
PanelIconDelete::on_click()
{
  event->editor_delete_selected_objects();
}

PanelIconCopy::PanelIconCopy()
{
  sur = CL_Surface::load("Icons/copy", PingusResource::get("editor.dat"));
}

void
PanelIconCopy::on_click()
{
  event->editor_duplicate_current_selection();
}

PanelIconEdit::PanelIconEdit()
{
  sur = CL_Surface::load("Icons/edit", PingusResource::get("editor.dat"));  
}

PanelIconPreferences::PanelIconPreferences()
{
  sur = CL_Surface::load("Icons/preferences", PingusResource::get("editor.dat"));
}

void
PanelIconPreferences::on_click()
{
  std::cout << "Preferences not implemented" << std::endl;
}

void 
PanelIconEdit::on_click()
{
  std::cout << "Edit not implemented" << std::endl;
}

PanelIconRun::PanelIconRun()
{
  sur = PingusResource::load_surface("Icons/clock_run", "editor");  
}

void 
PanelIconRun::on_click()
{
  event->editor_start_current_level();
}

PanelIconNew::PanelIconNew()
{
  sur = CL_Surface::load("Icons/new", PingusResource::get("editor.dat"));  
}

void
PanelIconNew::on_click()
{
  event->editor_new_level();
}

PanelIconInsertObject::PanelIconInsertObject()
{
  sur = CL_Surface::load("Icons/index", PingusResource::get("editor.dat"));  
}

void
PanelIconInsertObject::on_click()
{
  event->editor_insert_new_object();
}

/* EOF */
