//  $Id: groundpiece_obj.cxx,v 1.3 2002/09/28 11:52:24 torangan Exp $
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
#include "../editor/groundpiece_window.hxx"
#include "../worldobjsdata/groundpiece_data.hxx"
#include "../pingus_resource.hxx"
#include "groundpiece_obj.hxx"

namespace EditorObjs {

GroundpieceObj::GroundpieceObj (const WorldObjsData::GroundpieceData& data_)
  : SpriteEditorObj(data_.desc),
    data(new WorldObjsData::GroundpieceData(data_)),
    gptype(data->gptype)
{
  pos_ref = &data->pos;
}

GroundpieceObj::~GroundpieceObj ()
{
  delete data;
}

EditorObj*
GroundpieceObj::duplicate ()
{
  return new GroundpieceObj(*data);
}

void
GroundpieceObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

std::string 
GroundpieceObj::status_line ()
{
  char str[1024];  
  std::string type_name = Groundtype::type_to_string(gptype);
  
  snprintf(str, 1024, "Groundpiece: %s Type: %s",
	   data->desc.res_name.c_str(), type_name.c_str());

  return str;
}

void
GroundpieceObj::vertical_flip ()
{
  std::cout << "Vertical flip" << std::endl;
  data->desc.modifier = ResourceModifierNS::vertical_flip(data->desc.modifier);
  sprite.get_surface() = PingusResource::load_surface(data->desc);
}

void
GroundpieceObj::horizontal_flip ()
{
  std::cout << "Horz flip" << std::endl;
  data->desc.modifier = ResourceModifierNS::horizontal_flip(data->desc.modifier);
  sprite.get_surface() = PingusResource::load_surface(data->desc);
}

void
GroundpieceObj::rotate_90 ()
{
  std::cout << "rot90" << std::endl;

  data->pos.x += sprite.get_width ()/2;
  data->pos.y += sprite.get_height()/2;

  data->desc.modifier = ResourceModifierNS::rotate_90(data->desc.modifier);
  sprite.get_surface() = PingusResource::load_surface(data->desc);

  data->pos.x -= sprite.get_width ()/2;
  data->pos.y -= sprite.get_height()/2;
}

void
GroundpieceObj::rotate_270 ()
{
  std::cout << "rot 270" << std::endl;

  data->pos.x += sprite.get_width ()/2;
  data->pos.y += sprite.get_height()/2;

  data->desc.modifier = ResourceModifierNS::rotate_270(data->desc.modifier);
  sprite.get_surface() = PingusResource::load_surface(data->desc);

  data->pos.x -= sprite.get_width ()/2;
  data->pos.y -= sprite.get_height()/2;
}

EditorNS::PropertyFrame*
GroundpieceObj::get_gui_dialog (CL_Component* parent)
{
  return new EditorNS::GroundpieceWindow(parent, this);
}

} // namespace EditorObjs

/* EOF */
