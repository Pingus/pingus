//  $Id: object_selector_window.cxx,v 1.1 2002/12/02 10:40:19 grumbel Exp $
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

#include "object_selector_window.hxx"

ObjectSelectorWindow::ObjectSelectorWindow(CL_Component* parent)
  : CL_Window(CL_Rect(50, 50, 250, 410), "Object Inserter", parent),
    y_pos(10),
    groundpiece_ground_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Groundpiece (ground)", get_client_area()),
    groundpiece_solid_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Groundpiece (solid)", get_client_area()),
    groundpiece_transparent_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Groundpiece (transparent)", get_client_area()),
    groundpiece_remove_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Groundpiece (remove)", get_client_area()),

    hotspot_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Hotspot", get_client_area()),
    entrance_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Entrance", get_client_area()),
    exit_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Exit", get_client_area()),

    liquid_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Liquid", get_client_area()),
    weather_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Weather", get_client_area()),
    trap_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Trap", get_client_area()),
    worldobj_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Worldobject", get_client_area()),

    background_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Background", get_client_area()),
    prefab_button(CL_Rect(10, y_pos, 190, y_pos += 20), "Prefab", get_client_area()),
    file_button(CL_Rect(10, y_pos, 190, y_pos += 20), "From File", get_client_area()),

    close_button(CL_Rect(10, y_pos + 10, 190, y_pos + 30), "Close", get_client_area())
{

  close_button_slot = close_button.sig_clicked().connect(this, &ObjectSelectorWindow::on_close_press);
}

void
ObjectSelectorWindow::on_close_press()
{
  show(false);
}

/* EOF */
