//  $Id: panel_icons.cxx,v 1.13 2003/10/18 23:17:27 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Display/display.h>
#include "../gettext.h"
#include "../resource.hxx"
#include "panel_icons.hxx"
#include "editor.hxx"
#include "editor_event.hxx"

namespace Pingus {
namespace EditorNS {

PanelIconLoad::PanelIconLoad()
{
  tooltip = _("Load a level (F5)");
  sur = Resource::load_sprite("editor/open", "core");
}

void
PanelIconLoad::on_click()
{
  std::cout << "Load level..." << std::endl;
  editor->get_event()->editor_load_level();
}

PanelIconExit::PanelIconExit()
{
  tooltip = _("Exit the editor (Escape)");
  sur = Resource::load_sprite("editor/exit", "core");
}

void
PanelIconExit::on_click()
{
  editor->get_event()->editor_exit();
}

PanelIconSave::PanelIconSave()
{
  tooltip = _("Save this level (F6)");
  sur = Resource::load_sprite("editor/save", "core");
}

void
PanelIconSave::on_click()
{
  editor->get_event()->editor_save_level_as();
}

PanelIconDelete::PanelIconDelete()
{
  tooltip = _("Delete marked objects (delete)");
  sur = Resource::load_sprite("editor/delete", "core");
}

void
PanelIconDelete::on_click()
{
  editor->get_event()->editor_delete_selected_objects();
}

PanelIconCopy::PanelIconCopy()
{
  tooltip = _("Duplicate current object (d)");
  sur = Resource::load_sprite("editor/copy", "core");
}

void
PanelIconCopy::on_click()
{
  editor->get_event()->editor_duplicate_current_selection();
}

PanelIconLevelProperties::PanelIconLevelProperties()
{
  tooltip = _("Edit Level Properties");
  sur = Resource::load_sprite("editor/edit", "core");
}

void
PanelIconLevelProperties::on_click()
{
  editor->get_event ()->editor_show_level_properties ();
}

PanelIconObjectProperties::PanelIconObjectProperties()
{
  tooltip = _("Edit Object Properties");
  sur = Resource::load_sprite("editor/preferences", "core");
}

void
PanelIconObjectProperties::on_click()
{
  editor->get_event ()->editor_show_object_properties ();
}

PanelIconRun::PanelIconRun()
{
  tooltip = _("Start the level and test it (F4)");
  sur = Resource::load_sprite("editor/clock_run", "core");
}

void
PanelIconRun::on_click()
{
  editor->get_event()->editor_start_current_level();
}

PanelIconNew::PanelIconNew()
{
  tooltip = _("Create a new level from scratch");
  sur = Resource::load_sprite("editor/new", "core");
}

void
PanelIconNew::on_click()
{
  editor->get_event()->editor_new_level();
}

PanelIconInsertObject::PanelIconInsertObject()
{
  tooltip = _("Insert an object (Insert)");
  sur = Resource::load_sprite("editor/index", "core");
}

void
PanelIconInsertObject::on_click()
{
  editor->get_event()->editor_insert_new_object();
}

PanelIconRegionZoom::PanelIconRegionZoom ()
{
  tooltip = _("Zoom into a region");
  sur = Resource::load_sprite("editor/region-zoom", "core");
}

void
PanelIconRegionZoom::on_click()
{
  editor->get_event ()->zoom_mode ();
}

PanelIconZoomIn::PanelIconZoomIn()
{
  tooltip = _("Zoom in");
  sur = Resource::load_sprite("editor/zoom-in", "core");
}

void
PanelIconZoomIn::on_click()
{
  editor->get_event ()->editor_zoom_in(CL_Display::get_width()/2, 
                                       CL_Display::get_height()/2);
}

PanelIconZoomOut::PanelIconZoomOut()
{
  tooltip = _("Zoom out");
  sur = Resource::load_sprite("editor/zoom-out", "core");
}

void
PanelIconZoomOut::on_click()
{
  editor->get_event ()->editor_zoom_out (CL_Display::get_width()/2, 
                                         CL_Display::get_height()/2);
}

PanelIconSetupActions::PanelIconSetupActions ()
{
  tooltip = _("Setup Number of Actions");
  sur = Resource::load_sprite("editor/actions", "core");
}

void
PanelIconSetupActions::on_click()
{
  editor->get_event()->editor_show_actions_window ();
}

PanelIconHelp::PanelIconHelp ()
{
  tooltip = _("Display Help Screen (F1)");
  sur = Resource::load_sprite("editor/help", "core");
}

void
PanelIconHelp::on_click()
{
  editor->toggle_help_screen ();
}

} // namespace EditorNS
} // namespace Pingus

/* EOF */
