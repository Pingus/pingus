//  $Id: object_selector_window.cxx,v 1.3 2003/03/04 12:53:47 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "editor.hxx"
#include "object_selector.hxx"
#include "object_manager.hxx"
#include "../groundtype.hxx"
#include "object_selector_window.hxx"

using namespace EditorNS;

ObjectSelectorWindow::ObjectSelectorWindow(CL_Component* parent)
  : CL_Window(CL_Rect(50, 50, 250, 410), "Object Inserter", parent),
    y_pos(5)
{
  //close_button_slot = close_button.sig_clicked().connect(this, &ObjectSelectorWindow::on_close_press);
  add_button("Groundpieces (ground)",      &ObjectSelectorWindow::on_groundpiece_ground_press);
  add_button("Groundpieces (solid)",       &ObjectSelectorWindow::on_groundpiece_solid_press);
  add_button("Groundpieces (transparent)", &ObjectSelectorWindow::on_groundpiece_transparent_press);
  add_button("Groundpieces (remove)",      &ObjectSelectorWindow::on_groundpiece_remove_press);
  add_button("Hotspot",                    &ObjectSelectorWindow::on_hotspot_press);
  add_button("Entrance",                   &ObjectSelectorWindow::on_entrance_press);
  add_button("Exit",                       &ObjectSelectorWindow::on_exit_press);
  add_button("Liquid",                     &ObjectSelectorWindow::on_liquid_press);
  add_button("Weather",                    &ObjectSelectorWindow::on_weather_press);
  add_button("Trap",                       &ObjectSelectorWindow::on_trap_press);
  add_button("WorldObject",                &ObjectSelectorWindow::on_worldobject_press);
  add_button("Background",                 &ObjectSelectorWindow::on_background_press);
  add_button("Prefab",                     &ObjectSelectorWindow::on_prefab_press);
  add_button("From File",                  &ObjectSelectorWindow::on_from_file_press);
  y_pos += 10;
  add_button("Close",                      &ObjectSelectorWindow::on_close_press);

  set_client_size(200, y_pos + 10);
}

ObjectSelectorWindow::~ObjectSelectorWindow()
{
  for (std::vector<ButtonPair*>::iterator i = buttons.begin();  i != buttons.end(); ++i)
    delete *i;
}

void
ObjectSelectorWindow::on_close_press()
{
  show(false);
}

void
ObjectSelectorWindow::add_button (const std::string& name, Callback callback)
{
  buttons.push_back(new ButtonPair(this, name, callback, y_pos));
  y_pos += 20;
}

void
ObjectSelectorWindow::on_groundpiece_ground_press()
{
  Editor::instance()->get_object_selector()->get_groundpiece(Groundtype::GP_GROUND);
}

void
ObjectSelectorWindow::on_groundpiece_solid_press()
{
 Editor::instance()->get_object_selector()->get_groundpiece(Groundtype::GP_SOLID);
}

void
ObjectSelectorWindow::on_groundpiece_transparent_press()
{
  Editor::instance()->get_object_selector()->get_groundpiece(Groundtype::GP_TRANSPARENT);
}

void
ObjectSelectorWindow::on_groundpiece_remove_press()
{
  Editor::instance()->get_object_selector()->get_groundpiece(Groundtype::GP_REMOVE);
}

void
ObjectSelectorWindow::on_hotspot_press()
{
  Editor::instance()->get_object_selector()->get_hotspot("hotspots");
}

void
ObjectSelectorWindow::on_entrance_press()
{
  Editor::instance()->get_object_selector()->get_entrance();
}

void
ObjectSelectorWindow::on_exit_press()
{
  Editor::instance()->get_object_selector()->get_exit();
}

void
ObjectSelectorWindow::on_liquid_press()
{
}

void
ObjectSelectorWindow::on_weather_press()
{
}

void
ObjectSelectorWindow::on_trap_press()
{
}

void
ObjectSelectorWindow::on_worldobject_press()
{
}

void
ObjectSelectorWindow::on_background_press()
{
}

void
ObjectSelectorWindow::on_prefab_press()
{
}

void
ObjectSelectorWindow::on_from_file_press()
{
}

/* EOF */
