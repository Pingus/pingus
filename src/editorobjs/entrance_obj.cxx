//  $Id: entrance_obj.cxx,v 1.3 2002/11/28 20:09:54 grumbel Exp $
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

#include <stdio.h>
#include <iostream>
#include "../pingus_error.hxx"
#include "../editor/entrance_window.hxx"
#include "../editor/editor.hxx"
#include "../editor/property_window.hxx"
#include "entrance_obj.hxx"

namespace EditorObjs {

EntranceObj::EntranceObj (const WorldObjsData::EntranceData& data_)
  : data(new WorldObjsData::EntranceData(data_)),
    direction(data->direction),
    release_rate(data->release_rate)
{
  pos_ref = &data->pos;

  std::cout << "EntranceObj::EntranceObj(const EntranceData& data): " 
	    << data->type << std::endl;

  if (data->type == "generic")
    {
      sprite = Sprite("Entrances/generic", "entrances");
      sprite.set_align_center_bottom();
    } 
  else if (data->type == "woodthing") 
    {
      std::cout << "WOODTHING" << std::endl;
      sprite = Sprite("Entrances/woodthing_mov", "entrances");
      sprite.set_align(0  - sprite.get_width()/2,
		       32 - sprite.get_height());
    }
  else if (data->type == "cloud")
    {
      sprite = Sprite("Entrances/cloud", "entrances");
      sprite.set_align(-115, -75);
    } 
  else 
    {
      std::cout << "Entrance obj error!" << std::endl;
      PingusError::raise("EntranceObj: Unknown entrance type: " + data->type);
    }
}

EntranceObj::~EntranceObj ()
{
  delete data;
}

EditorObj*
EntranceObj::duplicate ()
{
  std::cout << "EntranceObj::duplicate()" << std::endl;
  return new EntranceObj(*data);
}

void
EntranceObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

std::string
EntranceObj::status_line ()
{
  std::string dir_str;
  char str[256];

  switch(direction)
    {
      case WorldObjsData::EntranceData::LEFT:
        dir_str = "left";
        break;
      case WorldObjsData::EntranceData::RIGHT:
        dir_str = "right";
        break;
      case WorldObjsData::EntranceData::MISC:
        dir_str = "misc";
        break;
      default:
        dir_str = "not set - this is a bug";
    }

  snprintf(str, 256, "Entrance: %s Rate: %d Direction: %s Owner: %d",
	   data->type.c_str(), data->release_rate, dir_str.c_str(), data->owner_id);

  return str;
}

EditorNS::PropertyFrame*
EntranceObj::get_gui_dialog (EditorNS::Editor* parent)
{
  return new EditorNS::EntranceWindow(parent->get_property_window()->get_client_area(), this);
}

} // namespace EditorObjs

/* EOF */
