//  $Id: liquid_obj.cxx,v 1.5 2003/02/19 17:16:59 grumbel Exp $
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
#include "../editor/editor_view.hxx"
#include "../editor/editor.hxx"
#include "../editor/property_window.hxx"
#include "../editor/generic_property_frame.hxx"
#include "liquid_obj.hxx"

namespace EditorObjs {

LiquidObj::LiquidObj (const WorldObjsData::LiquidData& data_)
  : SpriteEditorObj(data_.desc.res_name, data_.desc.datafile),
    data(new WorldObjsData::LiquidData(data_))
{
  if (data->old_width_handling)
    {
      data->width = (data->width + sprite.get_width()) / sprite.get_width();
      data->old_width_handling = false;
    }

  pos_ref = &data->pos;
}

LiquidObj::~LiquidObj ()
{
  delete data;
}

EditorObj*
LiquidObj::duplicate ()
{
  return new LiquidObj(*data);
}

void
LiquidObj::draw (EditorNS::EditorView* view)
{
  for (int i = 0; i < data->width; ++i)
    view->draw(sprite, data->pos + Vector(i * sprite.get_width(), 0));
}

void
LiquidObj::draw_mark (EditorNS::EditorView* view) 
{
  view->draw_rect(static_cast<int>(data->pos.x),
		  static_cast<int>(data->pos.y),
		  static_cast<int>(data->pos.x + sprite.get_width () * data->width),
		  static_cast<int>(data->pos.y + sprite.get_height()),
		  1.0, 1.0, 1.0, 0.0);
}

void
LiquidObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

std::string  
LiquidObj::status_line ()
{
  char str[32];
  snprintf(str, 32, "%4.2f:%4.2f:%3.2f:%2d",
           data->pos.x, data->pos.y, data->pos.z, data->speed);
  return str;
}

int
LiquidObj::get_width ()
{
  return sprite.get_width() * data->width;
}

void 
LiquidObj::make_larger ()
{
  ++data->width;
}

void 
LiquidObj::make_smaller ()
{
  if (data->width > 1)
    --data->width;
}

EditorNS::PropertyFrame*
LiquidObj::get_gui_dialog(EditorNS::Editor* editor)
{
  EditorNS::GenericPropertyFrame* propframe 
    = new EditorNS::GenericPropertyFrame("Liquid Properties",
                                         editor->get_property_window()->get_client_area());

  propframe->add_integer_box("speed", &data->speed);

  return propframe;
}

} // namespace EditorObjs

/* EOF */
