//  $Id: surface_background_obj.cxx,v 1.12 2003/10/19 12:25:47 grumbel Exp $
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

#include "../string_converter.hxx"
#include "../worldobjsdata/surface_background_data.hxx"
#include "../editor/editor.hxx"
#include "../editor/generic_property_frame.hxx"
#include "../editor/property_frame.hxx"
#include "../editor/property_window.hxx"
#include "surface_background_obj.hxx"

namespace Pingus {
namespace EditorObjs {

using namespace EditorNS;

SurfaceBackgroundObj::SurfaceBackgroundObj (const WorldObjsData::SurfaceBackgroundData& data_)
  : SpriteEditorObj (data_.desc.res_name, data_.desc.datafile),
    data(new WorldObjsData::SurfaceBackgroundData(data_))
{
  pos_ref = &data->pos;
}

SurfaceBackgroundObj::~SurfaceBackgroundObj ()
{
  delete data;
}

void
SurfaceBackgroundObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

EditorNS::EditorObj*
SurfaceBackgroundObj::duplicate ()
{
  return new SurfaceBackgroundObj(*data);
}

std::string
SurfaceBackgroundObj::status_line ()
{
  return "SurfaceBackground: "
         + to_string(data->pos.x) + ", "
         + to_string(data->pos.y) + ", "
         + to_string(data->pos.z);
}

EditorNS::PropertyFrame*
SurfaceBackgroundObj::get_gui_dialog(EditorNS::Editor* editor)
{
  GenericPropertyFrame* propframe
    = new GenericPropertyFrame("SurfaceBackground", editor->get_property_window()->get_client_area());

  propframe->add_float_box("Parallax-X", &data->para_x);
  propframe->add_float_box("Parallax-Y", &data->para_y);

  propframe->add_float_box("Scroll-Speed-X", &data->scroll_x);
  propframe->add_float_box("Scroll-Speed-Y", &data->scroll_y);

  propframe->add_check_box("Stretch-X", &data->stretch_x);
  propframe->add_check_box("Stretch-Y", &data->stretch_y);
  propframe->add_check_box("KeepAspect", &data->keep_aspect);

  propframe->add_float_box("Color Red", &data->color.red);
  propframe->add_float_box("Color Green", &data->color.green);
  propframe->add_float_box("Color Blue", &data->color.blue);
  propframe->add_float_box("Color Alpha", &data->color.alpha);

  //propframe->add_button_box("Hello World", Functor);

  return propframe;
}

} // namespace EditorObjs
} // namespace Pingus

/* EOF */
