//  $Id: groundpiece_obj.cxx,v 1.14 2003/04/02 20:02:28 grumbel Exp $
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
#include "../worldobjsdata/groundpiece_data.hxx"
#include "../pingus_resource.hxx"
#include "../editor/property_window.hxx"
#include "../editor/generic_property_frame.hxx"
#include "../editor/editor.hxx"
#include "../groundtype.hxx"
#include "groundpiece_obj.hxx"

namespace EditorObjs {

GroundpieceObj::GroundpieceObj (const WorldObjsData::GroundpieceData& data_)
  : SpriteEditorObj(data_.desc),
    data(new WorldObjsData::GroundpieceData(data_))
{
  // Groundpieces must not have a z-pos
  data->pos.z = 0;

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
  data->pos.x = int(data->pos.x);
  data->pos.y = int(data->pos.y);
  data->pos.z = int(data->pos.z);

  data->write_xml(xml);
}

std::string 
GroundpieceObj::status_line ()
{
  return "Groundpiece: " + data->desc.res_name
    + " Type: " + Groundtype::type_to_string(data->gptype)
    + " Rot: " + ResourceModifierNS::rs_to_string(data->desc.modifier);
}

void
GroundpieceObj::vertical_flip ()
{
  data->desc.modifier = ResourceModifierNS::vertical_flip(data->desc.modifier);
  sprite.get_surface() = PingusResource::load_surface(data->desc);
}

void
GroundpieceObj::horizontal_flip ()
{
  data->desc.modifier = ResourceModifierNS::horizontal_flip(data->desc.modifier);
  sprite.get_surface() = PingusResource::load_surface(data->desc);
}

void
GroundpieceObj::rotate_90 ()
{
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
  data->pos.x += sprite.get_width ()/2;
  data->pos.y += sprite.get_height()/2;

  data->desc.modifier = ResourceModifierNS::rotate_270(data->desc.modifier);
  sprite.get_surface() = PingusResource::load_surface(data->desc);

  data->pos.x -= sprite.get_width ()/2;
  data->pos.y -= sprite.get_height()/2;
}

struct GroundpieceObjRotate {
  GroundpieceObj* obj;
  GroundpieceObjRotate(GroundpieceObj* o) 
  {
    obj = o;
  }

  void operator()() {
    obj->rotate_90();
  }
};

struct GroundpieceObjFlipHorizontal {
  GroundpieceObj* obj;
  GroundpieceObjFlipHorizontal(GroundpieceObj* o) 
  {
    obj = o;
  }

  void operator()() {
    obj->horizontal_flip();
  }
};

struct GroundpieceObjFlipVertical {
  GroundpieceObj* obj;
  GroundpieceObjFlipVertical(GroundpieceObj* o) 
  {
    obj = o;
  }
  
  void operator()() {
    obj->vertical_flip();
  }
};

EditorNS::PropertyFrame*
GroundpieceObj::get_gui_dialog (EditorNS::Editor* editor)
{
  EditorNS::GenericPropertyFrame* propframe 
    = new EditorNS::GenericPropertyFrame("GroundPiece Properties",
                                         editor->get_property_window()->get_client_area());

  propframe->begin_add_enum_box("Groundtype:", (int*)&data->gptype);
  propframe->add_enum_value("Ground",      Groundtype::GP_GROUND);
  propframe->add_enum_value("Transparent", Groundtype::GP_TRANSPARENT);
  propframe->add_enum_value("Solid",       Groundtype::GP_SOLID);
  propframe->add_enum_value("Bridge",      Groundtype::GP_BRIDGE);
  propframe->add_enum_value("Remove",      Groundtype::GP_REMOVE);
  propframe->end_add_enum_box();
  
  propframe->add_button_box("Rotate 90 (r)",       GroundpieceObjRotate(this));
  propframe->add_button_box("Flip Vertical (v)",   GroundpieceObjFlipVertical(this));
  propframe->add_button_box("Flip Horizontal (h)", GroundpieceObjFlipHorizontal(this));
  
  return propframe;
}

} // namespace EditorObjs

/* EOF */
